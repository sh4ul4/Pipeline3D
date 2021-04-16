#pragma once
/**
 * @class Triangle permet d'encapsuler les données d'un triangle, à la fois sous sa forme 3D
 * dans l'environnement et sous sa forme 2D dans la frame.
 */
class Triangle {

public:

	// sommets du triangle en world-coordinates
	Vertex a, b, c;

	// sommets du triangle en screen-coordinates (clip-space)
	Vertex aScreen, bScreen, cScreen;

	// pointeur vers la texture bitmap du triangle
	Bitmap* bmp = nullptr;

	// sommets du triangle en coordonnées de texture
	Point2D bmpA, bmpB, bmpC;

	// couleur du triangle
	Color color = black;

	// distance du triangle à la caméra
	float distanceToCamera = INFINITY;

	// vecteur de triangles temporaires pour le clipping
	std::vector<Triangle> tmp;

	// sommet qui indique le vecteur normal du triangle
	Vector normalVec;

	// booléen pour indiquer la visibilité du triangle
	bool visible = false;

	// booléen pour indiquer si le triangle est rempli
	bool fill = true;

	// booléen pour indiquer si le triangle est contourné
	bool contour = true;
private:
	// variables propres au clipping
	bool farclipA = false;
	bool farclipB = false;
	bool farclipC = false;
	bool nearclipA = false;
	bool nearclipB = false;
	bool nearclipC = false;
	
public:

	Triangle(){}

	// constructeur sans texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vector& normalVec, const Color& color = black, const bool& fill = true)
		: a(a), b(b), c(c), normalVec(normalVec), color(color), fill(fill) {}

	// constructeur avec texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vector& normalVec,
		const Color& color, Bitmap* bmp, const Point2D& bmpA, const Point2D& bmpB, const Point2D& bmpC)
		: a(a), b(b), c(c), normalVec(normalVec), color(color),
		bmp(bmp), bmpA(bmpA), bmpB(bmpB), bmpC(bmpC) {}
	
	// comparaison de deux triangles en fonction de leurs distance à la caméra
	inline bool operator<(const Triangle& node) const {
		return (this->distanceToCamera < node.distanceToCamera);
	}
	inline bool operator>(const Triangle& node) const {
		return (this->distanceToCamera > node.distanceToCamera);
	}

	// bouger tout le triangle sur un vecteur
	inline void move(const Vector& movement) {
		a += movement;
		b += movement;
		c += movement;
	}

	// set screen coordinates and handle clipping
	void setScreenCoord(const Window& window, const bool& clip) {
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
			Matrix<4, 4> ma;
			ma.m[0] = { a.x,a.y,a.z,1 };
			Matrix<4, 4> mb;
			mb.m[0] = { b.x,b.y,b.z,1 };
			Matrix<4, 4> mc;
			mc.m[0] = { c.x,c.y,c.z,1 };
			aScreen = Camera::getCurrent().get2D(ma, farclipA, window);
			bScreen = Camera::getCurrent().get2D(mb, farclipB, window);
			cScreen = Camera::getCurrent().get2D(mc, farclipC, window);
		}
		if (/*!nearclipA && !nearclipB && !nearclipC &&*/!farclipA && !farclipB && !farclipC && visible) { // no clip
			visible = true;
		}
		else visible = false;
	}

	// dessiner le triangle dans la frame
	void render(const Window& window, GlobalTexture& globalTexture)  {
		if (!visible) return;
		if (abs(aScreen.x) > 10000 || abs(aScreen.y) > 10000 ||
			abs(bScreen.x) > 10000 || abs(bScreen.y) > 10000 ||
			abs(cScreen.x) > 10000 || abs(cScreen.y) > 10000) {
			//clip here
			return;
		}
		// render
		for (Triangle t : tmp) {
			t.setScreenCoord(window, false);
			t.render(window, globalTexture);
		}tmp.clear(); tmp.shrink_to_fit();
		/*const Vertex sunLight = Camera::getCurrent().getSunLight();
		float light = getPlaneNormal().dot(sunLight);
		light /= 30;
		light = abs(light);*/
		float light = 1;
		if (fill) {
			if (bmp != nullptr) draw(*bmp, globalTexture, light);
			else draw(color, globalTexture, light);
		}
		else if (contour) {
			/*Vertex campos = Camera::getCurrent().getCameraPosition();
			globalTexture.drawLine(globalTexture, a2, a.distance(campos), b2, b.distance(campos), color);
			globalTexture.drawLine(globalTexture, b2, b.distance(campos), c2, c.distance(campos), color);
			globalTexture.drawLine(globalTexture, a2, a.distance(campos), c2, c.distance(campos), color);*/
			globalTexture.drawLine(globalTexture, { aScreen.x,aScreen.y }, 0, { bScreen.x,bScreen.y }, 0, color);
			globalTexture.drawLine(globalTexture, { bScreen.x,bScreen.y }, 0, { cScreen.x,cScreen.y }, 0, color);
			globalTexture.drawLine(globalTexture, { aScreen.x,aScreen.y }, 0, { cScreen.x,cScreen.y }, 0, color);
		}
	}

	// afficher l'intérieur du triangle
	inline void fillIt(const bool& b) { this->fill = b; }

	// afficher les contours du triangle
	inline void contourIt(const bool& b) { this->contour = b; }

private:

	// renvoie 1 si x positif, -1 si x negatif et sinon 0
	inline int sign(const float& x) const { return (0.0 < x) - (x < 0.0); }

	// renvoie x entre min et max
	inline float clamp(const float& x, const float& min, const float& max) const {
		if (x < min) return min;
		else if (x > max) return max;
		else return x;
	}

public:

	// renvoie la distance point triangle
	float distanceToPoint(const Vertex& p) const {
		const Vector v21 = { b.x - a.x,b.y - a.y,b.z - a.z }; const Vector p1 = { p.x - a.x,p.y - a.y,p.z - a.z };
		const Vector v32 = { c.x - b.x,c.y - b.y,c.z - b.z }; const Vector p2 = { p.x - b.x,p.y - b.y,p.z - b.z };
		const Vector v13 = { a.x - c.x,a.y - c.y,a.z - c.z }; const Vector p3 = { p.x - c.x,p.y - c.y,p.z - c.z };
		const Vector nor = v21.cross(v13);
		if (sign(v21.cross(nor).dot(p1)) + sign((v32.cross(nor).dot(p2)) + sign(v13.cross(nor).dot(p3)) < 2)) {
			const Vector vec1 = v21 * clamp(v21.dot(p1) / v21.dot(v21), 0.0, 1.0) - p1;
			const Vector vec2 = v32 * clamp(v32.dot(p2) / v32.dot(v32), 0.0, 1.0) - p2;
			const Vector vec3 = v13 * clamp(v13.dot(p3) / v13.dot(v13), 0.0, 1.0) - p3;
			return sqrtf(std::min(std::min(vec1.dot(vec1), vec2.dot(vec2)), vec3.dot(vec3)));
		}
		else {
			return sqrtf(nor.dot(p1) * nor.dot(p1) / nor.dot(nor));
		}
	}

	// renvoie le point du plan le plus proche du point
	static Vertex ClosestPointOnPlane(const Vertex& pl1, const Vertex& pl2, const Vertex& pl3, const Vertex& p) {
		Vector plNormal = Vector(pl2.x - pl1.x, pl2.y - pl1.y, pl2.z - pl1.z).cross(Vector(pl3.x - pl1.x, pl3.y - pl1.y, pl3.z - pl1.z));
		plNormal.normalize();
		const float distance = plNormal.dot(p.getOriginVector()) - plNormal.dot(pl1.getOriginVector());
		if (distance == 0)return p;
		const Vertex result(p.x - distance * plNormal.x, p.y - distance * plNormal.y, p.z - distance * plNormal.z);
		return result;
	}

	// renvoie true si le point est dans le triangle, false autrement
	bool PointInTriangle(const Vertex& p) const {
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

	// renvoie le point du segment le plus proche du point
	Vertex ClosestPointOnLine(const Vertex& l1, const Vertex& l2, const Vertex& p) const {
		const Vector line = l2 - l1;
		float t = Vector(p - l1).dot(line) / line.dot(line);
		t = clamp(t, 0.0, 1.0);
		return Vertex(l1.x + t * line.x, l1.y + t * line.y, l1.z + t * line.z);
	}

	// renvoie le point du triangle le plus proche du point
	Vertex ClosestPoint(const Vertex& point) const {
		const Vertex p = ClosestPointOnPlane(a, b, c, point);

		if (PointInTriangle(p)) {
			return p;
		}
		const Vertex c1 = ClosestPointOnLine(a, b, p);
		const Vertex c2 = ClosestPointOnLine(b, c, p);
		const Vertex c3 = ClosestPointOnLine(c, a, p);

		const float mag1 = p.distance(c1);
		const float mag2 = p.distance(c2);
		const float mag3 = p.distance(c3);

		const float min = std::min(std::min(mag1, mag2),mag3);

		if (min == mag1) {
			return c1;
		}
		else if (min == mag2) {
			return c2;
		}
		return c3;
	}

	// renvoie le point d'intersection entre un segment et un plan
	static Vertex getIntersectionWithPlane(const Vertex& rayPoint, const Vector& rayVector, const Vector& planeNormal, const Vertex& planePoint) {
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

private:

	// dessiner le triangle dans la frame
	void draw(const Bitmap& src, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(src,
			Point2D(aScreen.x, aScreen.y), Point2D(bScreen.x, bScreen.y), Point2D(cScreen.x, cScreen.y),
			bmpA, bmpB, bmpC,
			aScreen.z, bScreen.z, cScreen.z,
			dst, light);
	}

	// dessiner le triangle dans la frame
	void draw(const Color& color, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(color,
			Point2D(aScreen.x,aScreen.y), Point2D(bScreen.x, bScreen.y), Point2D(cScreen.x, cScreen.y),
			aScreen.z, bScreen.z, cScreen.z,
			dst,light);
	}
};
