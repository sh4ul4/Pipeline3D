#pragma once
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
	// Le nom unique de la shape
	std::string name;

	// Vecteur des triangles qui composent la shape
	std::vector<Triangle> triangles;

	// Point 3D qui défini le centre de la shape
	Vertex center;

	// Booléen qui indique la visibilité
	bool visible = true;

	// Pointeur vers la texture bitmap utilisée
	Bitmap *bmp = nullptr;

	// suppression du constructeur par défaut
	Shape() = delete;

	/**
	 * @brief Constructeur par défaut
	 * @param name Nom unique de la shape
	 */
	Shape(const std::string& name) :name(name) {}

	/**
	 * @brief Constructeur basique
	 * @param name Nom unique de la shape
	 * @param triangles Liste de triangles (vector)
	 * @param center    Centre de la forme 
	 * @param bpm       Pointeur vers la Bitmap de texture si existante 
	 */
	Shape(const std::string& name, const std::vector<Triangle> &triangles, const Vertex &center, Bitmap *bmp = nullptr) : 
            name(name), triangles(triangles), center(center), bmp(bmp) {}

	/**
	 * @brief Constructeur par copie
	 * @param shape objet à copier
	 */
	Shape(const Shape &shape) : 
            name(shape.name), triangles(shape.triangles), center(shape.center), visible(shape.visible), bmp(shape.bmp) {}


	/**
	 * @brief Rotation autour de l'axe x sur la shape courante avec un angle donné.
	 * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
	 * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
	 * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation 
	 */
	void rotateX(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateX(triangles[i].a, angle);
			Maths::rotateX(triangles[i].b, angle);
			Maths::rotateX(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Maths::rotateX(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}

	/**
	 * @brief Rotation autour de l'axe y sur la shape courante avec un angle donné.
	 * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
	 * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
	 * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation
	 */
	void rotateY(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateY(triangles[i].a, angle);
			Maths::rotateY(triangles[i].b, angle);
			Maths::rotateY(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Maths::rotateY(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}

	/**
	 * @brief Rotation autour de l'axe z sur la shape courante avec un angle donné.
	 * Déplacement des points de la shape par rapport au Sommet qui sera le centre de rotation, 
	 * puis rotation selon l'angle (Calcul matriciel) et redéplacement à la position initiale. 
	 * 
	 * @param p Sommet centre de rotation
	 * @param angle Angle de rotation
	 */
	void rotateZ(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateZ(triangles[i].a, angle);
			Maths::rotateZ(triangles[i].b, angle);
			Maths::rotateZ(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Maths::rotateZ(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}

	/**
	 * @brief Déplace le centre ainsi que tous les triangles de la shape vers une nouvelle position 
	 * @param movement Vecteur de déplacement pour le mouvement de l'ensemble des triangles de la Shape
	 */
	void move(const Vector& movement) {
		for (Triangle& t : triangles)t.move(movement);
	}
};


class Sphere : public Shape {

public:

	int radius;

	// constructeur / remplissage par texture bitmap
	Sphere(const std::string& name, const Vertex& center, const double& radius, Bitmap* bmp, const int& precision) :Shape(name, {}, center), radius(radius) {}

	// constructeur / remplissage par couleur unique
	Sphere(const std::string& name, const Vertex& center, const double& radius, const Color& color, const int& precision) :Shape(name, {}, center), radius(radius) {}

	// constructeur par copie
	Sphere(const Sphere& sphere) : Shape(sphere), radius(sphere.radius) {}
	
};

class Cube : public Shape {
public:
	Cube(const std::string& name, const Vertex& center, const double& width, Bitmap* bmp) :Shape(name, {}, center, bmp), width(width) {
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
		triangles.push_back(Triangle(a, b, d, { 0,0,0 }, blue, bmp, Point2D<float>{ 1,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c,b,d, { 0,0,0 }, blue,bmp, Point2D<float>{ 1, 1 }, Point2D<float>{ 1, 0 }, Point2D<float>{ 0, 1 }));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,c, { 0,0,0 }, blue,bmp, Point2D<float>{ 1, 1 }, Point2D<float>{ 1, 0 }, Point2D<float>{ 0, 1 }));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,f, { 0,0,0 }, blue,bmp, Point2D<float>{ 1, 1 }, Point2D<float>{ 1, 0 }, Point2D<float>{ 0, 1 }));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,c, { 0,0,0 }, blue,bmp, Point2D<float>{ 1, 1 }, Point2D<float>{ 1, 0 }, Point2D<float>{ 0, 1 }));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,h, { 0,0,0 }, blue,bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,h, { 0,0,0 }, blue,bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,f, { 0,0,0 }, blue,bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,d, { 0,0,0 }, blue,bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,e, { 0,0,0 }, blue, bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,e, { 0,0,0 }, blue, bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,b, { 0,0,0 }, blue, bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,0 }, Point2D<float>{ 0,1 }));
		triangles.back().color = olive; triangles.back().fillIt(true);
		for (int i = 0; i < static_cast<int>(triangles.size()); i++) {
			triangles[i].fillIt(true);
		}
	}
	Cube(const std::string& name, const Vertex& center, const double& width) :Shape(name, {}, center, bmp), width(width) {
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
		triangles.push_back(Triangle(a, b, d, { 0,0,0 }, blue));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c, b, d, { 0,0,0 }, blue));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, c, { 0,0,0 }, blue));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, f, { 0,0,0 }, blue));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, c, { 0,0,0 }, blue));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, h, { 0,0,0 }, blue));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, h, { 0,0,0 }, blue));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, f, { 0,0,0 }, blue));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, d, { 0,0,0 }, blue));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, e, { 0,0,0 }, blue));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, e, { 0,0,0 }, blue));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, b, { 0,0,0 }, blue));
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
	Vertex hg, bg, hd, bd; 

	Rectangle(const std::string& name, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr): Shape(name) {
		hd = a; bg = b; hd = c; bd = d;
		if (bmp == nullptr) {
			this->center = a;
			this->center += b;
			this->center += c;
			this->center += d;
			this->center.x /= 4;
			this->center.y /= 4;
			this->center.z /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c, { 0,0,0 }));
			triangles.push_back(Triangle(c, b, d, { 0,0,0 }));
			triangles[0].fillIt(0);
			triangles[0].contourIt(1);
			triangles[1].fillIt(0);
			triangles[1].contourIt(1);
		}
		else {
			this->center = a;
			this->center += b;
			this->center += c;
			this->center += d;
			this->center.x /= 4;
			this->center.y /= 4;
			this->center.z /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c, { 0,0,0 }, black, bmp, Point2D<float>{ 0,1 }, Point2D<float>{ 1,1 }, Point2D<float>{ 0,0 }));
			triangles.push_back(Triangle(c, b, d, { 0,0,0 }, black, bmp, Point2D<float>{ 0,0 } , Point2D<float>{ 1,1 }, Point2D<float>{ 1,0 }));
			triangles[0].fillIt(1);
			triangles[0].contourIt(0);
			triangles[1].fillIt(1);
			triangles[1].contourIt(0);
		}
	}
	Rectangle(const Rectangle& rectangle) :Shape(rectangle) {}
};