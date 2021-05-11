#pragma once

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
template<class T>
class Point2D
{
public:
	/**
	 * Une variable publique.
	 * Coordonnée x du point.
	 */
	T x;

	/**
	 * Une variable publique.
	 * Coordonnée y du point.
	 */
	T y;

	/**
	 * Un constructeur.
	 * Met les coordonnées à zéro.
	 */
	Point2D() : x(0), y(0) {}

	/**
	 * Un constructeur.
	 * Assigne les coordonnées.
	 */
	Point2D(const T& x, const T& y) : x(x), y(y) {}

	/**
	 * Un constructeur.
	 * Prend des templates en entrée et cast vers int.
	 */
	template <class T1, class T2>
	Point2D(const T1& x, const T2& y) : x((T)x), y((T)y) {}

	/**
	 * Un constructeur.
	 * @param p Point copié.
	 * Copie les coordonnées du point.
	 */
	Point2D(const Point2D& p) : x(p.x), y(p.y) {}

	/**
	 * Un opérateur public.
	 * @param p Deuxième opérande.
	 * Additionne les coordonnées du point en paramètre avec le point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	Point2D operator+(const Point2D& p) const {
		return { x + p.x,y + p.y };
	}

	/**
	 * Un opérateur public.
	 * @param p Deuxième opérande.
	 * Soustrait les coordonnées du point en paramètre de celle du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	Point2D operator-(const Point2D& p) const {
		return { x - p.x,y - p.y };
	}

	/**
	 * Un opérateur public.
	 * @param val Opérande de n'importe quel type de base.
	 * Multiplie les coordonnées du point courant avec la valeur en paramètre.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T1>
	Point2D operator*(const T1& val) const {
		return { x * val,y * val };
	}

	/**
	 * Un opérateur public.
	 * @param val Opérande de n'importe quel type de base.
	 * Divise les coordonnées du point courant par la valeur en paramètre.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T1>
	Point2D operator/(const T1& val) const {
		return { x / val,y / val };
	}

	/**
	 * Un opérateur public.
	 * @param val Opérande de n'importe quel type de base.
	 * Additionne la valeur en paramètre avec les coordonnées du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T1>
	Point2D operator+(const T1& val) const {
		return { x + val,y + val };
	}

	/**
	 * Un opérateur public.
	 * @param val Opérande de n'importe quel type de base.
	 * Soustrait la valeur en paramètre des coordonnées du point courant.
	 * Ne modifie pas le point courant mais en renvoie un nouveau.
	 */
	template <class T1>
	Point2D operator-(const T1& val) const {
		return { x - val,y - val };
	}

	/**
	 * Un opérateur public.
	 * @param i Indice de coordonnée.
	 * Renvoie la valeur de x ou y en fonction de si i = 0 ou 1.
	 * Renvoie une erreur si i est différent de 0 ou 1.
	 */
	int& operator[](const int& i) {
		if (i < 0 || i > 1) {
			std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
			exit(1);
		}
		switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}

	/**
	 * Un opérateur public.
	 * @param p Deuxième opérande.
	 * Test d'égalité entre deux points.
	 */
	bool operator==(const Point2D& p) const {
		return ((p.x == x) && (p.y == y));
	}

	/**
	 * Une fonction publique.
	 * Formate le print pour un Point2D, affiche ses coordonnées (x,y).
	 */
	void print() const {
		std::cout << "[" << x << "|" << y << "]";
	}

	/**
	 * Une opérateur friend.
	 * @param os Sortie standard.
	 * @param p Point affiché.
	 * Redéfinition de l'opérateur <<, servant à l'affichage sur la sortie standard.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Point2D& p);

	/** @brief Calcule la distance entre 2 points passés en paramètres
	* @param Point2D a
	* @param Point2D b
	* @return Distance entière calculée entre a et b
	*/
	static float distance(const Point2D& a, const Point2D& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	/** @brief Calcule la distance entre un point et le point actuel
	* @param Point2D p
	* @return Distance entière calculée entre this et p
	*/
	float distance(const Point2D& p) const {
		return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Point2D<T>& p)
{
	return (os << "[" << p.x << "|" << p.y << "]");
}

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
	Point3D(const T1& x, const T2& y, const T3& z) : x((int)x), y((int)y), z((int)z) {}
	Point3D(const Point3D& p) : x(p.x), y(p.y), z(p.z) {}

	// Surcharge des opérateurs (Opérations basiques)
	Point3D& operator+(const Point3D& point) {
		x += point.x;
		y += point.y;
		z += point.z;
		return *this;
	}
	Point3D& operator-(const Point3D& point) {
		x -= point.x;
		y -= point.y;
		z -= point.z;
		return *this;
	}
	Point3D& operator*(const Point3D& point) {
		x *= point.x;
		y *= point.y;
		z *= point.z;
		return *this;
	}
	Point3D& operator/(const Point3D& point) {
		x /= point.x;
		y /= point.y;
		z /= point.z;
		return *this;
	}

	// Multiplication et division par des cordonnées par une valeur 'val'
	template <class T>
	Point3D& operator*(const T& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	template <class T>
	Point3D& operator/(const T& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}

	//
	int& operator[](const int& i) {
		if (i < 0 || i > 2) {
			std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
			exit(1);
		}
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
	}

	// Test l'égalité entre deux point 3D
	inline bool operator==(const Point3D& p) const {
		return ((p.x == x) && (p.y == y) && (p.z == z));
	}

	//
	void print() const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}

	// Redéfinition de l'opérateur <<
	friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
};

std::ostream& operator<<(std::ostream& os, const Point3D& p)
{
	return (os << "[" << p.x << "|" << p.y << "|" << p.z << "]");
}