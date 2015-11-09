#include "IShapeParser.h"
#include "Shapes.h"

int main()
{
	auto parser = IShapeParser::createParser();
	auto shapes = parser->parseShapes(std::cin);
	std::cout << "Parsed the following shapes: \n";
	for(auto const &shape : shapes)
	{
		shape->print();
	}
}
