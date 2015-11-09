#include "ShapeParser.h"
#include "Shapes.h"

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

auto ShapeParser::parseOneShape(std::string const &) -> AShape
{
	//TODO
	return std::make_unique<Circle>(1.0);
}
