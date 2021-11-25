#pragma once

#ifndef Custom_Rect
#define Custom_Rect
#include "Point2D.h"

namespace Custom_Develop
{

	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(float tlX, float tlY, float brX, float brY);
		Rectangle(Point2D center, float length, float width);
		Rectangle(Point2D tl, Point2D br);

		~Rectangle();

		bool Contain(Point2D pos);
		bool Contain(float x, float y);
		bool Contain(Rectangle& other);
		Point2D topLeft, bottomRight;
	};
}
#endif // !Custom_Rect