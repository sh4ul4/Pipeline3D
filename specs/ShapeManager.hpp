

 class ShapeManager {
public:

	/**
	 * Une variable publique.
	 * Un vecteur de pointeurs vers les shapes.
	 */
	std::vector<std::unique_ptr<Shape>> shapes;

	/**
	 * Une variable publique.
	 * Indice vers la shape sujet de la caméra ou bien -1.
	 */
	int subject = -1;

	/** @brief Met un indice de shape en sujet à la caméra courante.
	* @param val indice en paramètre
	*/
	void setSubject(const int& val);

	/** @brief Ajouter une nouvelle forme à partir de triangles, d'un centre et d'une texture.
	* @param triangles Vecteur de triangles qui composent la forme.
	* @param center Le sommet centre de la forme.
	* @param bmp La texture de la forme.
	*/
	void addShape(const std::vector<Triangle>& triangles, const Sommet& center, Bitmap* bmp = nullptr) {
		shapes.emplace_back(new Shape(triangles, center, bmp));
	}

	/** @brief Ajouter une nouvelle forme à partir d'une forme existante.
	* @param shape Shape à copier.
	*/
	void addShape(const Shape& shape) {
		shapes.emplace_back(new Shape(shape));
	}

	/** @brief Ajouter une nouvelle sphere 
	*/
	void addSphere(const Sommet& center, const double& radius, Bitmap* bmp, const int& precision = 20) {
		shapes.emplace_back(new Sphere(center, radius, bmp, precision));
	}

	void addSphere(const Sommet& center, const double& radius, const Color& color, const int& precision = 20) {
		shapes.emplace_back(new Sphere(center, radius, color, precision));
	}

	void addSphere(const Sphere& sphere) {
		shapes.emplace_back(new Sphere(sphere));
	}

	void addCube(const Sommet& center, const double& width, Bitmap* bmp = nullptr) {
		if(bmp == nullptr) shapes.emplace_back(new Cube(center, width));
		else shapes.emplace_back(new Cube(center, width, bmp));
	}

	void addCube(const Cube& cube) {
		shapes.emplace_back(new Cube(cube));
	}

	void addRectangle(const Sommet& a, const Sommet& b, const Sommet& c, const Sommet& d, Bitmap* bmp = nullptr) {
		shapes.emplace_back(new Rectangle(a, b, c, d, bmp));
	}

	void addRectangle(const Rectangle& rectangle) {
		shapes.emplace_back(new Rectangle(rectangle));
	}

	void set2ds(const Window& window) {
		for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
			for (int j = 0; j < static_cast<int>(shapes[i]->triangles.size()); j++)
				shapes[i]->triangles[j].setScreenCoord(window, true);
		}
	}

	bool getFirstInteraction(const Sommet& startingPos, const Sommet& movement, Triangle& intersectionZone, Sommet& intersectionPoint, const float& interactionDistance)const {
		const Sommet goalPos = startingPos + movement;
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i]->triangles.size(); j++) {
				/*if (shapes[i]->triangles[j].a.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].b.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].c.distance(startingPos) > interactionDistance + 10)continue;*/
				const Sommet triangleNormal(Sommet(shapes[i]->triangles[j].b - shapes[i]->triangles[j].a).crossProduct(shapes[i]->triangles[j].c - shapes[i]->triangles[j].a));
				const Sommet planePoint(shapes[i]->triangles[j].a);
				const Sommet tmp(Triangle::getIntersectionWithPlane(startingPos, movement, triangleNormal, planePoint)); // check if there is an intersection between movement-ray and triangle-plane
				if (tmp.getLength() != 0) {
					intersectionPoint = tmp; // copy value (not address)
					if (abs(shapes[i]->triangles[j].distanceToPoint(intersectionPoint)) > interactionDistance)continue; // check if point is on triangle
					if (abs(intersectionPoint.distanceToLine(startingPos, goalPos)) > interactionDistance) continue; // check if point is on segment
					intersectionZone = shapes[i]->triangles[j];
					return true;
				}
				else continue; // no intersection
			}
		}
		return false;
	}
};