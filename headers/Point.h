#pragma once

//#include "library.h"

struct Point2 {
	int x;
	int y;
	Point2() :x(0), y(0) {}
	Point2(const int& x, const int& y) :x(x), y(y) {}
	template<class T1, class T2>
	Point2(const T1& x, const T2& y) : x((int)x), y((int)y) {}
	Point2(const Point2& p) : x(p.x), y(p.y) {}
	Point2 operator+(const Point2& point)const {
		return { x + point.x,y + point.y };
	}
	Point2 operator-(const Point2& point)const {
		return { x - point.x,y - point.y };
	}
	template <class T>
	Point2 operator*(const T& val)const {
		return { x * val,y * val };
	}
	template <class T>
	Point2 operator/(const T& val)const {
		return { x / val,y / val };
	}
	template <class T>
	Point2 operator+(const T& val)const {
		return { x + val,y + val };
	}
	template <class T>
	Point2 operator-(const T& val)const {
		return { x - val,y - val };
	}
	const int& operator[](const int& i) {
		if (i < 0 || i > 1) {
			std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
			exit(1);
		}
		switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}
	bool operator==(const Point2& p) const {
		return ((p.x == x) && (p.y == y));
	}
	void print() const {
		std::cout << "[" << x << "|" << y << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point2& p);
	static int distance(const Point2& a, const Point2& b) {
		return static_cast<int>(sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
	}
	int distance(const Point2& p) const {
		return static_cast<int>(sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)));
	}
};

std::ostream& operator<<(std::ostream& os, const Point2& p)
{
	return (os << "[" << p.x << "|" << p.y << "]");
}

struct Point2F {
	float x;
	float y;
	Point2F() :x(0), y(0) {}
	Point2F(const float& x, const float& y) :x(x), y(y) {}
	template<class T1, class T2>
	Point2F(const T1& x, const T2& y) : x((float)x), y((float)y) {}
	Point2F(const Point2F& p) : x(p.x), y(p.y) {}
	Point2F operator+(const Point2F& point)const {
		return { x + point.x,y + point.y };
	}
	Point2F operator-(const Point2F& point)const {
		return { x - point.x,y - point.y };
	}
	template <class T>
	Point2F operator*(const T& val)const {
		return { x * val,y * val };
	}
	template <class T>
	Point2F operator/(const T& val)const {
		return { x / val,y / val };
	}
	template <class T>
	Point2F operator+(const T& val)const {
		return { x + val,y + val };
	}
	template <class T>
	Point2F operator-(const T& val)const {
		return { x - val,y - val };
	}
	const float& operator[](const int& i) {
		if (i < 0 || i > 1) {
			std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
			exit(1);
		}
		switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}
	bool operator==(const Point2F& p) const {
		return ((p.x == x) && (p.y == y));
	}
	void print() const {
		std::cout << "[" << x << "|" << y << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point2F& p);
	static float distance(const Point2F& a, const Point2F& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
	float distance(const Point2F& p) const {
		return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};

std::ostream& operator<<(std::ostream& os, const Point2F& p)
{
	return (os << "[" << p.x << "|" << p.y << "]");
}

struct Point3 {
	int x;
	int y;
	int z;
	Point3(void) :x(0), y(0), z(0) {}
	template<class T1, class T2, class T3>
	Point3(const T1& x, const T2& y, const T3& z) : x((int)x), y((int)y), z((int)z) {}
	Point3(const Point3& p) : x(p.x), y(p.y), z(p.z) {}
	Point3& operator+(const Point3& point) {
		x += point.x;
		y += point.y;
		z += point.z;
		return *this;
	}
	Point3& operator-(const Point3& point) {
		x -= point.x;
		y -= point.y;
		z -= point.z;
		return *this;
	}
	Point3& operator*(const Point3& point) {
		x *= point.x;
		y *= point.y;
		z *= point.z;
		return *this;
	}
	Point3& operator/(const Point3& point) {
		x /= point.x;
		y /= point.y;
		z /= point.z;
		return *this;
	}
	template <class T>
	Point3& operator*(const T& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	template <class T>
	Point3& operator/(const T& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	int& operator[](const int& i) {
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
	inline bool operator==(const Point3& p) const {
		return ((p.x == x) && (p.y == y) && (p.z == z));
	}
	void print() const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point3& p);
};

std::ostream& operator<<(std::ostream& os, const Point3& p)
{
	return (os << "[" << p.x << "|" << p.y << "|" << p.z << "]");
}