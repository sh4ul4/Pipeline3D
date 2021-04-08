#pragma once

#include "library.h"

class Draw {

	/**
	 * Une structure publique.
	 * Elle contient la gestion des chiffres analogiques
	 */	

	struct AnalogNumber {
		AnalogNumber(bool one = NULL, bool two = NULL, bool three = NULL,
			bool four = NULL, bool five = NULL, bool six = NULL, bool seven = NULL)
			:_1(one), _2(two), _3(three), _4(four), _5(five), _6(six), _7(seven) {}
		~AnalogNumber() {}
		bool _1, _2, _3, _4, _5, _6, _7;
	};
	
	/**
	 * Une structure publique.
	 * Elle contient la gestion des caractéres analogiques
	 */		
	
	struct AnalogChar {
		AnalogChar(bool A = NULL, bool B = NULL, bool C = NULL, bool D = NULL, bool E = NULL,
			bool F = NULL, bool G = NULL, bool H = NULL, bool I = NULL, bool J = NULL, bool K = NULL,
			bool L = NULL, bool M = NULL, bool N = NULL, bool O = NULL, bool P = NULL, bool Q = NULL,
			bool R = NULL, bool S = NULL, bool T = NULL, bool U = NULL, bool V = NULL, bool W = NULL,
			bool X = NULL, bool Y = NULL, bool Z = NULL, bool a = NULL, bool b = NULL, bool c = NULL,
			bool d = NULL, bool e = NULL, bool f = NULL, bool g = NULL, bool h = NULL)
			:_1(A), _2(B), _3(C), _4(D), _5(E), _6(F), _7(G), _8(H), _9(I), _10(J), _11(K), _12(L),
			_13(M), _14(N), _15(O), _16(P), _17(Q), _18(R), _19(S), _20(T), _21(U), _22(V), _23(W),
			_24(X), _25(Y), _26(Z), _27(a), _28(b), _29(c), _30(d), _31(e), _32(f), _33(g), _34(h) {}
		~AnalogChar() {}
		bool _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18,
			_19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34;
	};
public:

	/**
	 * Une fonction publique.
	 * @brief Dessine un point 2D pré existant avec une couleur donnée en parametre
	 * @param point Point 2D pré-existant
	 * @param color Couleur du futur point
	 * @param renderer Moteur de rendu SDL
	 */	

	static void DrawPoint(const Point2D& point, const Color& color, SDL_Renderer* renderer);

	/**
	 * Une fonction publique.
	 * @brief Dessine une ligne 2D avec deux points pré existants et une couleur donnée 
	 * @param point1, point2 Points 2D pré-existants
	 * @param color Couleur de la future ligne 
	 * @param renderer Moteur de rendu SDL
	 */	
		
	static void DrawLine(const Point2D& point1, const Point2D& point2, const Color& color, SDL_Renderer* renderer);
	
	/**
	 * Une fonction publique.
	 * @brief Dessine une ligne 2D avec deux points pré existants et la derniére couleur utilisée par le renderer 
	 * @param point1, point2 Points 2D pré-existants
	 * @param renderer Moteur de rendu SDL
	 */	
	
	static void DrawLine(const Point2D& point1, const Point2D& point2, SDL_Renderer* renderer);
	
	/**
	 * Une fonction publique.
	 * @brief Dessine un triangle 2D avec trois points pré existants, en reliant des lignes et la derniére couleur utilisée par le renderer 
	 * @param a, b, c Points 2D pré-existants
	 * @param renderer Moteur de rendu SDL
	 */	
	
	static void DrawTriangle(const Point2D& a, const Point2D& b, const Point2D& c, SDL_Renderer* renderer);
	
	/**
	 * Une fonction publique.
	 * @brief Dessine un triangle 2D avec trois points pré existants, en reliant des lignes et une couleur donnée
	 * @param a, b, c Points 2D pré-existants
	 * @param color Couleur du futur triangle 
	 * @param renderer Moteur de rendu SDL
	 */		
	
	static void DrawTriangle(const Point2D& a, const Point2D& b, const Point2D& c, const Color& color, SDL_Renderer* renderer);
	
	
private:

	/**
	 * Une fonction privée.
	 * @brief Rempli la partie inférieure d'un triangle 2D tant que ça ne dépasse pas des points donnés 
	 * @param v1, v2, v3 Points 2D pré-existants
	 * @param renderer Moteur de rendu SDL
	 */		

	void DrawFillBottomFlatTriangle(const Point2D& v1, const Point2D& v2, const Point2D& v3, SDL_Renderer* renderer);
	
	/**
	 * Une fonction privée.
	 * @brief Rempli la partie superieure d'un triangle 2D tant que ça ne va paq en dessous des points donnés 
	 * @param v1, v2, v3 Points 2D pré-existants
	 * @param renderer Moteur de rendu SDL
	 */		

	void DrawFillTopFlatTriangle(const Point2D& v1, const Point2D& v2, const Point2D& v3, SDL_Renderer* renderer);
	
public:

	/**
	 * Une fonction publique.
	 * @brief Remplie avec une couleur donnée un triangle 2D compris entre des points 2D qui forment des lignes 
	 * @param p1, p2, p3 Points 2D pré-existants
	 * @param color Couleur de remplissage du triangle
	 * @param renderer Moteur de rendu SDL
	 */		

	static void DrawFillTriangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, const Color& color, SDL_Renderer* renderer);
	
	/**
	 * Une fonction publique.
	 * @brief Dessine avec une couleur donnée un cercle 2D définis par un centre (x,y) et un rayon
	 * @param x,y Coordonnées du centre du cercle 
	 * @param radius Rayon du cercle
	 * @param color Couleur du cercle 
	 * @param renderer Moteur de rendu SDL
	 */		
	
	
	static void DrawCircle(const int& X, const int& Y, const int& radius, const Color& color, SDL_Renderer* renderer);
	
  	 /**
	 * Une fonction publique.
	 * @brief Remplie avec une couleur donnée un cercle 2D définis par un centre (x,y) et un rayon
	 * @param x,y Coordonnées du centre du cercle 
	 * @param radius Rayon du cercle
	 * @param color Couleur de remplissage du cercle 
	 * @param renderer Moteur de rendu SDL
	 */	
	 
	 static void DrawFillCircle(const int& X, const int& Y, const int& radius, const Color& color, SDL_Renderer* renderer);
	 
	
	
};
