#pragma once
class Key {
public:
	bool up = false;
	bool down = false;
	bool pressed = false;
	void setBack() {
		up = down = pressed = false;
	}
	void press() {
		if (pressed) {
			down = false;
			return;
		}
		down = true;
		pressed = true;
		up = false;
	}
	void release() {
		if (!pressed) {
			up = false;
			return;
		}
		pressed = false;
		up = true;
		down = false;
	}
	Key& operator=(const Key& other) {
		if (other.down && pressed)down = false;
		else down = other.down;
		pressed = other.pressed;
		up = other.up;
		return *this;
	}
};

class Keyboard {
public:
	Key a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
	Key A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;
	Key zero, one, two, three, four, five, six, seven, eight, nine;
	Key up, down, left, right;
	Key F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;
	Key tab, plus, minus, slash, backslash, equal, point, semicolon, escape, space, enter, delete_, quote, shift, ctrl, backspace, comma;
};