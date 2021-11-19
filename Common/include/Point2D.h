#pragma   once 
#ifndef POINT_2D
#define POINT_2D

#include <cmath>
#include <iostream>

#define ZERO_LIMIT (1. / 10000.)
#define NEAR_ZERO(exp) (std::abs(exp) <= ZERO_LIMIT)
#define NEAR_EQUAL(v1, v2) (NEAR_ZERO((v1) - (v2)))

class Point2D
{

public:
	float x;
	float y;

public:
	Point2D::Point2D()
		: x(0),
		y(0)
	{}


	Point2D::Point2D(float x, float y)
		: x(x),
		y(y)
	{}


	Point2D::~Point2D()
	{}


	Point2D::Point2D(const Point2D& other)
		: x(other.x),
		y(other.y)
	{}


	Point2D& Point2D::operator=(const Point2D& rhs)
	{
		if (this != &rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
		}
		return *this;
	}


	Point2D& Point2D::operator+=(const Point2D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}


	Point2D& Point2D::operator-=(const Point2D& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}


	Point2D& Point2D::operator*=(const Point2D& rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		return *this;
	}


	Point2D& Point2D::operator/=(const Point2D& rhs)
	{
		this->x /= rhs.x;
		this->y /= rhs.y;
		return *this;
	}


	Point2D& Point2D::operator*=(float factor)
	{
		this->x *= factor;
		this->y *= factor;
		return *this;
	}


	Point2D& Point2D::operator/=(float divisor)
	{
		this->x /= divisor;
		this->y /= divisor;
		return *this;
	}


	Point2D Point2D::operator-() const
	{
		return Point2D(-x, -y);
	}


	Point2D Point2D::operator+(const Point2D& other) const
	{
		return Point2D(*this) += other;
	}


	Point2D Point2D::operator-(const Point2D& other) const
	{
		return Point2D(*this) -= other;
	}


	Point2D Point2D::operator*(const Point2D& other) const
	{
		return Point2D(*this) *= other;
	}


	Point2D Point2D::operator/(const Point2D& other) const
	{
		return Point2D(*this) /= other;
	}


	Point2D Point2D::operator*(float factor) const
	{
		return Point2D(*this) *= factor;
	}


	Point2D Point2D::operator/(float divisor) const
	{
		return Point2D(*this) /= divisor;
	}


	bool Point2D::operator==(const Point2D& rhs) const
	{
		return NEAR_EQUAL(x, rhs.x) && NEAR_EQUAL(y, rhs.y);
	}


	bool Point2D::operator!=(const Point2D& rhs) const
	{
		return !(*this == rhs);
	}


	bool Point2D::operator<(const Point2D& rhs) const
	{
		return (x < rhs.x) && (y < rhs.y);
	}


	bool Point2D::operator>(const Point2D& rhs) const
	{
		return (x > rhs.x) && (y > rhs.y);
	}


	bool Point2D::operator<=(const Point2D& rhs) const
	{
		return (x <= rhs.x) && (y <= rhs.y);
	}


	bool Point2D::operator>=(const Point2D& rhs) const
	{
		return (x >= rhs.x) && (y >= rhs.y);
	}


	Point2D Point2D::abs() const
	{
		return Point2D(std::abs(x), std::abs(y));
	}


	bool Point2D::isIn(const Point2D list[], int listSize) const
	{
		for (int i = 0; i < listSize; ++i)
		{
			if (list[i] == *this)
			{
				return true;
			}
		}
		return false;
	}

	Point2D Point2D::FloorInt()
	{
		x = uint32_t(x);
		y = uint32_t(y);
		return *this;
	}

	static float distance(const Point2D& p1, const Point2D& p2)
	{
		return (float)(std::sqrt((float)(
			(p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y))));
	
	}
};


#endif // !POINT_2D