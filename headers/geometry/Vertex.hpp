#pragma once

class Vector;

class Vertex {
public:
	float x, y, z;

	Vertex() :x(0), y(0), z(0) {}

	Vertex(const Point3D& p) :x(p.x), y(p.y), z(p.z) {}

	template <class T1, class T2, class T3>
	Vertex(const T1& x, const T2& y, const T3& z) : x((float)x), y((float)y), z((float)z) {}

	Vertex(const Vertex& v) :x(v.x), y(v.y), z(v.z) {}

	Vertex& operator=(const Vertex& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vertex& operator+=(const Vertex& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vertex& operator-=(const Vertex& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vertex& operator+=(const Vector& other);

	Vertex& operator-=(const Vector& other);

	float& operator[](const unsigned& i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
		std::cout << "Index out of bounds for instance of [Vertex]" << std::endl; exit(1);
	}

	Point3D toPoint3D() const {
		return Point3D(x, y, z);
	}

	float distance(const Vertex& other) const {
		return sqrtf((x - other.x) * (x - other.x) + 
			(y - other.y) * (y - other.y) +
			(z - other.z) * (z - other.z));
	}

	float distanceToLine(const Vertex& p0, const Vertex& p1) const;

	void print()const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}

	friend std::ostream& operator<<(std::ostream& os, const Vertex& v);

	float getDistanceToOrigin() const {
		return sqrtf(x * x + y * y + z * z);
	}

	Vector getOriginVector() const;
};

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
	return (os << "[" << v.x << "|" << v.y << "|" << v.z << "]");
}

inline Vector operator+(const Vertex& v1, const Vertex& v2);

inline Vector operator-(const Vertex& v1, const Vertex& v2);

inline Vertex operator+(const Vertex& v1, const Vector& v2);

inline Vertex operator-(const Vertex& v1, const Vector& v2);