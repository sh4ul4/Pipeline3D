#pragma once

#include "library.h"

class Draw {
	struct AnalogNumber {
		AnalogNumber(bool one = NULL, bool two = NULL, bool three = NULL,
			bool four = NULL, bool five = NULL, bool six = NULL, bool seven = NULL)
			:_1(one), _2(two), _3(three), _4(four), _5(five), _6(six), _7(seven) {}
		~AnalogNumber() {}
		bool _1, _2, _3, _4, _5, _6, _7;
	};
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
	static void DrawPoint(const Point2& point, const Color& color, SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		SDL_RenderDrawPoint(renderer, point.x, point.y);
	}
	static void DrawLine(const Point2& point1, const Point2& Point2, const Color& color, SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		SDL_RenderDrawLine(renderer, point1.x, point1.y, Point2.x, Point2.y);
	}
	static void DrawLine(const Point2& point1, const Point2& Point2, SDL_Renderer* renderer)
	{
		SDL_RenderDrawLine(renderer, point1.x, point1.y, Point2.x, Point2.y);
	}
	static void DrawTriangle(const Point2& a, const Point2& b, const Point2& c, SDL_Renderer* renderer)
	{
		const SDL_Point p[4] = {
		{a.x, a.y},
		{b.x, b.y},
		{c.x, c.y},
		{a.x, a.y}
		};
		SDL_RenderDrawLines(renderer, p, 4);
	}
	static void DrawTriangle(const Point2& a, const Point2& b, const Point2& c, const Color& color, SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
		SDL_RenderDrawLine(renderer, b.x, b.y, c.x, c.y);
		SDL_RenderDrawLine(renderer, c.x, c.y, a.x, a.y);
	}
private:
	void DrawFillBottomFlatTriangle(const Point2& v1, const Point2& v2, const Point2& v3, SDL_Renderer* renderer)
	{
		float invslope1_f = float((v2.x - v1.x) / (v2.y - v1.y));
		float invslope2_f = float((v3.x - v1.x) / (v3.y - v1.y));
		float curx1_f = float(v1.x);
		float curx2_f = float(v1.x);
		for (int scanlineY_f = v1.y; scanlineY_f <= v2.y; scanlineY_f++)
		{
			DrawLine({ (int)curx1_f, scanlineY_f }, { (int)curx2_f, scanlineY_f }, white, renderer);
			curx1_f += invslope1_f;
			curx2_f += invslope2_f;
		}
	}
	void DrawFillTopFlatTriangle(const Point2& v1, const Point2& v2, const Point2& v3, SDL_Renderer* renderer)
	{
		float invslope1_f = float((v3.x - v1.x) / (v3.y - v1.y));
		float invslope2_f = float((v3.x - v2.x) / (v3.y - v2.y));
		float curx1_f = float(v3.x);
		float curx2_f = float(v3.x);
		for (int scanlineY_f = v3.y; scanlineY_f > v1.y; scanlineY_f--)
		{
			DrawLine({ (int)curx1_f, scanlineY_f }, { (int)curx2_f, scanlineY_f }, white, renderer);
			curx1_f -= invslope1_f;
			curx2_f -= invslope2_f;
		}
	}
public:
	static void DrawFillTriangle(const Point2& p1, const Point2& p2, const Point2& p3, const Color& color, SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		Point2 tri_f[3];
		tri_f[0] = p1;
		if (p2.x < p1.x) { tri_f[0] = p2; tri_f[1] = p1; }
		else { tri_f[1] = p2; }
		if (p3.x < tri_f[0].x) { tri_f[2] = tri_f[1]; tri_f[1] = tri_f[0]; tri_f[0] = p3; }
		else { if (p3.x < tri_f[1].x) { tri_f[2] = tri_f[1]; tri_f[1] = p3; } else { tri_f[2] = p3; } }
		double facteurx_f = -(tri_f[0].x - tri_f[1].x);
		double facteury_f = -(tri_f[0].y - tri_f[1].y);
		const double fx_1_2_f = facteury_f / facteurx_f;
		const double fy_1_2_f = tri_f[0].y - (fx_1_2_f * tri_f[0].x);
		facteurx_f = -(tri_f[1].x - tri_f[2].x);
		facteury_f = -(tri_f[1].y - tri_f[2].y);
		const double fx_2_3_f = facteury_f / facteurx_f;
		const double fy_2_3_f = tri_f[1].y - (fx_2_3_f * tri_f[1].x);
		facteurx_f = -(tri_f[0].x - tri_f[2].x);
		facteury_f = -(tri_f[0].y - tri_f[2].y);
		const double fx_1_3_f = facteury_f / facteurx_f;
		const double fy_1_3_f = tri_f[0].y - (fx_1_3_f * tri_f[0].x);
		Point2 remplissage1_f;
		Point2 remplissage2_f;
		int ix_f = tri_f[0].x;
		const int size = (tri_f[2].x - ix_f) * 2;
		int it = 0;
		SDL_Point* const lines = (SDL_Point*)malloc(size * sizeof(SDL_Point));
		if (lines == NULL)return;
		while (ix_f < tri_f[2].x) {
			if (ix_f < tri_f[1].x) {
				remplissage1_f.x = ix_f;
				remplissage1_f.y = int(fx_1_2_f * ix_f + fy_1_2_f);
				remplissage2_f.x = ix_f;
				remplissage2_f.y = int(fx_1_3_f * ix_f + fy_1_3_f);
				ix_f++;
				lines[it] = { remplissage1_f.x, remplissage1_f.y };
				it++;
				lines[it] = { remplissage2_f.x, remplissage2_f.y };
				it++;
			}
			else {
				remplissage1_f.x = ix_f;
				remplissage1_f.y = int(fx_2_3_f * ix_f + fy_2_3_f);
				remplissage2_f.x = ix_f;
				remplissage2_f.y = int(fx_1_3_f * ix_f + fy_1_3_f);
				ix_f++;
				lines[it] = { remplissage1_f.x, remplissage1_f.y };
				it++;
				lines[it] = { remplissage2_f.x, remplissage2_f.y };
				it++;
			}
		}
		SDL_RenderDrawLines(renderer, lines, size);
		free(lines);
	}
	static void DrawCircle(const int& X, const int& Y, const int& radius, const Color& color, SDL_Renderer* renderer)
	{
		if (radius < 0) { std::cout << "Radius must be positive.\n"; return; }
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		for (int y = -radius; y <= radius; y++) {
			for (int x = -radius; x <= radius; x++) {
				if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius) {
					SDL_RenderDrawPoint(renderer, X + x, Y + y);
				}
			}
		}
	}
	static void DrawFillCircle(const int& X, const int& Y, const int& radius, const Color& color, SDL_Renderer* renderer)
	{
		if (radius < 0) { std::cout << "Radius must be positive.\n"; return; }
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		for (int y = -radius; y <= radius; y++) {
			for (int x = -radius; x <= radius; x++) {
				if (x * x + y * y < radius * radius + radius) {
					SDL_RenderDrawPoint(renderer, X + x, Y + y);
				}
			}
		}
	}
	bool RenderString(const int& x, const int& y, const int& size, const std::string& string, const Color& color, SDL_Renderer* renderer) // ! in progress !
	{
		std::vector< AnalogChar> chars_f;
		for (int i = static_cast<int>(string.size()) - 1; i >= 0; i--)
		{
			switch (string[i]) {
			case 'a':chars_f.push_back({ 0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'b':chars_f.push_back({ 1,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'c':chars_f.push_back({ 0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'd':chars_f.push_back({ 1,0,1,1,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'e':chars_f.push_back({ 1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'f':chars_f.push_back({ 1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'g':chars_f.push_back({ 0,1,0,1,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'h':chars_f.push_back({ 0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'i':chars_f.push_back({ 1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 }); break;
			case 'j':chars_f.push_back({ 1,1,1,0,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'k':chars_f.push_back({ 0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0 }); break;
			case 'l':chars_f.push_back({ 0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'm':chars_f.push_back({ 0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'n':chars_f.push_back({ 0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0 }); break;
			case 'o':chars_f.push_back({ 0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'p':chars_f.push_back({ 1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'q':chars_f.push_back({ 0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0 }); break;
			case 'r':chars_f.push_back({ 1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0 }); break;
			case 's':chars_f.push_back({ 0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 't':chars_f.push_back({ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1 }); break;
			case 'u':chars_f.push_back({ 0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'v':chars_f.push_back({ 0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0 }); break;
			case 'w':chars_f.push_back({ 0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0 }); break;
			case 'x':chars_f.push_back({ 0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case 'y':chars_f.push_back({ 0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 }); break;
			case 'z':chars_f.push_back({ 1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case '#':chars_f.push_back({ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }); break;
			case '*':chars_f.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case '+':chars_f.push_back({ 0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0 }); break;
			case '-':chars_f.push_back({ 0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
			case ' ':chars_f.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }); break;
				//case ''':chars_f.push_back({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0 }); break;
			default:chars_f.push_back({ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }); break;
			}
		}
		for (int i = static_cast<int>(chars_f.size()) - 1; i >= 0; i--)
		{
			Point2 a, b;
			if (chars_f[i]._1) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color,renderer);
			}
			if (chars_f[i]._2) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._3) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y;
				b.x = a.x;
				b.y = size / 4 + y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._4) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = size / 4 + y;
				b.x = a.x;
				b.y = a.y + size / 4;;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._5) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = size / 2 + y;
				b.x = a.x;
				b.y = size / 4 + size / 2 + y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._6) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = size / 2 + size / 4 + y;
				b.x = a.x;
				b.y = size + y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._7) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = size / 2 + y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._8) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = size / 2 + y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._9) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._10) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y + size / 4;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._11) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y + size / 2;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._12) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = size / 4 + size / 2 + y;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._13) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = size + y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._14) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = size + y;
				b.x = a.x + size / 4;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._15) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._16) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._17) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y + size / 4;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._18) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y + size / 4;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._19) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 2;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._20) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 2;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._21) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y + size / 4 + size / 2;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._22) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y + size / 4 + size / 2;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			} // ---
			if (chars_f[i]._23) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._24) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._25) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 4;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._26) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 4;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._27) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 2;
				a.y = y + size / 2;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._28) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x;
				a.y = y + size / 2;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._29) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 4 + size / 2;
				b.x = a.x - size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._30) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 4 + size / 2;
				b.x = a.x + size / 4;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._31) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._32) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 4;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._33) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 2;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
			if (chars_f[i]._34) {
				a.x = (static_cast<int>(chars_f.size()) - i - 1) * size + x + size / 4;
				a.y = y + size / 2 + size / 4;
				b.x = a.x;
				b.y = a.y + size / 4;
				DrawLine(a, b, color, renderer);
			}
		}
		return 0;
	}
	static bool RenderDigits(const int& X, const int& Y, const int& size, long long int Num, const Color& color, SDL_Renderer* renderer) // render Digits
	{
		std::vector<AnalogNumber> num_f;
		const bool sign = (Num < 0);
		if (sign) { Num *= -1; } // can handle negative numbers
		if (Num == 0) { num_f.push_back({ 1,1,1,0,1,1,1 }); } // number equals zero
		while (Num > 0)
		{
			switch (Num % 10) {
			case 0:num_f.push_back({ 1,1,1,0,1,1,1 }); break;
			case 1:num_f.push_back({ 0,0,1,0,0,1,0 }); break;
			case 2:num_f.push_back({ 1,0,1,1,1,0,1 }); break;
			case 3:num_f.push_back({ 1,0,1,1,0,1,1 }); break;
			case 4:num_f.push_back({ 0,1,1,1,0,1,0 }); break;
			case 5:num_f.push_back({ 1,1,0,1,0,1,1 }); break;
			case 6:num_f.push_back({ 0,1,0,1,1,1,1 }); break;
			case 7:num_f.push_back({ 1,0,1,0,0,1,0 }); break;
			case 8:num_f.push_back({ 1,1,1,1,1,1,1 }); break;
			case 9:num_f.push_back({ 1,1,1,1,0,1,0 }); break;
			default:break;
			}
			Num = Num / 10;
		}
		if (sign) { num_f.push_back({ 0, 0, 0, 1, 0, 0, 0 }); }
		for (int i = static_cast<int>(num_f.size()) - 1; i >= 0; i--)
		{
			Point2 a, b;
			if (num_f[i]._1) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + X;
				a.y = size / 2 + Y;
				b.x = a.x + size / 2;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._2) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + X;
				a.y = size / 2 + Y;
				b.x = a.x;
				b.y = size + Y;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._3) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + size / 2 + X;
				a.y = size / 2 + Y;
				b.x = a.x;
				b.y = size + Y;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._4) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + X;
				a.y = size + Y;
				b.x = a.x + size / 2;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._5) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + X;
				a.y = size + Y;
				b.x = a.x;
				b.y = a.y + size / 2;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._6) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + size / 2 + X;
				a.y = size + Y;
				b.x = a.x;
				b.y = size + size / 2 + Y;
				DrawLine(a, b, color, renderer);
			}
			if (num_f[i]._7) {
				a.x = (static_cast<int>(num_f.size()) - i - 1) * size + X;
				a.y = size + size / 2 + Y;
				b.x = a.x + size / 2;
				b.y = a.y;
				DrawLine(a, b, color, renderer);
			}
		}
		return 0;
	}
};