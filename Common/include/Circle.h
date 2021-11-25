#pragma once
#include "Point2D.h"
#include "Rectangle.h"
namespace Custom_Develop
{
	class Circle
	{
	public:
		Circle(Point2D, float);

		bool Contain(Point2D&);
		bool Contain(Custom_Develop::Rectangle&);
		// check the posional relationship between circel and rect ：-1 inner；0 Intersect；1 outer 
		int CheckPosionalSide(Custom_Develop::Rectangle&);
		// 圆的外切矩形
		Custom_Develop::Rectangle GetOutSideRect();
		// 圆的内接矩形
		Custom_Develop::Rectangle GetInSideRect();

		float radius;
		Point2D center;
	};
}