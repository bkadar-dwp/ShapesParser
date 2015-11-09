#include "IShapeParser.h"
#include "Shapes.h"

#include <fstream>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <inFile>\n";
		return 1;
	}
	std::ifstream str(argv[1]);
	auto parser = IShapeParser::createParser();
	auto shapes = parser->parseShapes(str);
	std::cout << "Parsed the following shapes: \n";
	for(auto const &shape : shapes)
	{
		shape->print();
	}
}
