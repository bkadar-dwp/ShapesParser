#pragma once

#include "IShapeParser.h"

class ShapeParser : public IShapeParser
{
public:
	AShape parseOneShape(std::string const &str) override;
	Shapes parseShapes(std::istream &str) override;
};
