#pragma once
//#include "library.h"
//#include "Draw.h"
#include "Texture.h"
//#include "Vertex.h"
#include "Camera.h"

//#define CLIP

class Triangle {
public:
	std::vector<Triangle> tmp;
	Vertex a, b, c;
	Point2 a2, b2, c2;
	float wa, wb, wc;
	Bitmap* bmp = nullptr;
	Point2 bmpA;
	Point2 bmpB;
	Point2 bmpC;
private:
	bool visible = false;
	bool fill = true;
	bool contour = true;
	bool farclipA = false;
	bool farclipB = false;
	bool farclipC = false;
	bool nearclipA = false;
	bool nearclipB = false;
	bool nearclipC = false;
public:
	Color color = black;
	float distanceToCamera = INFINITY;
public:
	Triangle(){}
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color = black, const bool& fill = true) :a(a), b(b), c(c), color(color), fill(fill) {}
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color,
		Bitmap* bmp, const Point2& bmpA, const Point2& bmpB, const Point2& bmpC)
		:a(a), b(b), c(c), color(color),
		bmp(bmp), bmpA(bmpA), bmpB(bmpB), bmpC(bmpC) {}
	/*Triangle(const Triangle& t) :a(t.a), b(t.b), c(t.c), a2(t.a2), b2(t.b2), c2(t.c2), bmp(t.bmp),
		visible(t.visible), fill(t.fill), contour(t.contour), nearclipA(t.nearclipA), nearclipB(t.nearclipB), nearclipC(t.nearclipC),
		farclipA(t.farclipA), farclipB(t.farclipB), farclipC(t.farclipC),
		color(t.color), distanceToCamera(t.distanceToCamera) {}*/
	
	inline bool operator<(const Triangle& node) const {
		return (this->distanceToCamera < node.distanceToCamera);
	}
	inline bool operator>(const Triangle& node) const {
		return (this->distanceToCamera > node.distanceToCamera);
	}
	inline void setColor(const Color& color) { this->color = color; }
	inline void move(const Vertex& movement) {
		a += movement;
		b += movement;
		c += movement;
	}
	std::vector<Matrix::m4d> getCoord()const {
		return { {{a.x,a.y,a.z}},{{b.x,b.y,b.z}}, {{c.x,c.y,c.z}} };
	}
	Point2 getIntersection(const Point2& a, const Point2& b, const int& maxX, const int& maxY)const {
		Point2 P;
		P = lineLineIntersection(a, b, { 0,0 }, { maxX,0 });
		if (isOnLineSegments(P, a, b, { 0,0 }, { maxX,0 })) {
			return P;
		}
		P = lineLineIntersection(a, b, { 0,0 }, { 0,maxY });
		if (isOnLineSegments(P, a, b, { 0,0 }, { 0,maxY })) {
			return P;
		}
		P = lineLineIntersection(a, b, { 0,maxY }, { maxX,maxY });
		if (isOnLineSegments(P, a, b, { 0,maxY }, { maxX,maxY })) {
			return P;
		}
		P = lineLineIntersection(a, b, { maxX,0 }, { maxX,maxY });
		if (isOnLineSegments(P, a, b, { maxX,0 }, { maxX,maxY })) {
			return P;
		}
		return { 0,0 };
	}
	void set2d(const Window& window, const bool& clip) {
		visible = false;
		distanceToCamera = distanceToPoint(Camera::getCurrent().getCameraPosition());
		// near clip
		Vertex a(this->a);
		Vertex b(this->b);
		Vertex c(this->c);
		nearclipA = nearclipB = nearclipC = false;
		const float rela = Camera::getCurrent().relationToClipPlane(a);
		const float relb = Camera::getCurrent().relationToClipPlane(b);
		const float relc = Camera::getCurrent().relationToClipPlane(c);
		if (rela <= -100 || relb <= -100 || relc <= -100)return; // early exit if point is too far behind clip plane
		if (rela <= 0)nearclipA = true;
		if (relb <= 0)nearclipB = true;
		if (relc <= 0)nearclipC = true;
		const int pointsToClip = nearclipA + nearclipB + nearclipC;
		if(pointsToClip == 0 || !clip)visible = true;
#ifdef CLIP
		else if (pointsToClip == 2) {
			// a inside clip-zone
			if (!nearclipA) {
				{
					const float n = abs(rela) / (abs(rela) + abs(relb));
					b.x = n * (b.x - a.x) + a.x;
					b.y = n * (b.y - a.y) + a.y;
					b.z = n * (b.z - a.z) + a.z;
				}
				{
					const float n = abs(rela) / (abs(rela) + abs(relc));
					c.x = n * (c.x - a.x) + a.x;
					c.y = n * (c.y - a.y) + a.y;
					c.z = n * (c.z - a.z) + a.z;
				}
				visible = true;
			}
			// b inside clip-zone
			else if (!nearclipB) {
				{
					const float n = abs(relb) / (abs(relb) + abs(rela));
					a.x = n * (a.x - b.x) + b.x;
					a.y = n * (a.y - b.y) + b.y;
					a.z = n * (a.z - b.z) + b.z;
				}
				{
					const float n = abs(relb) / (abs(relb) + abs(relc));
					c.x = n * (c.x - b.x) + b.x;
					c.y = n * (c.y - b.y) + b.y;
					c.z = n * (c.z - b.z) + b.z;
				}
				visible = true;
			}
			// c inside clip-zone
			else if (!nearclipC) {
				{
					const float n = abs(relc) / (abs(relc) + abs(rela));
					a.x = n * (a.x - c.x) + c.x;
					a.y = n * (a.y - c.y) + c.y;
					a.z = n * (a.z - c.z) + c.z;
				}
				{
					const float n = abs(relc) / (abs(relc) + abs(relb));
					b.x = n * (b.x - c.x) + c.x;
					b.y = n * (b.y - c.y) + c.y;
					b.z = n * (b.z - c.z) + c.z;
				}
				visible = true;
			}
		}
		else if (pointsToClip == 1) {
			if (nearclipA) {
				const float abn = abs(relb) / (abs(relb) + abs(rela));
				const Vertex ab(
					abn * (a.x - b.x) + b.x,
					abn * (a.y - b.y) + b.y,
					abn * (a.z - b.z) + b.z
				);

				const float acn = abs(relc) / (abs(relc) + abs(rela));
				const Vertex ac(
					acn * (a.x - c.x) + c.x,
					acn * (a.y - c.y) + c.y,
					acn * (a.z - c.z) + c.z
				);
				tmp.emplace_back(c, b, ab, black,
					bmp,bmpA,bmpB,bmpC);
				tmp.back().fillIt(true);
				b = ab; a = ac;
				//a = ab + ac;
				//a /= 2;
				visible = true;
			}
			else if (nearclipB) {
				const float ban = abs(rela) / (abs(rela) + abs(relb));
				const Vertex ba(
					ban * (b.x - a.x) + a.x,
					ban * (b.y - a.y) + a.y,
					ban * (b.z - a.z) + a.z
				);

				const float bcn = abs(relc) / (abs(relc) + abs(relb));
				const Vertex bc(
					bcn * (b.x - c.x) + c.x,
					bcn * (b.y - c.y) + c.y,
					bcn * (b.z - c.z) + c.z
				);
				tmp.emplace_back(c, a, ba, black,
					bmp, bmpA, bmpB, bmpC);
				tmp.back().fillIt(true);
				b = ba; a = bc;
				//b = ba + bc;
				//b /= 2;
				visible = true;
			}
			else if (nearclipC) {
				const float cbn = abs(relb) / (abs(relb) + abs(relc));
				const Vertex cb(
					cbn * (c.x - b.x) + b.x,
					cbn * (c.y - b.y) + b.y,
					cbn * (c.z - b.z) + b.z
				);

				const float can = abs(rela) / (abs(rela) + abs(relc));
				const Vertex ca(
					can * (c.x - a.x) + a.x,
					can* (c.y - a.y) + a.y,
					can* (c.z - a.z) + a.z
				);
				tmp.emplace_back(a, b, ca, black,
					bmp, bmpA, bmpB, bmpC);
				tmp.back().fillIt(true);
				c = cb; a = ca;
				//c = cb + ca;
				//c /= 2;
				visible = true;
			}
		}
#endif // CLIP
		else {
			visible = false;
			return;
		}

		// far-clipping + 2D points calculation
		
		if ((distanceToCamera > Camera::getCurrent().far) == false) {
			a2 = Camera::getCurrent().get2D({ a.x,a.y,a.z,1 }, farclipA, wa, window);
			b2 = Camera::getCurrent().get2D({ b.x,b.y,b.z,1 }, farclipB, wb, window);
			c2 = Camera::getCurrent().get2D({ c.x,c.y,c.z,1 }, farclipC, wc, window);
		}
		if (/*!nearclipA && !nearclipB && !nearclipC &&*/!farclipA && !farclipB && !farclipC && visible) { // no clip
			visible = true;
		}
		else visible = false;
	}
	void render(const Window& window, GlobalTexture& globalTexture)  {
		if (!visible) return;
		if (abs(a2.x) > 10000 || abs(a2.y) > 10000 ||
			abs(b2.x) > 10000 || abs(b2.y) > 10000 ||
			abs(c2.x) > 10000 || abs(c2.y) > 10000) {
			//clip here
			return;
		}
		// render
		for (Triangle t : tmp) {
			t.set2d(window, false);
			t.render(window, globalTexture);
		}tmp.clear(); tmp.shrink_to_fit();
		const Vertex sunLight = Camera::getCurrent().getSunLight();
		float light = getPlaneNormal().dot(sunLight);
		light /= 30;
		light = abs(light);
		if (fill) {
			if (bmp != nullptr) renderTexture(*bmp, globalTexture, light);
			else renderTexture(color, globalTexture, light);
		}
		else if (contour) {
			/*Vertex campos = Camera::getCurrent().getCameraPosition();
			globalTexture.drawLine(globalTexture, a2, a.distance(campos), b2, b.distance(campos), color);
			globalTexture.drawLine(globalTexture, b2, b.distance(campos), c2, c.distance(campos), color);
			globalTexture.drawLine(globalTexture, a2, a.distance(campos), c2, c.distance(campos), color);*/
			globalTexture.drawLine(globalTexture, a2, 0, b2, 0, color);
			globalTexture.drawLine(globalTexture, b2, 0, c2, 0, color);
			globalTexture.drawLine(globalTexture, a2, 0, c2, 0, color);
		}
	}
	inline void fillIt(const bool& b) {
		this->fill = b;
	}
	inline void contourIt(const bool& b) {
		this->contour = b;
	}
private:
	inline int sign(const float& n) const {
		return (0.0 < n) - (n < 0.0);
	}
	inline float clamp(const float& x, const float& min, const float& max) const {
		if (x < min) return min;
		else if (x > max) return max;
		else return x;
	}
public:
	float distanceToPoint(const Vertex& p) const {
		const Matrix::vec3d v21 = { b.x - a.x,b.y - a.y,b.z - a.z }; const Matrix::vec3d p1 = { p.x - a.x,p.y - a.y,p.z - a.z };
		const Matrix::vec3d v32 = { c.x - b.x,c.y - b.y,c.z - b.z }; const Matrix::vec3d p2 = { p.x - b.x,p.y - b.y,p.z - b.z };
		const Matrix::vec3d v13 = { a.x - c.x,a.y - c.y,a.z - c.z }; const Matrix::vec3d p3 = { p.x - c.x,p.y - c.y,p.z - c.z };
		const Matrix::vec3d nor = Matrix::cross(v21, v13);
		if ((sign(Matrix::dotV3(Matrix::cross(v21, nor), p1)) + sign(Matrix::dotV3(Matrix::cross(v32, nor), p2)) + sign(Matrix::dotV3(Matrix::cross(v13, nor), p3)) < 2)) {
			return sqrtf(Min(Min(
				Matrix::dot2V3(Matrix::minus(Matrix::mult(v21, clamp(Matrix::dotV3(v21, p1) / Matrix::dot2V3(v21), 0.0, 1.0)), p1)),
				Matrix::dot2V3(Matrix::minus(Matrix::mult(v32, clamp(Matrix::dotV3(v32, p2) / Matrix::dot2V3(v32), 0.0, 1.0)), p2))),
				Matrix::dot2V3(Matrix::minus(Matrix::mult(v13, clamp(Matrix::dotV3(v13, p3) / Matrix::dot2V3(v13), 0.0, 1.0)), p3))));
		}
		else {
			return sqrtf(Matrix::dotV3(nor, p1) * Matrix::dotV3(nor, p1) / Matrix::dot2V3(nor));
		}
	}

	static Vertex ClosestPointOnPlane(const Vertex& pl1, const Vertex& pl2, const Vertex& pl3, const Vertex& point) {
		Vertex plNormal = Vertex::cross(Vertex(pl2.x - pl1.x, pl2.y - pl1.y, pl2.z - pl1.z), Vertex(pl3.x - pl1.x, pl3.y - pl1.y, pl3.z - pl1.z));
		plNormal.normalize();
		const float distance = plNormal.dot(point) - plNormal.dot(pl1);
		if (distance == 0)return point;
		const Vertex result(point.x - distance * plNormal.x, point.y - distance * plNormal.y, point.z - distance * plNormal.z);
		return result;
	}

	bool PointInTriangle(const Vertex& point) const {
		const Matrix::vec3d ta = { a.x - point.x,a.y - point.y,a.z - point.z };
		const Matrix::vec3d tb = { b.x - point.x,b.y - point.y,b.z - point.z };
		const Matrix::vec3d tc = { c.x - point.x,c.y - point.y,c.z - point.z };

		const Matrix::vec3d u = Matrix::cross(tb, tc);
		const Matrix::vec3d v = Matrix::cross(tc, ta);
		const Matrix::vec3d w = Matrix::cross(ta, tb);

		if (Matrix::dotV3(u, v) < 0.0) {
			return false;
		}
		if (Matrix::dotV3(u, w) < 0.0) {
			return false;
		}
		return true;
	}

	Point3 ClosestPointOnLine(const Vertex& l1, const Vertex& l2, const Vertex& p) const {
		const Matrix::vec3d a = { l1.x,l1.y,l1.z };
		const Matrix::vec3d b = { l2.x,l2.y,l2.z };

		float t = Matrix::dotV3({ p.x - a[0],p.y - a[1],p.z - a[2] },
			{ b[0] - a[0],b[1] - a[1],b[2] - a[2] }) / Matrix::dotV3({ b[0] - a[0],b[1] - a[1],b[2] - a[2] },
				{ b[0] - a[0],b[1] - a[1],b[2] - a[2] }
		);

		t = clamp(t, 0.0, 1.0);
		const Point3 d(a[0] + t * b[0] - a[0], a[1] + t * b[1] - a[1], a[2] + t * b[2] - a[2]);
		return d;
	}

	inline float distancePointPoint(const Vertex& a, const Vertex& b) const {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}

	Vertex ClosestPoint(const Vertex& point) const {
		const Vertex p = ClosestPointOnPlane(a,b,c, point);

		if (PointInTriangle(p)) {
			return p;
		}
		const Point3 c1 = ClosestPointOnLine(a,b, p);
		const Point3 c2 = ClosestPointOnLine(b,c, p);
		const Point3 c3 = ClosestPointOnLine(c,a, p);

		const float mag1 = distancePointPoint(p, c1);
		const float mag2 = distancePointPoint(p, c2);
		const float mag3 = distancePointPoint(p, c3);

		const float min = Min(Min(mag1, mag2),mag3);

		if (min == mag1) {
			return c1;
		}
		else if (min == mag2) {
			return c2;
		}
		return c3;
	}

	static Vertex getIntersectionWithPlane(const Vertex& rayPoint, const Vertex& rayVector, const Vertex& planeNormal, const Vertex& planePoint) {
		if (rayVector.x == 0 && rayVector.y == 0 && rayVector.z == 0) {
			return Vertex();
		}
		if (planeNormal.x == 0 && planeNormal.y == 0 && planeNormal.z == 0) {
			return Vertex();
		}
		const Vertex diff = rayPoint - planePoint;
		const double prod1 = diff.dot(planeNormal);
		const double prod2 = rayVector.dot(planeNormal);
		if (prod2 == 0) { return Vertex(); }
		const double prod3 = prod1 / prod2;
		if (abs(prod3) == INFINITY) {
			return Vertex();
		}
		const Vertex res = rayPoint - rayVector * prod3;
		return res;
	}
	inline float getTriangleArea() const {
		return abs(a2.x * (b2.y - c2.y) + b2.x * (c2.y - a2.y) + c2.x * (a2.y - b2.y)) / 2;
	}
	inline Vertex getPlaneNormal() const {
		return Vertex::cross(b - a, c - a);
	}
private:
	void renderTexture(const Bitmap& src, GlobalTexture& dst) const {
		const Vertex sun = Camera::getCurrent().getSun();
		float light = getPlaneNormal().dot(sun);
		light /= 30;
		light = abs(light);
		renderTexture(src, dst, light);
	}

	void renderTexture(const Bitmap& src, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(src,
			a2, b2, c2,
			bmpA,bmpB,bmpC,
			wa, wb, wc,
			dst,
			light);
	}

	void renderTexture(const Color& color, GlobalTexture& dst, const float& light) const {
		const float area = getTriangleArea();
		/*if(area > 200)
		TextureManager::transformv3(color, a2, b2, c2,
			a.distance(Camera::getCurrent().getCameraPosition()),
			b.distance(Camera::getCurrent().getCameraPosition()),
			c.distance(Camera::getCurrent().getCameraPosition()), dst);
		else TextureManager::transformv4(color, a2, b2, c2, dst);*/
		/*TextureManager::transformv3(color, a2, b2, c2,
			a.distance(Camera::getCurrent().getCameraPosition()),
			b.distance(Camera::getCurrent().getCameraPosition()),
			c.distance(Camera::getCurrent().getCameraPosition()), dst);*/
		TextureManager::rasterize(color, a2, b2, c2,
			wa, wb, wc,
			dst,light);
	}
};