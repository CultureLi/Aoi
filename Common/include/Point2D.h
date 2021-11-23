#ifndef POINT2D_H_
#define POINT2D_H_
#include <ostream>
/** A simple 2D point class. */
class Point2D
{

public:

	Point2D();

	Point2D(float x, float y);

	Point2D(const Point2D& other);

	~Point2D();

	Point2D& operator=(const Point2D& rhs);

	Point2D& operator+=(const Point2D& rhs);

	Point2D& operator-=(const Point2D& rhs);

	Point2D& operator*=(const Point2D& rhs);

	Point2D& operator/=(const Point2D& rhs);

	Point2D& operator*=(float factor);

	Point2D& operator/=(float divisor);

	Point2D operator-() const;

	Point2D operator+(const Point2D& other) const;

	Point2D operator-(const Point2D& other) const;

	Point2D operator*(const Point2D& other) const;

	Point2D operator/(const Point2D& other) const;

	Point2D operator*(float factor) const;

	Point2D operator/(float divisor) const;

	bool operator==(const Point2D& rhs) const;

	bool operator!=(const Point2D& rhs) const;

	bool operator<(const Point2D& rhs) const;

	bool operator>(const Point2D& rhs) const;

	bool operator<=(const Point2D& rhs) const;

	bool operator>=(const Point2D& rhs) const;

	Point2D abs() const;

	bool isIn(const Point2D list[], int listSize) const;

	Point2D Min(const Point2D& p1, const Point2D& p2) const; 

	Point2D Max(const Point2D& p1, const Point2D& p2) const;

	bool NearEqual(const Point2D& other,float ep) const;

	float Length();

	friend std::ostream& operator<<(std::ostream& out, const Point2D& point);

	static float distance(const Point2D& p1, const Point2D& p2);
public:

	float x;

	float y;

}; // class Point2D



#endif // POINT2D_H_


