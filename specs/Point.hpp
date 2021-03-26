#pragma once
//#include "library.h"
#include <iostream>
#include <math.h>
#include "ErrorsHandler.h"

/**
 * @file Issu du module de Géométrie, sous-module (?) 
 * Regroupe les classes Point2D et Point 3D
 */ 

/**
 * @class Point à deux coordonnées entières
 */
class Point2D {
    public:
  		// Coordonnées entières x et y du Point2D 
    	int x; 
  		int y;
  
  		// Constructeurs: par défaut et normal
  		Point2D() : x(0), y(0) {}
  		Point2D(const int& x, const int& y) :x(x), y(y) {}
		
  		// Constructeur par template en castant en entier 
 		template<class T1, class T2>
        Point2D(const T1& x, const T2& y) : x((int)x), y((int)y) {}
  		
  		// Constructeur par copie d'un Point2D
		Point2D(const Point2D& p) : x(p.x), y(p.y) {}
  
  		// Surcharges d'opérateurs
  		// Const à la fin: ne modifie pas la classe qu'il appelle (renvoie un nouveau point)
		//const: signifie qu'il renvoie un nouveau point sans modifier la classe qu'il appelle
        Point2D operator+(const Point2D& point) const {
			return { x + point.x, y + point.y };
        }
        Point2D operator-(const Point2D& point) const {
			return { x - point.x,y - point.y };
        }

		// Surcharges avec classe template
        template <class T>
        Point2D operator*(const T& val)const {
            return { x * val, y * val };
        }
        template <class T>
        Point2D operator/(const T& val)const {
            return { x / val, y / val };
        }
        template <class T>
        Point2D operator+(const T& val)const {
            return { x + val, y + val };
        }
        template <class T>
        Point2D operator-(const T& val)const {
            return { x - val, y - val };
        }
		
  		// Opérateur [], renvoie x ou y si i = 0 ou 1
		// Operateur[] qui renvoie x ou y en fonction de si i est égal à 0 ou 1.
		// Renvoie une erreur si i est différent de 0 ou 1.
  		const int& operator[](const int& i) {
            if (i < 0 || i > 1) 
              	FATAL_ERR("Index out of bounds for instance of [Vertex]")
            switch (i) {
                case 0: return x;
                case 1: return y;
            }
		}
  		
  		// Test d'égalité entre deux points
  		bool operator==(const Point2D& p) const {
            return ((p.x == x) && (p.y == y));
        }
  
  		// Formate le print pour un Point2D
        void print() const {
            std::cout << "[" << x << "|" << y << "]";
        }
		
        friend std::ostream& operator<<(std::ostream& os, const Point2D& p);
   
  		/** @brief Calcule la distance entre 2 points passés en paramètres
          * @param Point2D a
          *	@param Point2D b
          * @return Distance entière calculée entre a et b
          */
        static int distance(const Point2D& a, const Point2D& b) {
            return static_cast<int>(sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
        }

  		/** @brief Calcule la distance entre un point et le point actuel
          *	@param Point2D p
          * @return Distance entière calculée entre this et p
          */
  		int distance(const Point2D& p) const {
            return static_cast<int>(sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)));
        }
  
  	private:
};

std::ostream& operator<<(std::ostream& os, const Point2D& p)  {
	return (os << "[" << p.x << "|" << p.y << "]");
}


/**
 * @class Point3D à trois coordonnées entières
 * 
 */
class Point3D {
	public:
  	// Coordonnées entières x, y et z du Point3D 
    int x; 
  	int y;
  	int z;
  
  	// Constructeurs: par défaut et normal via Template
  	Point3D(void) :x(0), y(0), z(0) {}
	template<class T1, class T2, class T3>
	Point3D(const T1& x, const T2& y, const T3& z) : x((int)x), y((int)y), z((int)z) {}
	Point3D(const Point3D& p) : x(p.x), y(p.y), z(p.z) {}
    
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
	int& operator[](const int& i) {
		if (i < 0 || i > 2) 
			FATAL_ERR("Index out of bounds for instance of [Vertex]")
    	switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
		}
	}
	inline bool operator==(const Point3D& p) const {
		return ((p.x == x) && (p.y == y) && (p.z == z));
	}
	void print() const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point3D& p);
};

std::ostream& operator<<(std::ostream& os, const Point3D& p)
{
	return (os << "[" << p.x << "|" << p.y << "|" << p.z << "]");
};
