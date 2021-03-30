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
	/*=============================================================================================
	 *		Attributs
	 *===========================================================================================*/

	//Coordonnées flotantes x,y,z
	float x, y, z;

	/*=============================================================================================
	 *		Constructeurs
	 *===========================================================================================*/

	// Constructeur par défaut
	Sommet() : x(0), y(0), z(0) {}

	/**
	 * @brief Constructeur à partir d'un Point
	 * 
	 * @param p Référence vers un point 3D
	 */
	Sommet(const Point3D &p) : x(p.x), y(p.y), z(p.z) {}

	/**
	 * @brief Constructeur à partir d'un vecteur de valeurs flottantes
	 * 
	 * @param vec3d Vecteur de valeur flottantes contenant les 3 coordonnées
	 */
	Sommet(const std::array<float, 3> &vec3d) : x(vec3d[0]), y(vec3d[1]), z(vec3d[2]) {}

	/**
	 * @brief Constructeur à partir de 3 valeurs flottantes
	 * 
	 * @param x Coordonnées en x
	 * @param y Coordonnées en y
	 * @param z Coordonnées en z
	 */
	Sommet(const float &x, const float &y, const float &z) : x(x), y(y), z(z) {}

	/**
	 * @brief Constructeur avec template
	 * 
	 * @tparam T1,T2,T3 Le type doit correspondre à une valeur numérique (int, float, double)
	 * 
	 * @param x Coordonnées en x
	 * @param y Coordonnées en y
	 * @param z Coordonnées en z
	 */
	template <class T1, class T2, class T3>
	Sommet(const T1 &x, const T2 &y, const T3 &z) : x((float)x), y((float)y), z((float)z) {}

	/**
	 * @brief Constructeur par copie
	 * 
	 * @param v Sommet qui sera copié
	 */
	Sommet(const Sommet &v) : x(v.x), y(v.y), z(v.z) {}

	/*=============================================================================================
	 *		Surchages d'opérateurs
	 *===========================================================================================*/

	/* ----- Opérateurs s'assignement ----- */

	/**
	 * @brief Le sommet courant prend les coordonnées du sommet en paramètre
	 * 
	 * @param other Référence sur un Sommet 
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator=(const Sommet &other);

	/**
	 * @brief Les coordonnées du sommet courant sont égal à la somme d'eux-même avec les coordonnées du sommet en paramètre
	 * 
	 * @param other Référence sur un Sommet
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator+=(const Sommet &other);

	/**
	 * @brief Les coordonnées du sommet courant sont égal à la différence d'eux-même avec les coordonnées du sommet en paramètre
	 * 
	 * @param other Référence sur un Sommet
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator-=(const Sommet &other);

	/**
	 * @brief Multiplication des coordonnées du sommet courant par une valeur float
	 * 
	 * @param val Valeur par laquelle les coordonnées seront multipliées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator*=(const float &val);

	/**
	 * @brief Multiplication des coordonnées du sommet courant par une valeur int
	 * 
	 * @param val Valeur par laquelle les coordonnées seront multipliées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator*=(const int &val);

	/**
	 * @brief Multiplication des coordonnées du sommet courant par une valeur double
	 * 
	 * @param val Valeur par laquelle les coordonnées seront multipliées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator*=(const double &val);

	/**
	 * @brief Division des coordonnées du sommet courant par une valeur float
	 * 
	 * @param val Valeur par laquelle les coordonnées seront divisées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator/=(const float &val);

	/**
	 * @brief Division des coordonnées du sommet courant par une valeur int
	 * 
	 * @param val Valeur par laquelle les coordonnées seront divisées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator/=(const int &val);

	/**
	 * @brief Division des coordonnées du sommet courant par une valeur double
	 * 
	 * @param val Valeur par laquelle les coordonnées seront divisées
	 * 
	 * @return Sommet& 
	 */
	Sommet &operator/=(const double &val);

	/* ----- Opérateurs d'affectation ----- */

	const Sommet operator*(const float &val) const;
	const Sommet operator*(const double &val) const;
	const Sommet operator*(const int &val) const;
	const Sommet operator/(const float &val) const;
	const Sommet operator/(const double &val) const;
	const Sommet operator/(const int &val) const;

	/**
	 * @brief Operateur [ ] qui renvoie x,y ou z en fonction de la valeur de i
	 * 
	 * @param i unsigned int qui prend la valeur 0 pour x, 1 pour y et 2 pour z
	 * 
	 * @return Retourne la coordonnée x, y ou z en fonction de i
	 */
	float &operator[](const unsigned &i);

	/**
	 * @brief Convertie un Sommet en un Point3D
	 * 
	 * @return Retourne un point 3D 
	 */
	Point3D toPoint3D() const;

	// Le sommet devient à une distance de l'origine dans la même direction
	void normalize();

	/**
	 * @brief Renvoie le produit scalaire
	 * 
	 * @param v Sommet avec lequel sera efféctué le prosuit scalaire
	 * 
	 * @return Retourne le résultat du produit scalaire
	 */
	float dotProduct(const Sommet &v) const;

	/**
	 * @brief Effectue un produit en croix entre deux sommets
	 * 
	 * @param v Sommet avec lequel sera effectué le produit en croix
	 * 
	 * @return Retourne le résultat du produit en croix (Sommet)
	 */
	Sommet crossProduct(const Sommet &v) const;

	/**
	 * @brief Calcul de distance entre 2 sommets
	 * 
	 * @param v Sommet avec lequel la distance sera calculé
	 * 
	 * @return Retourne la distance entre le sommet courant et le sommet en argument
	 */
	float distance(const Sommet &v) const;

	/**
	 * @brief Calcule la distance entre le point actuel et le 2e point formant une ligne
	 * 
	 * @param P0 Premier point formant la ligne
	 * @param P2 Deuxième point formant la ligne
	 * 
	 * @return Distance flottante entre le point actuel et la ligne donnée
	 */
	float distanceToLine(const Sommet &P0, const Sommet &P1) const;

	// Afficher les coordonnées du sommet par stdout
	void print() const;

	// Affiche les coordonnées des sommets en surchageant l'opérateur <<
	friend std::ostream &operator<<(std::ostream &os, const Sommet &v);

	/**
	 * @brief ???
	 * 
	 * @param length ???
	 */
	void normalizeOnLength(const float &length);

	/**
	 * @brief Retourne la distance par rapport à l'origine du repère
	 * 
	 * @return Retourne la distance entre le Sommet et l'origine du repère 
	 */
	float getLength() const;
};