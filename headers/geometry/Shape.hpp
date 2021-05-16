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
	void getBoundingBox(float& minx, float& miny, float& minz, float& maxx, float& maxy, float& maxz) {
		minx = miny = minz = 9999999;
		maxx = maxy = maxz = -9999999;
		for (const Triangle& t : triangles) {
			minx = std::min(std::min(std::min(minx, t.a.x), t.b.x), t.c.x);
			miny = std::min(std::min(std::min(miny, t.a.y), t.b.y), t.c.y);
			minz = std::min(std::min(std::min(minz, t.a.z), t.b.z), t.c.z);
			maxx = std::max(std::max(std::max(maxx, t.a.x), t.b.x), t.c.x);
			maxy = std::max(std::max(std::max(maxy, t.a.y), t.b.y), t.c.y);
			maxz = std::max(std::max(std::max(maxz, t.a.z), t.b.z), t.c.z);
		}
	}

	void getBoundingBox2D(float& minx, float& miny, float& maxx, float& maxy) {
		minx = miny = 9999999;
		maxx = maxy = -9999999;
		for (const Triangle& t : triangles) {
			if (!t.visible) continue;
			minx = std::min(std::min(std::min(minx, t.aScreen.x), t.bScreen.x), t.cScreen.x);
			miny = std::min(std::min(std::min(miny, t.aScreen.y), t.bScreen.y), t.cScreen.y);
			maxx = std::max(std::max(std::max(maxx, t.aScreen.x), t.bScreen.x), t.cScreen.x);
			maxy = std::max(std::max(std::max(maxy, t.aScreen.y), t.bScreen.y), t.cScreen.y);
		}
	}
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
		for (size_t i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateX(triangles[i].a, angle);
			Maths::rotateX(triangles[i].b, angle);
			Maths::rotateX(triangles[i].c, angle);
			Maths::rotateX(triangles[i].normalVec, angle);
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
		for (size_t i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateY(triangles[i].a, angle);
			Maths::rotateY(triangles[i].b, angle);
			Maths::rotateY(triangles[i].c, angle);
			Maths::rotateY(triangles[i].normalVec, angle);
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
		for (size_t i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Maths::rotateZ(triangles[i].a, angle);
			Maths::rotateZ(triangles[i].b, angle);
			Maths::rotateZ(triangles[i].c, angle);
			Maths::rotateZ(triangles[i].normalVec, angle);
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
		center += movement;
	}

	void rescale(float scale) {
		for (size_t i = 0; i < triangles.size(); i++) {
			triangles[i].a -= center;
			triangles[i].b -= center;
			triangles[i].c -= center;
			triangles[i].a.x *= scale;
			triangles[i].a.y *= scale;
			triangles[i].a.z *= scale;
			triangles[i].b.x *= scale;
			triangles[i].b.y *= scale;
			triangles[i].b.z *= scale;
			triangles[i].c.x *= scale;
			triangles[i].c.y *= scale;
			triangles[i].c.z *= scale;
			triangles[i].a += center;
			triangles[i].b += center;
			triangles[i].c += center;
		}
	}

	bool hit2D(const Point2D<int> mouse, const Point2D<int> origin, const float& width, const float& height) {
		if (!visible || mouse.x < origin.x || mouse.x > origin.x + width || mouse.y < origin.y || mouse.y > origin.y + height)return false;
		float aminx, aminy;
		float amaxx, amaxy;
		getBoundingBox2D(aminx, aminy, amaxx, amaxy);
		aminx += origin.x;
		amaxx += origin.x;
		aminy += origin.y;
		amaxy += origin.y;
		return mouse.x <= amaxx && mouse.x >= aminx && mouse.y <= amaxy && mouse.y >= aminy;
	}

	void groundZero() {
		float aminx, aminy, aminz;
		float amaxx, amaxy, amaxz;
		getBoundingBox(aminx, aminy, aminz, amaxx, amaxy, amaxz);
		if(aminx < 0) move(Vector(0,-aminx,0));
	}

	bool hit(Shape& other) {
		float aminx, aminy, aminz;
		float amaxx, amaxy, amaxz;
		float bminx, bminy, bminz;
		float bmaxx, bmaxy, bmaxz;
		getBoundingBox(aminx, aminy, aminz, amaxx, amaxy, amaxz);
		other.getBoundingBox(bminx, bminy, bminz, bmaxx, bmaxy, bmaxz);
		return (aminx <= bmaxx && amaxx >= bminx) &&
			(aminy <= bmaxy && amaxy >= bminy) &&
			(aminz <= bmaxz && amaxz >= bminz);
	}
};




class Sphere : public Shape {

public:

	int radius;

	// constructeur / remplissage par texture bitmap
	Sphere(const std::string& name, const Vertex& center, const double& radius, Bitmap* bmp, const int& precision) :Shape(name, {}, center), radius(radius) {}

	// constructeur / remplissage par couleur unique
	Sphere(const std::string& name, const Vertex& center, const double& radius, const Color& color, const int& precision) :Shape(name, {}, center), radius(radius) {
		//int precision = precision;
		const float deltaTheta = M_PI / precision;
		const float deltaPhi = 2 * M_PI / precision;
		float theta = 0;
		float phi = 0;
		std::vector<Vertex> ps;
		ps.push_back(Vertex(0, 0, static_cast<float>(radius))); //north pole end cap
		for (int ring = 0; ring < precision; ring++) { //move to a new z - offset
			theta += deltaTheta;
			for (int point = 0; point < precision; point++) { // draw a ring
				phi += deltaPhi;
				float x = sin(theta) * cos(phi) * radius;
				float y = sin(theta) * sin(phi) * radius;
				float z = cos(theta) * radius;
				ps.push_back({ x, y, z });
			}
		}
		ps.push_back({ 0, 0, static_cast<float>(-radius) });
		for (size_t i = 2; i < precision && i < ps.size(); i += 2) {
			//triangles.push_back({ ps[static_cast<unsigned>(i) - 2],ps[static_cast<unsigned>(i) - 1],ps[0] });
			triangles.push_back(Triangle(ps[i - 2], ps[i - 1], ps[0], Vector(0, 0, 0), color, false));
			//triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = precision; i < ps.size(); i++) {
			triangles.push_back(Triangle(ps[i], ps[(int)(i - 1)], ps[(int)(i - precision)], Vector(0,0,0), color, false));
			//triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = 0; i < ps.size() && i < triangles.size(); i++) {
			triangles[i].a.x += center.x;
			triangles[i].b.x += center.x;
			triangles[i].c.x += center.x;

			triangles[i].a.y += center.y;
			triangles[i].b.y += center.y;
			triangles[i].c.y += center.y;

			triangles[i].a.z += center.z;
			triangles[i].b.z += center.z;
			triangles[i].c.z += center.z;
		}
	}

	// constructeur par copie
	Sphere(const Sphere& sphere) : Shape(sphere), radius(sphere.radius) {}
	
};

class Cube : public Shape {
public:

	//créer un cube avec bitmap
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
	
	// créer un cube sans bitmap
	Cube(const std::string& name, const Vertex& center, const double& width, const Color& color) :Shape(name, {}, center, nullptr), width(width) {
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
		triangles.push_back(Triangle(a, b, d, { 0,0,0 }, color));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c, b, d, { 0,0,0 }, color));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, c, { 0,0,0 }, color));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b, g, f, { 0,0,0 }, color));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, c, { 0,0,0 }, color));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d, g, h, { 0,0,0 }, color));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, h, { 0,0,0 }, color));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e, g, f, { 0,0,0 }, color));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, d, { 0,0,0 }, color));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, h, e, { 0,0,0 }, color));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, e, { 0,0,0 }, color));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a, f, b, { 0,0,0 }, color));
		triangles.back().color = olive; triangles.back().fillIt(true);
		// set 6 faces


		for (int i = 0; i < static_cast<int>(triangles.size()); i++) {
			triangles[i].fillIt(true);
		}
	}
	
	// créer un cube prédéfini
	Cube(const Cube& cube) :Shape(cube), width(cube.width) {}
	int width;
};

class Rectangle : public Shape {
	float max(float a, float b, float c, float d) {
		return std::max(a, std::max(b, std::max(c, d)));
	}
	float min(float a, float b, float c, float d) {
		return std::min(a, std::min(b, std::min(c, d)));
	}
public:
	Vertex hg, bg, hd, bd; 

	Rectangle(const std::string& name, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, int division, const Color& color, const bool& fill = true, Bitmap* bmp = nullptr) : Shape(name) {
		hg = a; bg = b; hd = c; bd = d;
		this->center = a;
		this->center += b;
		this->center += c;
		this->center += d;
		this->center.x /= 4;
		this->center.y /= 4;
		this->center.z /= 4;
		division += 1;
		int subdiv = pow(division, 2);
		for (int i = 0; i < subdiv; i++) {
			int x = i % division;
			int y = i / division;
			Vertex tmpHG(hg.x + (x / (float)division * (hd.x - hg.x)) + (y / (float)division * (bg.x - hg.x)),
				hg.y + (x / (float)division * (hd.y - hg.y)) + (y / (float)division * (bg.y - hg.y)),
				hg.z + (x / (float)division * (hd.z - hg.z)) + (y / (float)division * (bg.z - hg.z)));

			Vertex tmpBG(hg.x + ((x + 1) / (float)division * (hd.x - hg.x)) + (y / (float)division * (bg.x - hg.x)),
				hg.y + ((x + 1) / (float)division * (hd.y - hg.y)) + (y / (float)division * (bg.y - hg.y)),
				hg.z + ((x + 1) / (float)division * (hd.z - hg.z)) + (y / (float)division * (bg.z - hg.z)));

			Vertex tmpHD(hg.x + (x / (float)division * (hd.x - hg.x)) + ((y + 1) / (float)division * (bg.x - hg.x)),
				hg.y + (x / (float)division * (hd.y - hg.y)) + ((y + 1) / (float)division * (bg.y - hg.y)),
				hg.z + (x / (float)division * (hd.z - hg.z)) + ((y + 1) / (float)division * (bg.z - hg.z)));

			Vertex tmpBD(hg.x + ((x + 1) / (float)division * (hd.x - hg.x)) + ((y + 1) / (float)division * (bg.x - hg.x)),
				hg.y + ((x + 1) / (float)division * (hd.y - hg.y)) + ((y + 1) / (float)division * (bg.y - hg.y)),
				hg.z + ((x + 1) / (float)division * (hd.z - hg.z)) + ((y + 1) / (float)division * (bg.z - hg.z)));
			if (bmp == nullptr) {
				triangles.push_back(Triangle(tmpHG, tmpBG, tmpHD, { 0,0,0 }, color, fill));
				triangles.push_back(Triangle(tmpBG, tmpHD, tmpBD, { 0,0,0 }, color, fill));
			}
			else {
				triangles.push_back(Triangle(tmpHG, tmpBG, tmpHD,
					{ 0,0,0 }, black, bmp,
					Point2D<float>(x / (float)division, y / (float)division),
					Point2D<float>((x + 1) / (float)division, y / (float)division),
					Point2D<float>(x / (float)division, (y + 1) / (float)division)));
				triangles.push_back(Triangle(tmpBG, tmpHD, tmpBD,
					{ 0,0,0 }, black, bmp,
					Point2D<float>((x + 1) / (float)division, y / (float)division),
					Point2D<float>(x / (float)division, (y + 1) / (float)division),
					Point2D<float>((x + 1) / (float)division, (y + 1) / (float)division)));
			}
		}
	}
	Rectangle(const Rectangle& rectangle) :Shape(rectangle) {}
};