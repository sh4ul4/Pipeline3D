/** 
 * @class Shape
 * @brief Contient les informations de base d'une forme.
 * Cette classe est adaptée pour le polymorphisme
 * 
 * @public `std::vector` Vecteur des triangles qui composent la shape
 * @public `Sommet`      Point 3D qui défini le centre de la shape
 * @public `bool`        Booléen qui indique la visibilité
 * @public `Bitmap`      Pointeur vers la texture bitmap utilisée
 */
class Shape {
public:
	// Vecteur des triangles qui composent la shape
	std::vector<Triangle> triangles;

	// Point 3D qui défini le centre de la shape
	Vertex center;

	// Booléen qui indique la visibilité
	bool visible = true;

	// Pointeur vers la texture bitmap utilisée
	Bitmap *bmp = nullptr;

	// Constructeur par défaut modifié
	Shape();

	/**
	 * @brief Constructeur basique
	 * @param triangles Liste de triangles (vector)
	 * @param center    Centre de la forme 
	 * @param bpm       Pointeur vers la Bitmap de texture si existante 
	 */
	Shape(const std::vector<Triangle> &triangles, const Sommet &center, Bitmap *bmp = nullptr) : 
            triangles(triangles), center(center), bmp(bmp) {}

	/**
	 * @brief Constructeur par copie
	 * @param shape objet à copier
	 */
	Shape(const Shape &shape) : 
            triangles(shape.triangles), center(shape.center), visible(shape.visible), bmp(shape.bmp) {}


	/**
	 * @brief Rotation autour de l'axe x sur la shape courante avec un angle donné.
     * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
     * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
     * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation 
	 */
	void rotateX(const Sommet &p, const float &angle);

	/**
	 * @brief Rotation autour de l'axe y sur la shape courante avec un angle donné.
	 * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
     * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
	 * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation
	 */
	void rotateY(const Sommet &p, const float &angle);

	/**
	 * @brief Rotation autour de l'axe z sur la shape courante avec un angle donné.
	 * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
     * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
	 * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation
	 */
	void rotateZ(const Sommet &p, const float &angle);

	/**
	 * @brief Déplace le centre ainsi que tous les triangles de la shape vers une nouvelle position 
	 * @param movement Vecteur de déplacement pour le mouvement de l'ensemble des triangles de la Shape
	 */
	void move(const Sommet &movement);
};

/*
class Sphere : public Shape {

public:

	int radius;

	// constructeur / remplissage par texture bitmap
	Sphere(const Vertex& center, const double& radius, Bitmap* bmp, const int& precision) :Shape({}, center), radius(radius) {}

	// constructeur / remplissage par couleur unique
	Sphere(const Vertex& center, const double& radius, const Color& color, const int& precision) :Shape({}, center), radius(radius) {}

	// constructeur par copie
	Sphere(const Sphere& sphere) : Shape(sphere), radius(sphere.radius) {}
	
};

class Cube : public Shape {
public:
	Cube(const Vertex& center, const double& width, Bitmap* bmp) :Shape({}, center, bmp), width(width) {
		const int half_width = static_cast<int>(width) / 2;
		// set 8 points
		const Vertex a(center.x + half_width, center.y - half_width, center.z - half_width);
		const Vertex b(center.x + half_width, center.y + half_width, center.z - half_width);
		const Vertex c(center.x + half_width, center.y + half_width, center.z + half_width);
		const Vertex d(center.x + half_width, center.y - half_width, center.z + half_width);
		const Vertex e(center.x - half_width, center.y - half_width, center.z - half_width);
		const Vertex f(center.x - half_width, center.y + half_width, center.z - half_width);
		const Vertex g(center.x - half_width, center.y + half_width, center.z + half_width);
		const Vertex h(center.x - half_width, center.y - half_width, center.z + half_width);
		// set 12 triangles
		triangles.push_back(Triangle(a, b, d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c,b,d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,c, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,f, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,c, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,h, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,h, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,f, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,e, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,e, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,b, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = olive; triangles.back().fillIt(true);
		for (int i = 0; i < static_cast<int>(triangles.size()); i++) {
			triangles[i].fillIt(true);
		}
	}
	Cube(const Vertex& center, const double& width) :Shape({}, center, bmp), width(width) {
		const int half_width = static_cast<int>(width) / 2;
		// set 8 points
		const Vertex a(center.x + half_width, center.y - half_width, center.z - half_width);
		const Vertex b(center.x + half_width, center.y + half_width, center.z - half_width);
		const Vertex c(center.x + half_width, center.y + half_width, center.z + half_width);
		const Vertex d(center.x + half_width, center.y - half_width, center.z + half_width);
		const Vertex e(center.x - half_width, center.y - half_width, center.z - half_width);
		const Vertex f(center.x - half_width, center.y + half_width, center.z - half_width);
		const Vertex g(center.x - half_width, center.y + half_width, center.z + half_width);
		const Vertex h(center.x - half_width, center.y - half_width, center.z + half_width);
		// set 12 triangles
		triangles.push_back(Triangle(a, b, d, blue));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c, b, d, blue));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, c, blue));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, f, blue));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, c, blue));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, h, blue));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, h, blue));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, f, blue));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, d, blue));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, e, blue));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, e, blue));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, b, blue));
		triangles.back().color = olive; triangles.back().fillIt(true);
		for (int i = 0; i < static_cast<int>(triangles.size()); i++) {
			triangles[i].fillIt(true);
		}
	}
	Cube(const Cube& cube) :Shape(cube), width(cube.width) {}
	int width;
};

class Rectangle : public Shape {
public:
	Rectangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr) {
		if (bmp == nullptr) {
			this->center = a + b + c + d;
			this->center /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c));
			triangles.push_back(Triangle(c, b, d));
			triangles[0].fillIt(0);
			triangles[0].contourIt(1);
			triangles[1].fillIt(0);
			triangles[1].contourIt(1);
		}
		else {
			this->center = a + b + c + d;
			this->center /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c, black, bmp, { 0,bmp->surface->h }, { bmp->surface->w,bmp->surface->h }, { 0,0 }));
			triangles.push_back(Triangle(c, b, d, black, bmp, { 0,0 } ,{ bmp->surface->w,bmp->surface->h }, { bmp->surface->w,0 }));
			triangles[0].fillIt(1);
			triangles[0].contourIt(0);
			triangles[1].fillIt(1);
			triangles[1].contourIt(0);
		}
	}
	Rectangle(const Rectangle& rectangle) :Shape(rectangle) {}
};*/