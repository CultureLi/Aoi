#include "Rectangle.h"
#include "Point2D.h"

namespace Custom_Develop
{
	Rectangle::Rectangle()
	{
		topLeft = Point2D();
		bottomRight = Point2D();
	}

	Rectangle::Rectangle(float tlX, float tlY, float brX, float brY)
	{
		topLeft.x = tlX;
		topLeft.y = tlY;
		bottomRight.x = brX;
		bottomRight.y = brY;
	}

	Rectangle::Rectangle(Point2D center, float length, float width)
	{
		auto size = Point2D(length, width);
		topLeft = center - size;
		bottomRight = center + size;
	}

	Rectangle::Rectangle(Point2D tl, Point2D br)
	{
		topLeft = tl;
		bottomRight = br;
	}

	Rectangle::~Rectangle()
	{}

	bool Rectangle::Contain(Point2D pos)
	{
		return  topLeft <= pos && pos <= bottomRight;
	}

	bool Rectangle::Contain(float x, float y)
	{
		Point2D pos(x, y);
		return  topLeft <= pos && pos <= bottomRight;
	}
	bool Rectangle::Contain(Rectangle& other)
	{
		return (other.topLeft >= this->topLeft && other.bottomRight <= this->bottomRight);
	}
}