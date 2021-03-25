/**
 * @file Représente un Sommet à 3 coordonnées, issu du module de Géométrie 
 */ 

struct Sommet {
	//Coordonnées flotantes
	float x;
	float y;
	float z;
	Sommet() : x(0), y(0), z(0) {}
	Sommet(const Point3& p) :x(p.x), y(p.y), z(p.z) {}
	Sommet(const std::array<float, 3>& vec3d) :x(vec3d[0]), y(vec3d[1]), z(vec3d[2]) {}
	Sommet(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}
	template<class T1, class T2, class T3>
	Sommet(const T1& x, const T2& y, const T3& z) : x((float)x), y((float)y), z((float)z) {}
	Sommet (const Sommet& v) : x(v.x), y(v.y), z(v.z) {}
  	//Pren
	Sommet& operator=(const Sommet& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	Sommet& operator+=(const Sommet& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Sommet& operator-=(const Sommet& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	Sommet& operator*=(const float& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator*=(const int& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator*=(const double& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator/=(const float& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	Sommet& operator/=(const int& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	Sommet& operator/=(const double& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	const Sommet operator*(const float& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const double& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const int& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator/(const float& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const double& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const int& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	float& operator[](const unsigned& i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
		std::cout << "Index out of bounds for instance of [Vertex]" << std::endl; exit(1);
	}
	Point3 toPoint3()const {
		return Point3(x,y,z);
	}

	static void normalize(Sommet& v) {
		v /= v.getLength();
	}
	void normalize() {
		const float length = getLength();
		*this /= length;
	}

	static float dot(const Sommet& v1, const Sommet& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	float dot(const Sommet& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	static Sommet cross(const Sommet& v1, const Sommet& v2) {
		return Sommet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	Sommet cross(const Sommet& v)const {
		return Sommet(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	static float distance(const Sommet& a,const Sommet& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}
	float distance(const Sommet& v)const {
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}

	static float distanceToLine(const Sommet& P, const Sommet& P0, const Sommet& P1);
	float distanceToLine(const Sommet& P0, const Sommet& P1)const;

	void print(void)const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Sommet& v);

	void normalizeOnLength(const float& length) {
		const float sum = abs(x) + abs(y) + abs(z);
		if (sum == 0) {
			this->x = this->y = this->z = 0; return;
		}
		const float normalizedLength = length / sum;
		x *= normalizedLength;
		y *= normalizedLength;
		z *= normalizedLength;
	}
	float getLength(void) const {
		return sqrtf(x * x + y * y + z * z);
	}
};

inline const Sommet operator+(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline const Sommet operator-(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline const Sommet operator*(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline const Sommet operator/(const Sommet& v1, const Sommet& v2) {
	//if (v.x == 0 || v.y == 0 || v.z == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
	return Sommet(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

float Sommet::distanceToLine(const Sommet& P, const Sommet& P0, const Sommet& P1) {
	Sommet v = P1 - P0;
	const Sommet w = P - P0;

	const float c1 = w.dot(v);
	if (c1 <= 0)  // before P0
		return distance(P, P0);
	const float c2 = v.dot(v);
	if (c2 <= c1) // after P1
		return distance(P, P1);
	v *= (c1 / c2);
	return distance(P, P0 + v );
}
float Sommet::distanceToLine(const Sommet& P0, const Sommet& P1)const {
	Som v = P1 - P0;
	const Sommet w = *this - P0;

	const float c1 = w.dot(v);
	if (c1 <= 0)  // before P0
		return distance(*this, P0);
	const float c2 = v.dot(v);
	if (c2 <= c1) // after P1
		return distance(*this, P1);
	v *= (c1 / c2);
	return distance(*this, P0 + v);
}

std::ostream& operator<<(std::ostream& os, const Sommet& v)
{
	return (os << "[" << v.x << "|" << v.y << "|" << v.z << "]");
}