#pragma once
//#include "library.h"


// Quitter le programme en affichant un message d'erreur 
#define PRINT_ON_ERR(s) { std::cout << s << std::endl; exit(1); }

/**
 * @class Point à deux coordonnées entières
 * 
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
  		const int& operator[](const int& i) {
            if (i < 0 || i > 1) 
              	PRINT_ON_ERR("Index out of bounds for instance of [Vertex]")
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
  
  		/** @brief Calcule la distance entre 2 points 
          * @param Point2D a
          *	@param Point2D b
          * @return Distance entière calculée entre a et b
          */
        static int distance(const Point2D& a, const Point2D& b) {
            return static_cast<int>(sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
        }
  
  		/** @brief Calcule la distance entre le point this et un point extérieur
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
 * @class Point2F à deux coordonnées flottantes
 * 
 */
class Point2F {
	public: 
	// Coordonnées flottantes x et y
	float x;
	float y;
	// Constructeurs par défaut
	Point2F() :x(0), y(0) {}
  // Constructeur à partir de 2 valeurs
	Point2F(const float& x, const float& y) :x(x), y(y) {}
  // Constructeur avec template
	template<class T1, class T2>
	Point2F(const T1& x, const T2& y) : x((float)x), y((float)y) {}
  // Constructeur à partir d'un point
	Point2F(const Point2F& p) : x(p.x), y(p.y) {}
  //Surcharges d'opérateurs
  //const: signifie qu'il renvoie un nouveau point sans modifier la classe qu'il appelle
	Point2F operator+(const Point2F& point)const {
		return { x + point.x,y + point.y };
	}
	Point2F operator-(const Point2F& point)const {
		return { x - point.x,y - point.y };
	}
  //Surcharge d'opérateurs avec les classes templates
	template <class T>
	Point2F operator*(const T& val)const {
		return { x * val,y * val };
	}
	template <class T>
	Point2F operator/(const T& val)const {
		return { x / val,y / val };
	}
	template <class T>
	Point2F operator+(const T& val)const {
		return { x + val,y + val };
	}
	template <class T>
	Point2F operator-(const T& val)const {
		return { x - val,y - val };
	}
  // Operateur[] qui renvoie x ou y en fonction de si i est égal à 0 ou 1.
  // Renvoie une erreur si i est différent de 0 ou 1.
	const float& operator[](const int& i) {
		if (i < 0 || i > 1) {
			std::cout << "Index out of bounds for instance of [Vertex]" << std::endl;
			exit(1);
		}
		switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}
  // Operateur de test d'égalité entre deux points
	bool operator==(const Point2F& p) const {
		return ((p.x == x) && (p.y == y));
	}
  // Affiche les valeurs de x et y
	void print() const {
		std::cout << "[" << x << "|" << y << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point2F& p);
  // Renvoie la distance entre 2 points mis en paramètres
	static float distance(const Point2F& a, const Point2F& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
  // Renvoie la distance entre un point et le point actuel
	float distance(const Point2F& p) const {
		return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};

std::ostream& operator<<(std::ostream& os, const Point2F& p)  {
	return (os << "[" << p.x << "|" << p.y << "]");
}

/**
 * @class Point3D à trois coordonnées entières
 * 
 */
class Point3D {
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
		if (i < 0 || i > 2) {
			PRINT_ON_ERR("Index out of bounds for instance of [Vertex]")
    	switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
		}
	}
	inline bool operator==(const Point3& p) const {
		return ((p.x == x) && (p.y == y) && (p.z == z));
	}
	void print() const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Point3& p);
};

std::ostream& operator<<(std::ostream& os, const Point3& p)
{
	return (os << "[" << p.x << "|" << p.y << "|" << p.z << "]");
}


//##############################################################################################
#pragma once

//#include "library.h"
#include "Point.h"
#include <array>

struct Sommet {
	float x;
	float y;
	float z;
	Sommet() : x(0), y(0), z(0) {}
	Sommet(const Point3& p) :x(p.x), y(p.y), z(p.z) {}
	Sommet(const std::array<float, 3>& vec3d) :x(vec3d[0]), y(vec3d[1]), z(vec3d[2]) {}
	Sommet(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}
	template<class T1, class T2, class T3>
	Sommet(const T1& x, const T2& y, const T3& z) : x((float)x), y((float)y), z((float)z) {}
	Sommet (const Sommet& v) : x(v.x), y(v.y), z(v.z) {}
  	//Pren
	Sommet& operator=(const Sommet& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	Sommet& operator+=(const Sommet& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Sommet& operator-=(const Sommet& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	Sommet& operator*=(const float& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator*=(const int& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator*=(const double& val) {
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	Sommet& operator/=(const float& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	Sommet& operator/=(const int& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	Sommet& operator/=(const double& val) {
		x /= val;
		y /= val;
		z /= val;
		return *this;
	}
	const Sommet operator*(const float& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const double& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator*(const int& val) const {
		return Sommet(x * val, y * val, z * val);
	}
	const Sommet operator/(const float& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const double& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	const Sommet operator/(const int& val) const {
		//if(val == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
		return Sommet(x / val, y / val, z / val);
	}
	float& operator[](const unsigned& i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
		std::cout << "Index out of bounds for instance of [Vertex]" << std::endl; exit(1);
	}
	Point3 toPoint3()const {
		return Point3(x,y,z);
	}

	static void normalize(Sommet& v) {
		v /= v.getLength();
	}
	void normalize() {
		const float length = getLength();
		*this /= length;
	}

	static float dot(const Sommet& v1, const Sommet& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	float dot(const Sommet& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	static Sommet cross(const Sommet& v1, const Sommet& v2) {
		return Sommet(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	Sommet cross(const Sommet& v)const {
		return Sommet(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	static float distance(const Sommet& a,const Sommet& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
	}
	float distance(const Sommet& v)const {
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}

	static float distanceToLine(const Sommet& P, const Sommet& P0, const Sommet& P1);
	float distanceToLine(const Sommet& P0, const Sommet& P1)const;

	void print(void)const {
		std::cout << "[" << x << "|" << y << "|" << z << "]";
	}
	friend std::ostream& operator<<(std::ostream& os, const Sommet& v);

	void normalizeOnLength(const float& length) {
		const float sum = abs(x) + abs(y) + abs(z);
		if (sum == 0) {
			this->x = this->y = this->z = 0; return;
		}
		const float normalizedLength = length / sum;
		x *= normalizedLength;
		y *= normalizedLength;
		z *= normalizedLength;
	}
	float getLength(void) const {
		return sqrtf(x * x + y * y + z * z);
	}
};

inline const Sommet operator+(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline const Sommet operator-(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline const Sommet operator*(const Sommet& v1, const Sommet& v2) {
	return Sommet(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline const Sommet operator/(const Sommet& v1, const Sommet& v2) {
	//if (v.x == 0 || v.y == 0 || v.z == 0) { std::cout << "ERROR : cannot divide by 0" << std::endl; exit(1); }
	return Sommet(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

float Sommet::distanceToLine(const Sommet& P, const Sommet& P0, const Sommet& P1) {
	Sommet v = P1 - P0;
	const Sommet w = P - P0;

	const float c1 = w.dot(v);
	if (c1 <= 0)  // before P0
		return distance(P, P0);
	const float c2 = v.dot(v);
	if (c2 <= c1) // after P1
		return distance(P, P1);
	v *= (c1 / c2);
	return distance(P, P0 + v );
}
float Sommet::distanceToLine(const Sommet& P0, const Sommet& P1)const {
	Som v = P1 - P0;
	const Sommet w = *this - P0;

	const float c1 = w.dot(v);
	if (c1 <= 0)  // before P0
		return distance(*this, P0);
	const float c2 = v.dot(v);
	if (c2 <= c1) // after P1
		return distance(*this, P1);
	v *= (c1 / c2);
	return distance(*this, P0 + v);
}

std::ostream& operator<<(std::ostream& os, const Sommet& v)
{
	return (os << "[" << v.x << "|" << v.y << "|" << v.z << "]");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bitmap{
public:
	SDL_Surface* surface = nullptr;
	Bitmap() = delete;
  	/**	@brief Constructeur 
      *
      *	vérifier si l'argument path est valide
      *	créer une nouvelle surface avec la fonction IMG_Load()
      *	définir la variable temporaire pixelFormat avec la fonction SDL_AllocFormat()
      *	convertir la surface avec le nouveau format défini
      *	bloquer les pixels de la surface avec SDL_LockSurface()
      * vérifier les erreurs avec SDL_GetError() et supprimer les variables temporaires
      */
	Bitmap(const char* path) {}
  	/** @brief Destructeur
	  *
	  * Débloquer et libérer les pixel de la bitmap
	  */
	~Bitmap() {
		SDL_UnlockSurface(surface);
		SDL_FreeSurface(surface);
	}
};
