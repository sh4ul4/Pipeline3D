#pragma once

class ShapeManager
{
public:
	/*=============================================================================================
	 *		Attributs
	 *===========================================================================================*/

	// Un vecteur de pointeurs vers les shapes
	std::vector<std::unique_ptr<Shape>> shapes;

	// Indice vers la shape sujet de la caméra ou bien -1
	int subject = -1;

	/*=============================================================================================
	 *		Méthodes
	 *===========================================================================================*/

	/**
	 * @brief Met un indice de shape en sujet à la caméra courante
	 * 
	 * @param val indice en paramètre
	 */
	void setSubject(const int &val);

	/**
	 * @brief Ajouter une nouvelle forme à partir de triangles, d'un centre et d'une texture
	 * 
	 * @param triangles Vecteur de triangles qui composent la forme
	 * @param center Le sommet centre de la forme
	 * @param bmp La texture de la forme
	 */
	void addShape(const std::vector<Triangle> &triangles, const Sommet &center, Bitmap *bmp = nullptr);

	/**
	 * @brief Ajouter une nouvelle forme à partir d'une forme existante
	 * 
	 * @param shape Shape à copier
	 */
	void addShape(const Shape &shape);

	/**
	 * @brief Ajoute une nouvelle sphere
	 * 
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param bmp Bitmap
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const Sommet &center, const double &radius, Bitmap *bmp, const int &precision = 20);

	/**
	 * @brief Ajoute une nouvelle sphère
	 * 
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param color Indique la couleur de la sphère
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const Sommet &center, const double &radius, const Color &color, const int &precision = 20);

	/**
	 * @brief Ajoute une sphère pré-existante
	 * 
	 * @param sphere Sphère qui sera ajoutée
	 */
	void addSphere(const Sphere &sphere);

	/**
	 * @brief Ajoute un cube
	 * 
	 * @param center Sommet représentant le centre de la sphère
	 * @param width indique la largeur du cube (Taille des arètes)
	 * @param bmp Bitmap
	 */
	void addCube(const Sommet &center, const double &width, Bitmap *bmp = nullptr);

	/**
	 * @brief Ajoute un cube pré-existant
	 * 
	 * @param cube Cube qui sera ajouté
	 */
	void addCube(const Cube &cube);

	/**
	 * @brief Ajoute un rectangle
	 * 
	 * @param a,b,c,d Sommets du rectangle, permettant de le dessiner
	 * @param bmp Bitmap
	 */
	void addRectangle(const Sommet &a, const Sommet &b, const Sommet &c, const Sommet &d, Bitmap *bmp = nullptr);

	/**
	 * @brief Ajoute un rectangle pré-existant
	 * 
	 * @param rectangle Rectangle qui sera ajouté
	 */
	void addRectangle(const Rectangle &rectangle);

	/**
	 * @brief ???
	 * 
	 * @param window Fenêtre avec laquelle on interagit
	 */
	void set2ds(const Window &window)
	{
		for (int i = 0; i < static_cast<int>(shapes.size()); i++)
		{
			for (int j = 0; j < static_cast<int>(shapes[i]->triangles.size()); j++)
				shapes[i]->triangles[j].setScreenCoord(window, true);
		}
	}

	/**
	 * @brief ???
	 * 
	 * @param startingPos 
	 * @param movement 
	 * @param intersectionZone 
	 * @param intersectionPoint 
	 * @param interactionDistance 
	 * @return Retourne 'True' si... et 'False' si...
	 */
	bool getFirstInteraction(const Sommet &startingPos, const Sommet &movement, Triangle &intersectionZone, Sommet &intersectionPoint, const float &interactionDistance) const
	{
		const Sommet goalPos = startingPos + movement;
		for (int i = 0; i < shapes.size(); i++)
		{
			for (int j = 0; j < shapes[i]->triangles.size(); j++)
			{
				/*if (shapes[i]->triangles[j].a.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].b.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].c.distance(startingPos) > interactionDistance + 10)continue;*/
				const Sommet triangleNormal(Sommet(shapes[i]->triangles[j].b - shapes[i]->triangles[j].a).crossProduct(shapes[i]->triangles[j].c - shapes[i]->triangles[j].a));
				const Sommet planePoint(shapes[i]->triangles[j].a);
				const Sommet tmp(Triangle::getIntersectionWithPlane(startingPos, movement, triangleNormal, planePoint)); // check if there is an intersection between movement-ray and triangle-plane
				if (tmp.getLength() != 0)
				{
					intersectionPoint = tmp; // copy value (not address)
					if (abs(shapes[i]->triangles[j].distanceToPoint(intersectionPoint)) > interactionDistance)
						continue; // check if point is on triangle
					if (abs(intersectionPoint.distanceToLine(startingPos, goalPos)) > interactionDistance)
						continue; // check if point is on segment
					intersectionZone = shapes[i]->triangles[j];
					return true;
				}
				else
					continue; // no intersection
			}
		}
		return false;
	}
};