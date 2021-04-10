#pragma once
#include <string>
#include "Shape.hpp"
#include "Camera.hpp"
class ShapeManager
{
public:
	/*=============================================================================================
	 *		Attributs
	 *===========================================================================================*/

	// Un vecteur de pointeurs vers les shapes
	std::vector<std::unique_ptr<Shape>> shapes;

	// Indice vers la shape sujet de la caméra ou bien -1
	std::string subject;

	/*=============================================================================================
	 *		Méthodes
	 *===========================================================================================*/

	/**
	 * @brief Vérifie si une shape avec le nom donnée existe déjà
	 * 
	 * @param name Nom à vérifier
	 */
	bool nameTaken(const std::string& name) const {
		for(int i = 0; i < shapes.size(); i++) if (!name.compare(shapes[i]->name)) return true;
		return false;
	}

	/**
	 * @brief Donne le premier nom unique disponible
	 * 
	 * @param name Nom à vérifier
	 */
	std::string giveUniqueName() const {
		std::string newName;
		return newName;
	}

	/**
	 * @brief Renvoie la shape demandée par reference
	 *
	 * @param name Nom de la shape désirée
	 */
	Shape& getShape(const std::string& name) {
		for (int i = 0; i < shapes.size(); i++) if (!name.compare(shapes[i]->name)) return *shapes[i];
		std::cout << "Error: there is no shape named " << name << std::endl;
		exit(1);
	}

	/**
	 * @brief Met un indice de shape en sujet à la caméra courante
	 * 
	 * @param val indice en paramètre
	 */
	void setSubject(const std::string& name) {
		subject = name;
		if (nameTaken(subject)) Camera::getCurrent().setSubject(getShape(name).center);
		else Camera::getCurrent().hasSubject = false;
	}

	/**
	 * @brief Ajouter une nouvelle forme à partir de triangles, d'un centre et d'une texture
	 * 
	 * @param Nom unique de la shape
	 * @param triangles Vecteur de triangles qui composent la forme
	 * @param center Le sommet centre de la forme
	 * @param bmp La texture de la forme
	 */
	void addShape(const std::string& name, const std::vector<Triangle>& triangles, const Vertex& center, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Shape(name, triangles, center, bmp));
	}

	/**
	 * @brief Ajouter une nouvelle forme à partir d'une forme existante
	 * 
	 * @param shape Shape à copier
	 */
	void addShape(const Shape& shape) {
		if (nameTaken(shape.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Shape(shape));
	}

	/**
	 * @brief Ajoute une nouvelle sphere
	 * 
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param bmp Bitmap
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const std::string& name, const Vertex& center, const double& radius, Bitmap* bmp, const int& precision = 20) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(name, center, radius, bmp, precision));
	}

	/**
	 * @brief Ajoute une nouvelle sphère
	 * 
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param color Indique la couleur de la sphère
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const std::string& name, const Vertex& center, const double& radius, const Color& color, const int& precision = 20) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(name, center, radius, color, precision));
	}

	/**
	 * @brief Ajoute une sphère pré-existante
	 * 
	 * @param sphere Sphère qui sera ajoutée
	 */
	void addSphere(const Sphere& sphere) {
		if (nameTaken(sphere.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(sphere));
	}

	/**
	 * @brief Ajoute un cube
	 * 
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param width indique la largeur du cube (Taille des arètes)
	 * @param bmp Bitmap
	 */
	void addCube(const std::string& name, const Vertex& center, const double& width, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		if (bmp == nullptr) shapes.emplace_back(new Cube(name, center, width));
		else shapes.emplace_back(new Cube(name, center, width, bmp));
	}

	/**
	 * @brief Ajoute un cube pré-existant
	 * 
	 * @param cube Cube qui sera ajouté
	 */
	void addCube(const Cube& cube) {
		if (nameTaken(cube.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Cube(cube));
	}

	/**
	 * @brief Ajoute un rectangle
	 * 
	 * @param Nom unique de la shape
	 * @param a,b,c,d Sommets du rectangle, permettant de le dessiner
	 * @param bmp Bitmap
	 */
	void addRectangle(const std::string& name, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Rectangle(name, a, b, c, d, bmp));
	}

	/**
	 * @brief Ajoute un rectangle pré-existant
	 * 
	 * @param rectangle Rectangle qui sera ajouté
	 */
	void addRectangle(const Rectangle& rectangle) {
		if (nameTaken(rectangle.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Rectangle(rectangle));
	}

	/**
	 * @brief Retire la shape qui porte le nom donné
	 *
	 * @param name Nom de la shape recherchée
	 */
	void removeShape(const std::string& name) {
		for (int i = 0; i < shapes.size(); i++)
			if (!shapes[i]->name.compare(name)) {
				shapes.erase(shapes.begin() + i);
				shapes.shrink_to_fit();
				return;
			}
		std::cout << "Warning : A Shape named " << name << " does not exist" << std::endl;
	}

	/**
	 * @brief ???
	 * 
	 * @param window Fenêtre avec laquelle on interagit
	 */
	void set2ds(const Window& window) {
		for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
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
	bool getFirstInteraction(const Vertex &startingPos, const Vector &movement, Triangle &intersectionZone, Vertex &intersectionPoint, const float &interactionDistance) const {
		const Vertex goalPos = startingPos + movement;
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i]->triangles.size(); j++) {
				/*if (shapes[i]->triangles[j].a.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].b.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].c.distance(startingPos) > interactionDistance + 10)continue;*/
				const Vector triangleNormal(Vector(shapes[i]->triangles[j].b - shapes[i]->triangles[j].a).cross(shapes[i]->triangles[j].c - shapes[i]->triangles[j].a));
				const Vertex planePoint(shapes[i]->triangles[j].a);
				const Vertex tmp(Triangle::getIntersectionWithPlane(startingPos, movement, triangleNormal, planePoint)); // check if there is an intersection between movement-ray and triangle-plane
				if (tmp.getDistanceToOrigin() != 0) {
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