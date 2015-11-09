#pragma once

#include <memory>
#include <vector>
class Shape;

class IShapeParser
{
public:
	virtual ~IShapeParser() = default;

	using AShape = std::unique_ptr<Shape>;
	virtual AShape parseOneShape(std::string const &str) = 0;
	using Shapes = std::vector<AShape>;
	virtual Shapes parseShapes(std::istream &str) = 0;

	static std::unique_ptr<IShapeParser> createParser();
};

