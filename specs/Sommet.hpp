#pragma once

//#include "library.h"
#include "Point.h"
#include <array>

/**
 * @class Représente un Sommet à 3 coordonnées, issu du module de Géométrie 
 */

class Sommet
{
public:
	//Coordonnées flotantes x,y,z
	float x, y, z;

	/**********************************************************************************************
	 * 		Constructeurs
	 * *******************************************************************************************/

	// Constructeur par défaut
	Sommet() : x(0), y(0), z(0) {}

	// Constructeur à partir d'un Point
	Sommet(const Point3D &p) : x(p.x), y(p.y), z(p.z) {}

	// Constructeur à partir d'un vecteur de valeurs flottantes
	Sommet(const std::array<float, 3> &vec3d) : x(vec3d[0]), y(vec3d[1]), z(vec3d[2]) {}

	// Constructeur à partir de 3 valeurs flottantes
	Sommet(const float &x, const float &y, const float &z) : x(x), y(y), z(z) {}

	// Constructeur avec template
	template <class T1, class T2, class T3>
	Sommet(const T1 &x, const T2 &y, const T3 &z) : x((float)x), y((float)y), z((float)z) {}

	//Constructeur par copie
	Sommet(const Sommet &v) : x(v.x), y(v.y), z(v.z) {}

	/**********************************************************************************************
	 * 		Surcharges d'opérateur
	 * *******************************************************************************************/

	// Le sommet courant prend les coordonnées du sommet en paramètre
	Sommet &operator=(const Sommet &other);
	// Les coordonnées du sommet courant sont égal à la somme d'eux-même avec les coordonnées du sommet en paramètre
	Sommet &operator+=(const Sommet &other);
	// Les coordonnées du sommet courant sont égal à la différence entre les coordonnées du sommet courant et ceux du sommet en paramètre
	Sommet &operator-=(const Sommet &other);

	// Multiplication des coordonnées du sommet courant par une valeur float
	Sommet &operator*=(const float &val);
	// Multiplication des coordonnées du sommet courant par une valeur int
	Sommet &operator*=(const int &val);
	// Multiplication des coordonnées du sommet courant par une valeur double
	Sommet &operator*=(const double &val);
	// Division des coordonnées du sommet courant par une valeur float
	Sommet &operator/=(const float &val);
	// Division des coordonnées du sommet courant par une valeur int
	Sommet &operator/=(const int &val);
	// Division des coordonnées du sommet courant par une valeur double
	Sommet &operator/=(const double &val);

	// Multiplie les coordonnées du sommet courant par une valeur float et renvoie le nouveau sommet
	const Sommet operator*(const float &val) const;
	// Multiplie les coordonnées du sommet courant par une valeur double et renvoie le nouveau sommet
	const Sommet operator*(const double &val) const;
	// Multiplie les coordonnées du sommet courant par une valeur int et renvoie le nouveau sommet
	const Sommet operator*(const int &val) const;
	// Divise les coordonnées du sommet courant par une valeur float et renvoie le nouveau sommet
	const Sommet operator/(const float &val) const;
	// Divise les coordonnées du sommet courant par une valeur double et renvoie le nouveau sommet
	const Sommet operator/(const double &val) const;
	// Divise les coordonnées du sommet courant par une valeur int et renvoie le nouveau sommet
	const Sommet operator/(const int &val) const;

	// Opérateur permettant de récupérer la valeur d'une coordonnée
	float &operator[](const unsigned &i);

	/**********************************************************************************************
	 * 		Méthodes de classe
	 * *******************************************************************************************/

	// Convertie un sommet en un point 3D
	Point3D toPoint3D() const;
	//Le sommet devient à une distance de l'origine dans la même direction.
	void normalize()
	{
		const float length = getLength();
		*this /= length;
	}
	// Renvoie le produit scalaire effectué entre le les coordonnées du sommet courant et le sommet en argument
	float dotProduct(const Sommet &v) const;
	// Renvoie un sommet qui est le résultat du produit en croix du sommet courant avec le sommet en argument
	Sommet crossProduct(const Sommet &v) const;
	// Calcul de distance entre le sommet courant et le sommet en argument
	float distance(const Sommet &v) const;

	/**
	 * @brief Calcule la distance le point actuel et 2 points formant une ligne
	 * 
	 * @param Sommet& P0 Premier point formant la ligne
	 * @param Sommet& P1 Deuxième point formant la ligne
	 * 
	 * @return Distance flottante entre le point actuel et la ligne donnée
	 */
	float distanceToLine(const Sommet &P0, const Sommet &P1) const;

	// Afficher les coordonnées du sommet par stdout
	void print(void) const;
	// Affiche les coordonées des sommets en surchageant l'opérateur <<
	friend std::ostream &operator<<(std::ostream &os, const Sommet &v);

	// ???????????????????
	void normalizeOnLength(const float &length)
	{
		const float sum = abs(x) + abs(y) + abs(z);
		if (sum == 0)
		{
			this->x = this->y = this->z = 0;
			return;
		}
		const float normalizedLength = length / sum;
		x *= normalizedLength;
		y *= normalizedLength;
		z *= normalizedLength;
	}

	// Retourne la distance par rapport à l'origine du repère
	float getLength() const;
};