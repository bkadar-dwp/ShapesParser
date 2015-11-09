#pragma once

#include <iostream>

class Shape
{
public:
	virtual ~Shape() = default;
	virtual void print() const = 0;
};

class Rectangle: public Shape
{
public:
	Rectangle(double a, double b) : mA(a), mB(b) {}
	void print() const override { std::cout << "Rectangle(" << mA << ", " << mB << ")\n"; }
private:
	double mA;
	double mB;
};

class Circle: public Shape
{
public:
	explicit Circle(double r) : mR(r) {}
	void print() const override { std::cout << "Circle(" << mR << ")\n"; }
private:
	double mR;
};

class Triangle: public Shape
{
public:
	Triangle(double a, double b, double c) : mA(a), mB(b), mC(c)
	{
		if(!isValid())
			throw std::invalid_argument("The given sizes don't specify a triangle");
	}
	void print() const override { std::cout << "Triangle(" << mA << ", " << mB << ", " << mC << ")\n"; }
private:
	bool isValid() const
	{
		return mA + mB > mC && mA + mC > mB && mB + mC > mA;
	}

	double mA;
	double mB;
	double mC;
};
