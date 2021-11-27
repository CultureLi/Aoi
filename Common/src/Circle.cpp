#define _MATH_DEFINES_DEFINED
#include <cmath>
#include "Circle.h"


namespace Custom_Develop
{
	Circle::Circle(Point2D center,float r):center(center),radius(r)
	{}

	bool Circle::Contain(Point2D& point)
	{
		return Point2D::DistanceSqrt(point, center) < radius*radius;
	}

	bool Circle::Contain(Custom_Develop::Rectangle& rect)
	{
		auto size = rect.bottomRight - rect.topLeft;
		auto v = Point2D();
		for(int x=rect.topLeft.x;x<=rect.bottomRight.x;x+=size.x)
		{
			for (int y = rect.topLeft.y; y <= rect.bottomRight.y; y += size.y)
			{
				v.x = x;
				v.y = y;
				if (!Contain(v))
					return false;
			}
		}
		
		return true;
	}

	EPosionalType Circle::CheckPosionalSide(Custom_Develop::Rectangle& rect)
	{
		auto size = rect.bottomRight - rect.topLeft;

		auto v = Point2D();
		int inSideCount = 0;
		for (int x = rect.topLeft.x; x <= rect.bottomRight.x; x += int(size.x))
		{
			for (int y = rect.topLeft.y; y <= rect.bottomRight.y; y += int(size.y))
			{
				v.x = x;
				v.y = y;
				if (Contain(v))
					inSideCount++;
			}
		}


		if (inSideCount == 4)
			return EPosionalType::E_Inside;
		else if (inSideCount == 0)
			return EPosionalType::E_Outside;
		else
			return EPosionalType::E_Intersect;
	}

	Custom_Develop::Rectangle Circle::GetOutSideRect()
	{
		auto size = Point2D(radius, radius);
		return Custom_Develop::Rectangle(center - size, center + size);
	}
	Custom_Develop::Rectangle Circle::GetInSideRect()
	{
		float sr = 0.70710678 * radius;
		auto size = Point2D(sr,sr);
		return Custom_Develop::Rectangle(center - size, center + size);
	}
}