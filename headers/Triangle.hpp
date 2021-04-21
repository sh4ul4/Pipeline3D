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
	Point2D<float> bmpA, bmpB, bmpC;

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
		const Color& color, Bitmap* bmp, const Point2D<float>& bmpA, const Point2D<float>& bmpB, const Point2D<float>& bmpC)
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
	void setScreenCoord(const Window& window, const bool& clip, const Point2D<int>& center) {
		visible = false;
		distanceToCamera = Maths::distancePointTriangle(Camera::getCurrent().getCameraPosition(), a, b, c);
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
			aScreen = Camera::getCurrent().get2D(ma, farclipA, center);
			bScreen = Camera::getCurrent().get2D(mb, farclipB, center);
			cScreen = Camera::getCurrent().get2D(mc, farclipC, center);
		}
		if (/*!nearclipA && !nearclipB && !nearclipC &&*/!farclipA && !farclipB && !farclipC && visible) { // no clip
			visible = true;
		}
		else visible = false;
	}

	// dessiner le triangle dans la frame
	void render(const Window& window, GlobalTexture& globalTexture, const Point2D<int>& center)  {
		if (!visible) return;
		if (abs(aScreen.x) > 10000 || abs(aScreen.y) > 10000 ||
			abs(bScreen.x) > 10000 || abs(bScreen.y) > 10000 ||
			abs(cScreen.x) > 10000 || abs(cScreen.y) > 10000) {
			//clip here
			return;
		}
		for (Triangle t : tmp) {
			t.setScreenCoord(window, false, center);
			t.render(window, globalTexture, center);
		}tmp.clear(); tmp.shrink_to_fit();
		float light = 1;// normalVec.dot(Vector(0, 10, 10));
		//light *= 10;
		if (fill) {
			if (bmp != nullptr) draw(*bmp, globalTexture, light);
			else draw(color, globalTexture, light);
		}
		else if (contour) {
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, 0, Point2D<int>{ bScreen.x,bScreen.y }, 0, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ bScreen.x,bScreen.y }, 0, Point2D<int>{ cScreen.x,cScreen.y }, 0, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, 0, Point2D<int>{ cScreen.x,cScreen.y }, 0, color);
		}
	}

	// afficher l'intérieur du triangle
	inline void fillIt(const bool& b) { this->fill = b; }

	// afficher les contours du triangle
	inline void contourIt(const bool& b) { this->contour = b; }

public:

	// renvoie le point du triangle le plus proche du point
	Vertex ClosestPoint(const Vertex& point) const {
		return Maths::ClosestPointOnTriangle(point, a, b, c);
	}

private:

	// dessiner le triangle dans la frame
	void draw(const Bitmap& src, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(src,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			Point2D<int>(bmp->surface->w * bmpA.x, std::abs(bmp->surface->h * bmpA.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpB.x, std::abs(bmp->surface->h * bmpB.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpC.x, std::abs(bmp->surface->h * bmpC.y - bmp->surface->h)),
			aScreen.z, bScreen.z, cScreen.z,
			dst, light);
	}

	// dessiner le triangle dans la frame
	void draw(const Color& color, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(color,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			aScreen.z, bScreen.z, cScreen.z,
			dst,light);
	}
};
