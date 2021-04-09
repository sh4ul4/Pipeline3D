#pragma once

class Vertex;

class Vector {
public:
	float x, y, z;

	Vector() : x(0), y(0), z(0) {}

	Vector(const std::array<float, 3>& v) : x(v[0]), y(v[1]), z(v[2]) {}

	template <class T1, class T2, class T3>
	Vector(const T1& x, const T2& y, const T3& z) : x((float)x), y((float)y), z((float)z) {}

	Vector(const Vector& v) : x(v.x), y(v.y), z(v.z) {}

	Vertex toVertex()const {
		return Vertex(x, y, z);
	}

	Vector& operator=(const Vector& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector& operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <typename T>
	Vector& operator*=(const T& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	template <typename T>
	Vector& operator/=(const T& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}

	template <typename T>
	Vector operator*(const T& val) const {
		return Vector(x * val, y * val, z * val);
	}

	template <typename T>
	Vector operator/(const T& val) const {
		return Vector(x / val, y / val, z / val);
	}

	float& operator[](const unsigned& i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
		std::cout << "Index out of bounds for instance of [Vector]" << std::endl; exit(1);
	}

	void normalize() {
		const float length = getLength();
		*this /= length;
	}

	float dot(const Vector& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector cross(const Vector& v) const {
		return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	void print()const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& v);

	void normalizeOnLength(const float& length) {
		const float sum = std::abs(x) + std::abs(y) + std::abs(z);
		if (sum == 0) {
			this->x = this->y = this->z = 0; return;
		}
		const float normalizedLength = length / sum;
		*this *= normalizedLength;
	}

	float getLength() const {
		return sqrtf(x * x + y * y + z * z);
	}
};

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
	return (os << "[" << v.x << "|" << v.y << "|" << v.z << "]");
}

inline const Vector operator+(const Vector& v1, const Vector& v2) {
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline const Vector operator-(const Vector& v1, const Vector& v2) {
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline const Vector operator*(const Vector& v1, const Vector& v2) {
	return Vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline const Vector operator/(const Vector& v1, const Vector& v2) {
	return Vector(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

Vertex& Vertex::operator+=(const Vector& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vertex& Vertex::operator-=(const Vector& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

float Vertex::distanceToLine(const Vertex& p0, const Vertex& p1) const {
	Vector v = p1 - p0;
	const Vector w = *this - p0;

	const float c1 = w.dot(v);
	if (c1 <= 0)  // before P0
		return distance(p0);
	const float c2 = v.dot(v);
	if (c2 <= c1) // after P1
		return distance(p1);
	v *= (c1 / c2);
	return distance(p0 + v);
}

Vector Vertex::getOriginVector() const {
	return Vector(x, y, z);
}

inline Vector operator+(const Vertex& v1, const Vertex& v2) {
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector operator-(const Vertex& v1, const Vertex& v2) {
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vertex operator+(const Vertex& v1, const Vector& v2) {
	return Vertex(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vertex operator-(const Vertex& v1, const Vector& v2) {
	return Vertex(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}