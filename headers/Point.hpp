#pragma once
#include <iostream>
/**
 * @file Issu du module de G�om�trie, sous-module (?)
 * Regroupe les classes Point2D et Point 3D
 */

 /*===========================================================================================
	 Point2D
 ===========================================================================================*/

 /**
  * @class Point � deux coordonn�es enti�res
  */
class Point2D
{
public:
	/**
	 * Une variable publique.
	 * Coordonn�e x du point.
	 */
	int x;

	/**
	 * Une variable publique.
	 * Coordonn�e y du point.
	 */
	int y;

	/**
	 * Un constructeur.
	 * Met les coordonn�es � z�ro.
	 */
	Point2D() : x(0), y(0) {}

	/**
	 * Un constructeur.
	 * Assigne les coordonn�es.
	 */
	Point2D(const int& x, const int& y) : x(x), y(y) {}

	/**
	 * Un constructeur.
	 * Prend des templates en entr�e et cast vers int.
	 */
	template <class T1, class T2>
	Point2D(const T1& x, const T2& y) : x((int)x), y((int)y) {}

	/**
	 * Un constructeur.
	 * @param p Point copi�.
	 * Copie les coordonn�es du point.
	 */
	Point2D(const Point2D& p) : x(p.x), y(p.y) {}

	/**
	 * Un op�rateur public.
	 * @param p Deuxi�me op�rande.
	 * Additionne les coordonn�es du point en param�tre avec le point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	Point2D operator+(const Point2D& p) const { return { x + p.x, y + p.y }; }

	/**
	 * Un op�rateur public.
	 * @param p Deuxi�me op�rande.
	 * Soustrait les coordonn�es du point en param�tre de celle du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	Point2D operator-(const Point2D& p) const { return { x - p.x, y - p.y }; }

	/**
	 * Un op�rateur public.
	 * @param val Op�rande de n'importe quel type de base.
	 * Multiplie les coordonn�es du point courant avec la valeur en param�tre.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T>
	Point2D operator*(const T& val) const;

	/**
	 * Un op�rateur public.
	 * @param val Op�rande de n'importe quel type de base.
	 * Divise les coordonn�es du point courant par la valeur en param�tre.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T>
	Point2D operator/(const T& val) const;

	/**
	 * Un op�rateur public.
	 * @param val Op�rande de n'importe quel type de base.
	 * Additionne la valeur en param�tre avec les coordonn�es du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T>
	Point2D operator+(const T& val) const;

	/**
	 * Un op�rateur public.
	 * @param val Op�rande de n'importe quel type de base.
	 * Soustrait la valeur en param�tre des coordonn�es du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T>
	Point2D operator-(const T& val) const;

	/**
	 * Un op�rateur public.
	 * @param i Indice de coordonn�e.
	 * Renvoie la valeur de x ou y en fonction de si i = 0 ou 1.
	 * Renvoie une erreur si i est diff�rent de 0 ou 1.
	 */
	int& operator[](const int& i);

	/**
	 * Un op�rateur public.
	 * @param p Deuxi�me op�rande.
	 * Test d'�galit� entre deux points.
	 */
	bool operator==(const Point2D& p) const;

	/**
	 * Une fonction publique.
	 * Formate le print pour un Point2D, affiche ses coordonn�es (x,y).
	 */
	void print() const;

	/**
	 * Une op�rateur friend.
	 * @param os Sortie standard.
	 * @param p Point affich�.
	 * Red�finition de l'op�rateur <<, servant � l'affichage sur la sortie standard.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Point2D& p);

	/** @brief Calcule la distance entre 2 points pass�s en param�tres
	* @param Point2D a
	* @param Point2D b
	* @return Distance enti�re calcul�e entre a et b
	*/
	static int distance(const Point2D& a, const Point2D& b);

	/** @brief Calcule la distance entre un point et le point actuel
	* @param Point2D p
	* @return Distance enti�re calcul�e entre this et p
	*/
	int distance(const Point2D& p) const;
};

/*===========================================================================================
	Point3D
===========================================================================================*/

/**
 * @class Point3D � trois coordonn�es enti�res
 */
class Point3D
{
public:
	// Coordonn�es enti�res x, y et z du Point3D
	int x;
	int y;
	int z;

	// Constructeurs: par d�faut et normal via Template
	Point3D(void) : x(0), y(0), z(0) {}
	template <class T1, class T2, class T3>
	Point3D(const T1& x, const T2& y, const T3& z) : x((int)x), y((int)y), z((int)z) {}
	Point3D(const Point3D& p) : x(p.x), y(p.y), z(p.z) {}

	// Surcharge des op�rateurs (Op�rations basiques)
	Point3D& operator+(const Point3D& p);
	Point3D& operator-(const Point3D& p);
	Point3D& operator*(const Point3D& p);
	Point3D& operator/(const Point3D& p);

	// Multiplication et division par des cordonn�es par une valeur 'val'
	template <class T>
	Point3D& operator*(const T& val);
	template <class T>
	Point3D& operator/(const T& val);

	//
	int& operator[](const int& i);

	// Test l'�galit� entre deux point 3D
	inline bool operator==(const Point3D& p) const;

	//
	void print() const;

	// Red�finition de l'op�rateur <<
	friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
};