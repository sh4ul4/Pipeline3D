#pragma once
//#include "library.h"
#include <iostream>
#include <math.h>
#include "ErrorsHandler.h"

/**
 * @file Issu du module de Géométrie, sous-module (?) 
 * Regroupe les classes Point2D et Point 3D
 */

/*===========================================================================================
	Point2D
===========================================================================================*/

/**
 * @class Point à deux coordonnées entières
 */
class Point2D
{
public:
	// Coordonnées entières x et y du Point2D
	int x;
	int y;

	// Constructeurs: par défaut et normal
	Point2D() : x(0), y(0) {}
	Point2D(const int &x, const int &y) : x(x), y(y);

	// Constructeur par template en castant en entier
	template <class T1, class T2>
	Point2D(const T1 &x, const T2 &y) : x((int)x), y((int)y);

	// Constructeur par copie d'un Point2D
	Point2D(const Point2D &p) : x(p.x), y(p.y) {}

	// Surcharges d'opérateurs
	// Const à la fin: ne modifie pas la classe qu'il appelle (renvoie un nouveau point)
	// const: signifie qu'il renvoie un nouveau point sans modifier la classe qu'il appelle
	Point2D operator+(const Point2D &point) const; // Additionne les coordonnées du point en paramètre avec le point courant
	Point2D operator-(const Point2D &point) const; // soustrait les coordonnées du point en paramètre avec le point courant

	// Surcharges avec classe template
	template <class T>
	Point2D operator*(const T &val) const; // Multiplie les coordonnées du point courant par un float, int, ou double

	template <class T>
	Point2D operator/(const T &val) const; // divise les coordonnées du point courant par un float, int, ou double

	template <class T>
	Point2D operator+(const T &val) const; // Additionne les coordonnées du point courant par un float, int, ou double

	template <class T>
	Point2D operator-(const T &val) const; // Soustrait les coordonnées du point courant par un float, int, ou double

	// Opérateur [], renvoie x ou y si i = 0 ou 1
	// Operateur[] qui renvoie x ou y en fonction de si i est égal à 0 ou 1.
	// Renvoie une erreur si i est différent de 0 ou 1.
	const int &operator[](const int &i);

	// Test d'égalité entre deux points
	bool operator==(const Point2D &p) const;

	// Formate le print pour un Point2D, affiche ses coordonnées (x,y)
	void print() const;

	// Rédredéfinition de l'opérateur <<, servant à l'affichage sur la sortie standard
	friend std::ostream &operator<<(std::ostream &os, const Point2D &p);

	/** @brief Calcule la distance entre 2 points passés en paramètres
	* @param Point2D a
	* @param Point2D b
	* @return Distance entière calculée entre a et b
	*/
	static int distance(const Point2D &a, const Point2D &b);

	/** @brief Calcule la distance entre un point et le point actuel
	* @param Point2D p
	* @return Distance entière calculée entre this et p
	*/
	int distance(const Point2D &p) const;
};

/*===========================================================================================
	Point3D
===========================================================================================*/

/**
 * @class Point3D à trois coordonnées entières
 */
class Point3D
{
public:
	// Coordonnées entières x, y et z du Point3D
	int x;
	int y;
	int z;

	// Constructeurs: par défaut et normal via Template
	Point3D(void) : x(0), y(0), z(0) {}
	template <class T1, class T2, class T3>
	Point3D(const T1 &x, const T2 &y, const T3 &z) : x((int)x), y((int)y), z((int)z) {}
	Point3D(const Point3D &p) : x(p.x), y(p.y), z(p.z) {}

	// Surcharge des opérateurs (Opérations basiques)
	Point3D &operator+(const Point3D &point);
	Point3D &operator-(const Point3D &point);
	Point3D &operator*(const Point3D &point);
	Point3D &operator/(const Point3D &point);

	// Multiplication et division par des cordonnées par une valeur 'val'
	template <class T>
	Point3D &operator*(const T &val);
	template <class T>
	Point3D &operator/(const T &val);

	//
	int &operator[](const int &i);

	// Test l'égalité entre deux point 3D
	inline bool operator==(const Point3D &p) const;

	//
	void print() const;

	// Redéfinition de l'opérateur <<
	friend std::ostream &operator<<(std::ostream &os, const Point3D &p);
};