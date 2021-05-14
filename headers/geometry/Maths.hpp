#pragma once

namespace Maths{

	inline Uint8 clamp0_255(int n) {
		int tmp = 255;
		tmp -= n;
		tmp >>= 31;
		tmp |= n;
		n >>= 31;
		n = ~n;
		n &= tmp;
		return n;
	}

	// concaténer une valeur de pixel et un flottant
	inline Uint8 concatF(const Uint8& a, const Uint8& b) {
		return clamp0_255(a + b);
	}

	inline Uint8 concat(const Uint8& a, const Uint8& b) {
		const Uint16 res = (Uint16)a + (Uint16)b;
		if (res >= 255)return 255;
		if (res <= 0)return 0;
		return res;
	}

	// fonction de signe pour les coordonnées barycentriques
	inline const int sign(const Point2D<int>& p1, const Point2D<int>& p2, const Point2D<int>& p3) {
		//return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
	}

    /* @brief Renvoie le vecteur normale au plan du triangle
     * @param t un triangle
    */
	Vector getNormalVector(const Vertex& pl1, const Vertex& pl2, const Vertex& pl3) {
		Vector plNormal = Vector(pl2.x - pl1.x, pl2.y - pl1.y, pl2.z - pl1.z).cross(Vector(pl3.x - pl1.x, pl3.y - pl1.y, pl3.z - pl1.z));
		plNormal.normalize();
		return plNormal;
	}

	// renvoie 1 si x positif, -1 si x negatif et sinon 0
	inline int sign(const float& x) { return (0.0 < x) - (x < 0.0); }

	// renvoie x entre min et max
	inline float clamp(const float& x, const float& min, const float& max) {
		if (x < min) return min;
		else if (x > max) return max;
		else return x;
	}

	inline int clamp(const int& x, const int& min, const int& max) {
		if (x < min) return min;
		else if (x > max) return max;
		else return x;
	}

	// renvoie le point du segment le plus proche du point
	Vertex ClosestPointOnLine(const Vertex& l1, const Vertex& l2, const Vertex& p) {
		const Vector line = l2 - l1;
		float t = Vector(p - l1).dot(line) / line.dot(line);
		t = clamp(t, 0.0, 1.0);
		return Vertex(l1.x + t * line.x, l1.y + t * line.y, l1.z + t * line.z);
	}

	// renvoie true si le point est dans le triangle, false autrement
	inline bool PointInTriangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c) {
		const Vector ta = a - p;
		const Vector tb = b - p;
		const Vector tc = c - p;

		const Vector u = tb.cross(tc);
		const Vector v = tc.cross(ta);
		const Vector w = ta.cross(tb);

		if (u.dot(v) < 0.0) return false;
		if (u.dot(w) < 0.0) return false;
		return true;
	}

	Vertex getIntersectionWithPlane(const Vertex& rayPoint, const Vector& rayVector, const Vector& planeNormal, const Vertex& planePoint) {
		if (rayVector.x == 0 && rayVector.y == 0 && rayVector.z == 0) {
			return Vertex();
		}
		if (planeNormal.x == 0 && planeNormal.y == 0 && planeNormal.z == 0) {
			return Vertex();
		}
		const Vector diff = rayPoint - planePoint;
		const double prod1 = diff.dot(planeNormal);
		const double prod2 = rayVector.dot(planeNormal);
		if (prod2 == 0) { return Vertex(); }
		const double prod3 = prod1 / prod2;
		if (abs(prod3) == INFINITY) {
			return Vertex();
		}
		const Vertex res = rayPoint - (rayVector * prod3);
		return res;
	}

	// renvoie le point du plan le plus proche du point
	Vertex ClosestPointOnPlane(const Vertex& pl1, const Vertex& pl2, const Vertex& pl3, const Vertex& p) {
		const Vector plNormal = getNormalVector(pl1, pl2, pl3);
		const float distance = plNormal.dot(p.getOriginVector()) - plNormal.dot(pl1.getOriginVector());
		if (distance == 0)return p;
		const Vertex result(p.x - distance * plNormal.x, p.y - distance * plNormal.y, p.z - distance * plNormal.z);
		return result;
	}

	// renvoie la distance point triangle
	float distancePointTriangle(const Vertex& p, const Vertex& a, const Vertex& b, const Vertex& c) {
		const Vector v21 = { b.x - a.x,b.y - a.y,b.z - a.z };
		const Vector p1 = { p.x - a.x,p.y - a.y,p.z - a.z };
		const Vector v32 = { c.x - b.x,c.y - b.y,c.z - b.z };
		const Vector p2 = { p.x - b.x,p.y - b.y,p.z - b.z };
		const Vector v13 = { a.x - c.x,a.y - c.y,a.z - c.z };
		const Vector p3 = { p.x - c.x,p.y - c.y,p.z - c.z };
		const Vector nor = v21.cross(v13);
		if (Maths::sign(v21.cross(nor).dot(p1)) + Maths::sign((v32.cross(nor).dot(p2)) + Maths::sign(v13.cross(nor).dot(p3)) < 2)) {
			const Vector vec1 = v21 * Maths::clamp(v21.dot(p1) / v21.dot(v21), 0.0, 1.0) - p1;
			const Vector vec2 = v32 * Maths::clamp(v32.dot(p2) / v32.dot(v32), 0.0, 1.0) - p2;
			const Vector vec3 = v13 * Maths::clamp(v13.dot(p3) / v13.dot(v13), 0.0, 1.0) - p3;
			return sqrtf(std::min(std::min(vec1.dot(vec1), vec2.dot(vec2)), vec3.dot(vec3)));
		}
		else {
			return sqrtf(nor.dot(p1) * nor.dot(p1) / nor.dot(nor));
		}
	}

	// renvoie le point du triangle le plus proche du point
	Vertex ClosestPointOnTriangle(const Vertex& point, const Vertex& a, const Vertex& b, const Vertex& c) {
		const Vertex p = ClosestPointOnPlane(a, b, c, point);

		if (PointInTriangle(p, a, b, c)) {
			return p;
		}
		const Vertex c1 = Maths::ClosestPointOnLine(a, b, p);
		const Vertex c2 = Maths::ClosestPointOnLine(b, c, p);
		const Vertex c3 = Maths::ClosestPointOnLine(c, a, p);

		const float mag1 = p.distance(c1);
		const float mag2 = p.distance(c2);
		const float mag3 = p.distance(c3);

		const float min = std::min(std::min(mag1, mag2), mag3);

		if (min == mag1) {
			return c1;
		}
		else if (min == mag2) {
			return c2;
		}
		return c3;
	}

	void mult3x3_3x1(const Matrix<3,3>& m, Vertex v, Vertex& res) {
		res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
		res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
		res.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
	}

	void mult3x3_3x1(const Matrix<3, 3>& m, Vector v, Vector& res) {
		res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
		res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
		res.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
	}

	// rotate around X-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateX(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { 1,0,0 };
		a.m[1] = { 0,cos(angle),-sin(angle) };
		a.m[2] = { 0,sin(angle),cos(angle) };
		mult3x3_3x1(a, p, p);
	}
	void rotateX(Vector& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { 1,0,0 };
		a.m[1] = { 0,cos(angle),-sin(angle) };
		a.m[2] = { 0,sin(angle),cos(angle) };
		mult3x3_3x1(a, p, p);
	}

	// rotate around Y-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateY(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),0,sin(angle) };
		a.m[1] = { 0,1,0 };
		a.m[2] = { -sin(angle),0,cos(angle) };
		mult3x3_3x1(a, p, p);
	}
	void rotateY(Vector& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),0,sin(angle) };
		a.m[1] = { 0,1,0 };
		a.m[2] = { -sin(angle),0,cos(angle) };
		mult3x3_3x1(a, p, p);
	}

	// rotate around Z-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateZ(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),-sin(angle),0 };
		a.m[1] = { sin(angle),cos(angle),0 };
		a.m[2] = { 0,0,1 };
		mult3x3_3x1(a, p, p);
	}
	void rotateZ(Vector& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),-sin(angle),0 };
		a.m[1] = { sin(angle),cos(angle),0 };
		a.m[2] = { 0,0,1 };
		mult3x3_3x1(a, p, p);
	}
}