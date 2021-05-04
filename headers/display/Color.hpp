#pragma once
/** @brief class Color
 * La classe Color permet de faciliter l'utilisation de couleurs RGBA 32 bits
 */
class Color {

public:

	Uint8 r; // rouge
	Uint8 g; // vert
	Uint8 b; // bleu
	Uint8 a; // transparence

	// constructeur par défaut
	Color(const Uint8 R = NULL, const Uint8 G = NULL, const Uint8 B = NULL, const Uint8 A = 255) : r(R), g(G), b(B), a(A) {}

	// conversion vers la classe SDL_Color
	SDL_Color toSDL_Color() const {
		SDL_Color color;
		color.a = a;
		color.r = r;
		color.g = g;
		color.b = b;
		return color;
	}

	// correspondance entre deux couleurs
	bool operator == (const Color& col) { return (col.r == r && col.g == g && col.b == b && col.a == a); }

	// inégalité entre deux couleurs
	bool operator != (const Color& col) { return (col.r != r || col.g != g || col.b != b || col.a != a); }

};

// instances de couleurs basiques
const Color white(255, 255, 255);
const Color light_gray(200, 200, 200);
const Color gray(127, 127, 127);
const Color dark_gray(80, 80, 80);
const Color black(0, 0, 0);
const Color red(255, 0, 0);
const Color lime(0, 255, 0);
const Color blue(0, 0, 255);
const Color maroon(128, 0, 0);
const Color green(0, 128, 0);
const Color navy(0, 0, 128);
const Color olive(128, 128, 0);
const Color purple(128, 0, 128);
const Color teal(0, 128, 128);
const Color yellow(255, 255, 0);
const Color magenta(255, 0, 255);
const Color cyan(0, 255, 255);
const Color elynbeige(248, 237, 227);
const Color elynmarron(216, 172, 156);
const Color elynvfonce(121, 135, 119);
