#include "ShapeParser.h"
#include "Shapes.h"

#include <boost/spirit/home/x3.hpp>

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
namespace ShapeGrammar
{
	using AShape = IShapeParser::AShape;
	x3::rule<struct ShapeExpression, AShape> const shapeExpression = "shapeExpression";
	using x3::lit;
	using x3::_val;
	using x3::_attr;
	using x3::double_;
	namespace mpl = boost::mpl;

	auto createCircle = [](auto &ctx){ _val(ctx) = std::make_unique<Circle>(_attr(ctx)); };
	auto createRectangle = [](auto &ctx)
			{
				auto const &attr = _attr(ctx);
				auto a = boost::fusion::at<mpl::int_<0>>(attr);
				auto b = boost::fusion::at<mpl::int_<1>>(attr);
				_val(ctx) = std::make_unique<Rectangle>(a, b);
			};
	auto createTriangle = [](auto &ctx)
			{
				auto const &attr = _attr(ctx);
				auto a = boost::fusion::at<mpl::int_<0>>(attr);
				auto b = boost::fusion::at<mpl::int_<1>>(attr);
				auto c = boost::fusion::at<mpl::int_<2>>(attr);
				_val(ctx) = std::make_unique<Triangle>(a, b, c);
			};

	auto const shapeExpression_def =
			(lit("circle") > double_)[ createCircle ]
		 |  (lit("rectangle") > double_ >> double_)[ createRectangle ]
		 |  (lit("triangle") > double_ >> double_ >> double_)[ createTriangle ]
		 ;
	BOOST_SPIRIT_DEFINE(shapeExpression)
} //namespace ShapeGrammar
} //namespace unnamed

auto ShapeParser::parseOneShape(std::string const &str) -> AShape
{
	auto pos = str.cbegin();
	auto last = str.cend();
	AShape shape;
	namespace ascii = boost::spirit::x3::ascii;
	if( x3::phrase_parse(pos, last, ShapeGrammar::shapeExpression, ascii::space, shape) && pos == last )
		return shape;
	throw std::runtime_error("invalid shape");
}
