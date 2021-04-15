/**
 * @file Module Interaction utilisateur
 */

/** @brief class Key
 * La classe Key gère les interactions utilisateur qui passent par une touche du clavier.
 */
class Key {
public:
	/** @brief up Touche noon appuyée. */
	bool up = false;

	/** @brief down Touche appuyée. */
	bool down = false;

	/** @brief pressed Touche vient d'être enfoncée. */
	bool pressed = false;

	/** @brief Mettre à zéro la touche. */
	void setBack();

	/** @brief Appuyer sur la touche. */
	void press();

	/** @brief Relacher la touche. */
	void release();

	/** @brief Copier la touche correctement. */
	Key& operator=(const Key& other);
};

/** @brief class Keyboard
 * La classe Keyboard gère les différentes touches du clavier.
 */
class Keyboard {
public:
	/** Touches du clavier. */
	Key a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
	Key A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	Key zero, one, two, three, four, five, six, seven, eight, nine;
	Key up, down, left, right;
	Key F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;
	Key tab, plus, minus, slash, backslash, equal, point, semicolon, escape, space, enter, delete_, quote, shift, backspace, comma;
};