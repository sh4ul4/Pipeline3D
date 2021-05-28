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

	void setNormalVector() {
		Vector v1 = a - c;
		Vector v2 = b - c;
		normalVec = v1.cross(v2);
		normalVec.normalize();
	}

	// set screen coordinates and handle clipping
	void setScreenCoord(const Window& window, const Point2D<int>& center, Camera& camera) {
		visible = true;
		// far-clipping
		distanceToCamera = a.distance(camera.getCameraPosition()) / 3 +
			b.distance(camera.getCameraPosition()) / 3 +
			c.distance(camera.getCameraPosition()) / 3;
		visible = visible && distanceToCamera < camera.far;
		if (!visible)return;
		// near-clipping
		nearclipA = nearclipB = nearclipC = false;
		const float rela = camera.relationToClipPlane(a);
		const float relb = camera.relationToClipPlane(b);
		const float relc = camera.relationToClipPlane(c);
		nearclipA = rela <= 0;
		nearclipB = relb <= 0;
		nearclipC = relc <= 0;
		visible = visible && !nearclipA && !nearclipB && !nearclipC;
		if (!visible)return;
		// 2D points calculation
		Matrix<4, 4> ma;
		ma.m[0] = { a.x,a.y,a.z,1 };
		Matrix<4, 4> mb;
		mb.m[0] = { b.x,b.y,b.z,1 };
		Matrix<4, 4> mc;
		mc.m[0] = { c.x,c.y,c.z,1 };
		aScreen = camera.get2D(ma, farclipA, center);
		bScreen = camera.get2D(mb, farclipB, center);
		cScreen = camera.get2D(mc, farclipC, center);
		visible = visible && !farclipA && !farclipB && !farclipC;
	}

	// dessiner le triangle dans la frame
	void render(const Window& window, GlobalTexture& globalTexture, const Point2D<int>& center, bool showDepth = false, bool showMesh = false)  {
		if (!visible) return;
		if (abs(aScreen.x) > 10000 || abs(aScreen.y) > 10000 ||
			abs(bScreen.x) > 10000 || abs(bScreen.y) > 10000 ||
			abs(cScreen.x) > 10000 || abs(cScreen.y) > 10000) {
			//clip here
			return;
		}
		Camera::getCurrent().lightSource.normalize();
		float light = normalVec.dot(Camera::getCurrent().lightSource);
		light *= Camera::getCurrent().lightIntensity;
		light = Maths::clamp(light, 0.0f, 1.0f);
		if (showDepth && bmp != nullptr) {
			drawDepth(*bmp, globalTexture, light, Camera::getCurrent().lightColor);
		}
		else if (fill && !showMesh) {
			if (bmp != nullptr) draw(*bmp, globalTexture, light, Camera::getCurrent().lightColor);
			else draw(color, globalTexture, light, Camera::getCurrent().lightColor);
		}
		else if (contour || showMesh) {
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, aScreen.z, Point2D<int>{ bScreen.x,bScreen.y }, bScreen.z, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ bScreen.x,bScreen.y }, bScreen.z, Point2D<int>{ cScreen.x,cScreen.y }, cScreen.z, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, aScreen.z, Point2D<int>{ cScreen.x,cScreen.y }, cScreen.z, color);
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
	void draw(const Bitmap& src, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterize(src,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			Point2D<int>(bmp->surface->w * bmpA.x, std::abs(bmp->surface->h * bmpA.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpB.x, std::abs(bmp->surface->h * bmpB.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpC.x, std::abs(bmp->surface->h * bmpC.y - bmp->surface->h)),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}

	// dessiner le triangle dans la frame
	void draw(const Color& color, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterize(color,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}

	void drawDepth(const Bitmap& src, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterizeDepth(src,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			Point2D<int>(bmp->surface->w * bmpA.x, std::abs(bmp->surface->h * bmpA.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpB.x, std::abs(bmp->surface->h * bmpB.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpC.x, std::abs(bmp->surface->h * bmpC.y - bmp->surface->h)),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}
};
