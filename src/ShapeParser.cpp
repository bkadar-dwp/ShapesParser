#include "ShapeParser.h"
#include "Shapes.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

std::unique_ptr<IShapeParser> IShapeParser::createParser()
{
	return std::make_unique<ShapeParser>();
}

auto ShapeParser::parseShapes(std::istream &str) -> Shapes
{
	Shapes result;
	std::string line;
	while(getline(str, line))
	{
		result.push_back(parseOneShape(line));
	}
	return result;
}

namespace x3 = boost::spirit::x3;

namespace {
namespace ShapeAST
{
	struct Circle
	{
		double r;
	};
	struct Rectangle
	{
		double a;
		double b;
	};
	struct Triangle
	{
		double a;
		double b;
		double c;
	};
	using Variant = boost::variant<Circle, Rectangle, Triangle>;
} // namespace ShapeAST

struct Factory : boost::static_visitor<Shape*> // boost::static_visitor<IShapeParser::AShape> doesn't work :-/
{
	Shape* operator()(ShapeAST::Circle const &c) const { return new Circle(c.r); }
	Shape* operator()(ShapeAST::Rectangle const &r) const { return new Rectangle(r.a, r.b); }
	Shape* operator()(ShapeAST::Triangle const &t) const { return new Triangle(t.a, t.b, t.c); }
};

} //namespace unnamed

BOOST_FUSION_ADAPT_STRUCT(ShapeAST::Circle, (double, r))
BOOST_FUSION_ADAPT_STRUCT(ShapeAST::Rectangle, (double, a)(double, b))
BOOST_FUSION_ADAPT_STRUCT(ShapeAST::Triangle, (double, a)(double, b)(double, c))

namespace {
namespace ShapeGrammar
{
	x3::rule<struct ShapeExpression, ShapeAST::Variant> const shapeExpression = "shapeExpression";
	x3::rule<struct CicleExpression, ShapeAST::Circle> const circleExpression = "circleExpression";
	x3::rule<struct RectangleExpression, ShapeAST::Rectangle> const rectangleExpression = "rectangleExpression";
	x3::rule<struct TriangleExpression, ShapeAST::Triangle> const triangleExpression = "triangleExpression";
	using x3::lit;
	using x3::double_;
	namespace mpl = boost::mpl;

	auto const circleExpression_def = lit("circle") > double_;
	auto const rectangleExpression_def = lit("rectangle") > double_ >> double_;
	auto const triangleExpression_def = lit("triangle") > double_ >> double_ >> double_;
	auto const shapeExpression_def = circleExpression | rectangleExpression | triangleExpression;
	BOOST_SPIRIT_DEFINE(circleExpression, rectangleExpression, triangleExpression, shapeExpression)
} //namespace ShapeGrammar
} //namespace unnamed

auto ShapeParser::parseOneShape(std::string const &str) -> AShape
{
	auto pos = str.cbegin();
	auto last = str.cend();
	ShapeAST::Variant ast;
	namespace ascii = boost::spirit::x3::ascii;
	if( x3::phrase_parse(pos, last, ShapeGrammar::shapeExpression, ascii::space, ast) && pos == last )
		return AShape{boost::apply_visitor(Factory(), ast)};
	throw std::runtime_error("invalid shape");
}
