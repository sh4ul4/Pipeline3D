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
	Point2 bmpA, bmpB, bmpC;

	// couleur du triangle
	Color color = black;

	// distance du triangle à la caméra
	float distanceToCamera = INFINITY;

	// vecteur de triangles temporaires pour le clipping
	std::vector<Triangle> tmp;

private:

	// booléen pour indiquer la visibilité du triangle
	bool visible = false;

	// booléen pour indiquer si le triangle est rempli
	bool fill = true;

	// booléen pour indiquer si le triangle est contourné
	bool contour = true;

	// variables propres au clipping
	bool farclipA = false;
	bool farclipB = false;
	bool farclipC = false;
	bool nearclipA = false;
	bool nearclipB = false;
	bool nearclipC = false;
	
public:

	// constructeur sans texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color = black, const bool& fill = true)
		: a(a), b(b), c(c), color(color), fill(fill) {}

	// constructeur avec texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color,
		Bitmap* bmp, const Point2& bmpA, const Point2& bmpB, const Point2& bmpC)
		: a(a), b(b), c(c), color(color),
		bmp(bmp), bmpA(bmpA), bmpB(bmpB), bmpC(bmpC) {}
	
	// comparaison de deux triangles en fonction de leurs distance à la caméra
	inline bool operator<(const Triangle& node) const {
		return (this->distanceToCamera < node.distanceToCamera);
	}
	inline bool operator>(const Triangle& node) const {
		return (this->distanceToCamera > node.distanceToCamera);
	}

	// bouger tout le triangle sur un vecteur
	inline void move(const Vertex& movement) {
		a += movement;
		b += movement;
		c += movement;
	}

	// set screen coordinates and handle clipping
	void setScreenCoord(const Window& window, const bool& clip) {
		/*- mettre à jour distance à la caméra
		* - calculer les relations aux plans de clipping
		* - vérifier respect du clipping proche et distant
		* - appliquer clipping complet avec multisampling du triangle
		* - calculer coordonnées en clipped screen space
		*/
	}

	// dessiner le triangle dans la frame
	void render(const Window& window, GlobalTexture& globalTexture)  {}

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
	float distanceToPoint(const Vertex& p) const {}

	// renvoie le point du plan le plus proche du point
	static Vertex ClosestPointOnPlane(const Vertex& pl1, const Vertex& pl2, const Vertex& pl3, const Vertex& p) {}

	// renvoie true si le point est dans le triangle, false autrement
	bool PointInTriangle(const Vertex& p) const {}

	// renvoie le point du segment le plus proche du point
	Point3 ClosestPointOnLine(const Vertex& l1, const Vertex& l2, const Vertex& p) const {}

	// renvoie le point du triangle le plus proche du point
	Vertex ClosestPoint(const Vertex& p) const {}

	// renvoie le point d'intersection entre un segment et un plan
	static Vertex getIntersectionWithPlane(const Vertex& rayPoint, const Vertex& rayVector, const Vertex& planeNormal, const Vertex& planePoint) {}

private:

	// dessiner le triangle dans la frame
	void draw(const Bitmap& src, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(src,
			Point2(aScreen.x, aScreen.y), Point2(bScreen.x, bScreen.y), Point2(cScreen.x, cScreen.y),
			bmpA, bmpB, bmpC,
			aScreen.z, bScreen.z, cScreen.z,
			dst, light);
	}

	// dessiner le triangle dans la frame
	void draw(const Color& color, GlobalTexture& dst, const float& light) const {
		TextureManager::rasterize(color,
			Point2(aScreen.x,aScreen.y), Point2(bScreen.x, bScreen.y), Point2(cScreen.x, cScreen.y),
			aScreen.z, bScreen.z, cScreen.z,
			dst,light);
	}
};