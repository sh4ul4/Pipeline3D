#include "Point.hpp"

template <class T>
Point2D Point2D::operator*(const T& val)const {
	return { x * val,y * val };
}

template <class T>
Point2D Point2D::operator/(const T& val)const {
	return { x / val,y / val };
}

template <class T>
Point2D Point2D::operator+(const T& val)const {
	return { x + val, y + val };
}

template <class T>
Point2D Point2D::operator-(const T& val)const {
	return { x - val,y - val };
}

int& Point2D::operator[](const int& i) {
	if (i < 0 || i > 1) {
		std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
		exit(1);
	}
	switch (i) {
	case 0: return x;
	case 1: return y;
	}
}

bool Point2D::operator==(const Point2D& p) const {
	return ((p.x == x) && (p.y == y));
}

void Point2D::print() const {
	std::cout << "[" << x << "|" << y << "]";
}

int Point2D::distance(const Point2D& a, const Point2D& b) {
	return static_cast<int>(sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}
int Point2D::distance(const Point2D& p) const {
	return static_cast<int>(sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)));
}

std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
	return (os << "[" << p.x << "|" << p.y << "]");
}



Point3D& Point3D::operator+(const Point3D& point) {
	x += point.x;
	y += point.y;
	z += point.z;
	return *this;
}

Point3D& Point3D::operator-(const Point3D& point) {
	x -= point.x;
	y -= point.y;
	z -= point.z;
	return *this;
}

Point3D& Point3D::operator*(const Point3D& point) {
	x *= point.x;
	y *= point.y;
	z *= point.z;
	return *this;
}

Point3D& Point3D::operator/(const Point3D& point) {
	x /= point.x;
	y /= point.y;
	z /= point.z;
	return *this;
}

template <class T>
Point3D& Point3D::operator*(const T& val) {
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

template <class T>
Point3D& Point3D::operator/(const T& val) {
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

int& Point3D::operator[](const int& i) {
	if (i < 0 || i > 2) {
		std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
		exit(1);
	}
	switch (i) {
	case 0: return x;
	case 1: return y;
	case 2: return z;
	}
}

inline bool Point3D::operator==(const Point3D& p) const {
	return ((p.x == x) && (p.y == y) && (p.z == z));
}

void Point3D::print() const {
	std::cout << "[" << x << "|" << y << "|" << z << "]";
}

std::ostream& operator<<(std::ostream& os, const Point3D& p)
{
	return (os << "[" << p.x << "|" << p.y << "|" << p.z << "]");
}