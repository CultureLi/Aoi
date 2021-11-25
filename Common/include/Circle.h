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
		// check the posional relationship between circel and rect ��-1 inner��0 Intersect��1 outer 
		int CheckPosionalSide(Custom_Develop::Rectangle&);
		// Բ�����о���
		Custom_Develop::Rectangle GetOutSideRect();
		// Բ���ڽӾ���
		Custom_Develop::Rectangle GetInSideRect();

		float radius;
		Point2D center;
	};
}