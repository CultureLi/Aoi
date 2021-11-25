#include <cmath>
#include "Point2D.h"
//#define ZERO_LIMIT (1. / 10000.)
//#define NEAR_ZERO(exp) (std::abs(exp) <= ZERO_LIMIT)
//#define NEAR_EQUAL(v1, v2) (NEAR_ZERO((v1) - (v2)))

namespace Custom_Develop
{
	Point2D::Point2D()
		: x(0.0), y(0.0)
	{}


	Point2D::Point2D(float x, float y)
		: x(x), y(y)
	{}


	Point2D::~Point2D()
	{}


	Point2D::Point2D(const Point2D& other)
		: x(other.x), y(other.y)
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
		return NearEqual(rhs, 0.2f);
		//return NEAR_EQUAL(x, rhs.x) && NEAR_EQUAL(y, rhs.y);
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

	Point2D Point2D::Min(const Point2D& p1, const Point2D& p2) const
	{
		return Point2D(std::fmin(p1.x, p2.x), std::fmin(p1.y, p2.y));
	}

	Point2D Point2D::Max(const Point2D& p1, const Point2D& p2) const
	{
		return Point2D(std::fmax(p1.x, p2.x), std::fmax(p1.y, p2.y));
	}

	bool Point2D::NearEqual(const Point2D& other, float ep = 1e-06) const
	{
		return (this->operator-(other)).Length() <= ep;
		//return NEAR_EQUAL(x, other.x) && NEAR_EQUAL(y, other.y);
	}

	float Point2D::Length()
	{
		return std::sqrt(x * x + y * y);
	}

	std::ostream& operator<<(std::ostream& out, const Point2D& point)
	{
		out << "Point2D(" << point.x << ", " << point.y << ")";
		return out;
	}

	float Point2D::Distance(const Point2D& p1, const Point2D& p2)
	{
		return (float)(std::sqrt((float)(
			(p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y))));

	}

	float Point2D::DistanceSqrt(const Point2D& p1, const Point2D& p2)
	{
		return (float)((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));

	}

	uint16_t Point2D::CheckDistance(const Point2D& p1, const Point2D& p2, float distance)
	{
		float r = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
		if (std::abs(r - distance) < 1e-05)
			return 0;
		else if (r > distance)
			return 1;
		else
			return -1;
	}
}