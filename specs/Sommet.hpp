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
	float x;
	float y;
	float z;
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
	
	/* --- Surchages d'opérateurs --- */
	Sommet &operator=(const Sommet &other); // Le sommet courant prend les coordonnées du sommet en paramètre
	Sommet &operator+=(const Sommet &other); // Les coordonnées du sommet courant sont égal à la somme d'eux-même avec les coordonnées du sommet en paramètre 
	Sommet &operator-=(const Sommet &other); // ---- différence ---
	
	Sommet &operator*=(const float &val); // Multiplication des coordonnées du sommet courant par une valeur float
	Sommet &operator*=(const int &val); // Multiplication des coordonnées du sommet courant par une valeur int
	Sommet &operator*=(const double &val); // Multiplication des coordonnées du sommet courant par une valeur double
	Sommet &operator/=(const float &val); // Division des coordonnées du sommet courant par une valeur float
	Sommet &operator/=(const int &val); // Division des coordonnées du sommet courant par une valeur int
	Sommet &operator/=(const double &val); // Division des coordonnées du sommet courant par une valeur double

	const Sommet operator*(const float &val) const
	{
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const double &val) const
	{
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const int &val) const
	{
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator/(const float &val) const
	{
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const double &val) const
	{
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const int &val) const
	{
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	float &operator[](const unsigned &i)
	{
		switch (i)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
		FATAL_ERR("Index out of bounds for instance of [Vertex]")
	}
	// Return Point3D with the value of sommet
	Point3D toPoint3D() const
	{
		return Point3D(x, y, z);
	}
	//Le sommet devient à une distance de l'origine dans la même direction.
	void normalize()
	{
		const float length = getLength();
		*this /= length;
	}
	//Renvoie le produit scalaire
	float dotProduct(const Sommet &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	//Renvoie un sommet qui le résultat produit en croix de 2 sommets
	Sommet crossProduct(const Sommet &v) const
	{
		return Sommet(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	//Calcul de distance entre 2 sommets
	float distance(const Sommet &v) const
	{
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}

	/**
	 * @brief Calcule la distance le point actuel et 2 points formant une ligne
	 * 
	 * @param Sommet& P0 Premier point formant la ligne
	 * @param Sommet& P2 Deuxième point formant la ligne
	 * 
	 * @return Distance flottante entre le point actuel et la ligne donnée
	 */
	float distanceToLine(const Sommet &P0, const Sommet &P1) const;

	//Afficher les coordonnées du sommet par stdout
	void print(void) const
	{
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream &operator<<(std::ostream &os, const Sommet &v);

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
	// Retourne la distance par rapport à l'origine du rapport
	float getLength(void) const
	{
		return sqrtf(x * x + y * y + z * z);
	}
};
//Surchage d'opérateur
inline const Sommet operator+(const Sommet &v1, const Sommet &v2)
{
	return Sommet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline const Sommet operator-(const Sommet &v1, const Sommet &v2)
{
	return Sommet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline const Sommet operator*(const Sommet &v1, const Sommet &v2)
{
	return Sommet(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline const Sommet operator/(const Sommet &v1, const Sommet &v2)
{
	return Sommet(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

float Sommet::distanceToLine(const Sommet &P0, const Sommet &P1) const
{
	Sommet v = P1 - P0;
	const Sommet w = *this - P0;

	const float c1 = w.dotProduct(v);
	if (c1 <= 0) // before P0
		return this->distance(P0);
	const float c2 = v.dotProduct(v);
	if (c2 <= c1) // after P1
		return this->distance(P1);
	v *= (c1 / c2);
	return this->distance(P0 + v);
}
//affiche les coordonées des sommets en surchageant l'opérateur <<
std::ostream &operator<<(std::ostream &os, const Sommet &v)
{
	return (os << "[" << v.x << "|" << v.y << "|" << v.z << "]");
}
