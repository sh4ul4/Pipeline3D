#pragma once

//#include "library.h"
#include "Draw.h"
#include "Window.h"
#include <map>
#include <unordered_map>
#include <thread>

class GlobalTexture {
	SDL_Texture* texture;
	size_t width;
	size_t height;
	std::vector<Uint32> pixels; //std::vector< unsigned char > pixelsV;
	int pitch = 0;
public:
	float* zbuffer;
	SDL_PixelFormat* pixelFormat;
public:
	GlobalTexture() = delete;
	GlobalTexture(const Window& w) {
		zbuffer = nullptr;
		texture = SDL_CreateTexture(w.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, w.getWidth(), w.getHeight());
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		width = w.getWidth();
		height = w.getHeight();
		// generate pixel-array
		pixels.reserve(width * height);
		for (int i = 0; i < width * height; i++) pixels.emplace_back(0);
		pixels.shrink_to_fit();
		pitch = width * 4;
		//pixelsV.reserve(width * height * 4);
		zbuffer = new float[width * height];
		pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB32);
		const char* error_f = SDL_GetError();
		if (*error_f) { std::cout << "Error occured in Texture(): " << error_f << std::endl; }
	}
	~GlobalTexture() {
		SDL_DestroyTexture(texture);
		delete[] zbuffer;
		zbuffer = nullptr;
		SDL_FreeFormat(pixelFormat);
		const char* error_f = SDL_GetError();
		if (*error_f) { std::cout << "Error occured in ~Texture(): " << error_f << std::endl; }
	}
	inline float getZvalue(const int& at) {
		//if (at < 0 || at >= width * height)return 0;
		return zbuffer[at];
	}
	inline void putZvalue(const int& at, const float& value) {
		//if (at < 0 || at >= width * height)return;
		zbuffer[at] = value;
	}
	inline void refreshZbuffer() {
		for (int i = 0; i < width * height; i++) {
			zbuffer[i] = 999999;
		}
		//std::memset(zbuffer, 999999, width * height);
	}
	void updateTexture(const Window& w) {
		if (texture == nullptr || pixels.empty()) { std::cout << "Error occured in GlobalTexture::updateTexture()" << std::endl; exit(1); }
		SDL_UpdateTexture(texture, NULL, pixels.data(), pitch);
		//SDL_UpdateTexture(texture, NULL, pixelsV.data(), pitch);
	}
	/*void applyBlackNWhite() {
		Uint32* pixels = lock();
		for (int i = 0; i < height * width; i++) {
			const Uint8 a = pixels[i];
			const Uint8 r = pixels[i] >> 8;
			const Uint8 g = pixels[i] >> 16;
			const Uint8 b = pixels[i] >> 24;
			const Uint8 val = (r + g + b) / 3;
			pixels[i] = (val << 24) + (val << 16) + (val << 8) + (a); // bgra
		}
		unlock();
	}
	void applySobel() {
		applyBlackNWhite();
		Uint32* pixels = lock();
		Uint32* origin = new Uint32[width * height];
		memcpy(origin, pixels, width * height * sizeof(Uint32));
		for (size_t y = 1; y < height - 1; y++) {
			for (size_t x = 1; x < width - 1; x++) {
				Uint8 magX = 0;// origin[y * width + x] >> 24;
				magX -= origin[(y-1)*width + (x-1)] >> 24;
				//magX += origin[(y-1)*width + (x)];
				magX += origin[(y-1)*width + (x+1)] >> 24;
				magX -= origin[(y)*width + (x-1)] >> 24;
				//magX += origin[(y)*width + (x)];
				magX += origin[(y)*width + (x+1)] >> 24;
				magX -= origin[(y+1)*width + (x-1)] >> 24;
				//magX += origin[(y+1)*width + (x)];
				magX += origin[(y+1)*width + (x+1)] >> 24;
				pixels[y * width + x] = rgbaToUint32((Uint8)magX, (Uint8)magX, (Uint8)magX, 255);
			}
		}
		//memcpy_s(pixels, sizeof(pixels), origin, sizeof(origin));
		delete[] origin;
		unlock();
	}*/
	inline Uint32& operator[] (const unsigned& i) {
		return pixels[i];
	}
	inline const int& getWidth()const { return width; }
	inline const int& getHeight()const { return height; }
	static inline Uint32 rgbaToUint32(const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a) {
		return (b << 24) + (g << 16) + (r << 8) + (a);
	}
	void clearPixels() {
		const Uint32 pixel = rgbaToUint32(170, 170, 255, 255);
		for (auto& p : pixels) {
			p = pixel;
		}
		/*for (int i = 0; i < width * height; i++) {
			pixelsV[i * 4 + 0] = 255;
			pixelsV[i * 4 + 1] = 255;
			pixelsV[i * 4 + 2] = 255;
			pixelsV[i * 4 + 3] = 255;
		}*/
	}
	void renderTexture(SDL_Renderer* renderer, const Point2& topLeft, const unsigned int& width, const unsigned int& height, const int& flip, const double& angle) const
	{
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		SDL_Rect rect_f;
		rect_f.x = 0;
		rect_f.y = 0;
		if (this->width == 0) { rect_f.w = width; }
		else rect_f.w = this->width;
		if (this->height == 0) { rect_f.h = height; }
		else rect_f.h = this->height;

		SDL_Rect dstrect_f{ topLeft.x,topLeft.y,(int)width,(int)height };

		switch (flip) {
		case 0: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_NONE); break;
		case 1: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_HORIZONTAL); break;
		case 2: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_VERTICAL); break;
		default: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_NONE); break;
		}
	}
	private:
	// Bresenham's line drawing algorithm
	inline static void ScanLine(const int& x1, const int& y1, const int& x2, const int& y2,
		std::vector<Point2>& line, const int& maxHeight, const int& maxWidth)
	{
		int dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;
		const int sx = x2 - x1;
		const int sy = y2 - y1;

		if (sx > 0) dx1 = 1;
		else if (sx < 0) dx1 = -1;
		else dx1 = 0;

		if (sy > 0) dy1 = 1;
		else if (sy < 0) dy1 = -1;
		else dy1 = 0;

		m = std::abs(sx);
		n = std::abs(sy);
		dx2 = dx1;
		dy2 = 0;

		if (m < n) {
			m = std::abs(sy);
			n = std::abs(sx);
			dx2 = 0;
			dy2 = dy1;
		}

		x = x1; y = y1;
		cnt = m + 1;
		k = n / 2;

		while (cnt--) {
			if (y >= 0 && y < maxHeight /*&& x >= 0 && x < maxWidth*/) {
				int xtmp;
				if (x < 0)xtmp = 0;
				else if (x >= maxWidth) xtmp = maxWidth - 1;
				else xtmp = x;
				line.emplace_back(xtmp, y);
			}
			k += n;
			if (k < m) {
				x += dx2;
				y += dy2;
			}
			else {
				k -= m;
				x += dx1;
				y += dy1;
			}
		}
	}
	public:
	void drawLine(const GlobalTexture& globalTexture, const Point2& a, const float& adepth, const Point2& b, const float& bdepth, const Color& color) {
		std::vector<Point2> line;
		ScanLine(a.x, a.y, b.x, b.y, line, height, width);
		//float rela = bdepth - adepth;
		for (auto& p : line) {
			/*int l1 = p.distance(a);
			int l2 = p.distance(b);
			float depth = adepth + (rela) / (l1 + l2) * l1;
			if (globalTexture.zbuffer[p.x + width * p.y] < depth) continue;
			globalTexture.zbuffer[p.x + width * p.y] = depth;*/
			pixels[p.x + width * p.y] = (color.b << 24) + (color.g << 16) + (color.r << 8) + (color.a);
			/*pixelsV[width * 4 * p.y + p.x * 4 + 0] = color.a;
			pixelsV[width * 4 * p.y + p.x * 4 + 1] = color.b;
			pixelsV[width * 4 * p.y + p.x * 4 + 2] = color.g;
			pixelsV[width * 4 * p.y + p.x * 4 + 3] = color.r;*/
		}
	}
};

class Bitmap{
public:
	SDL_Surface* surface = nullptr;
	Bitmap() = delete;
	Bitmap(const char* path) {
		if (path == nullptr) { std::cout << "Wrong path name for object of class [Bitmap].\n"; exit(1); }
		SDL_Surface* newsurface = nullptr;
		newsurface = IMG_Load(path);
		if (newsurface == nullptr) { std::cout << "ERROR : [" << path << "] could not be loaded.\n"; exit(1); }
		SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB32); // convert the pixel-format of the surface to ARGB32/ARGB8888
		surface = SDL_ConvertSurface(newsurface, pixelFormat, 0);
		SDL_LockSurface(surface);
		SDL_FreeSurface(newsurface);
		SDL_FreeFormat(pixelFormat);
		const char* error_f = SDL_GetError();
		if (*error_f) { std::cout << "Error occured in Bitmap(): " << error_f << std::endl; }
		if (surface == nullptr) { std::cout << "ERROR : surface conversion.\n"; exit(1); }
	}
	~Bitmap() {
		SDL_UnlockSurface(surface);
		SDL_FreeSurface(surface);
	}
};

class TextureManager {
public:
	// Bresenham's line drawing algorithm
	inline static void ScanLine(const int& x1, const int& y1, const int& x2, const int& y2,
		std::unordered_map<int/*y*/,Point2/*x-min & x-max*/>& contour, const int& maxHeight, const int& maxWidth)
	{
		int dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;
		const int sx = x2 - x1;
		const int sy = y2 - y1;

		if (sx > 0) dx1 = 1;
		else if (sx < 0) dx1 = -1;
		else dx1 = 0;

		if (sy > 0) dy1 = 1;
		else if (sy < 0) dy1 = -1;
		else dy1 = 0;

		m = std::abs(sx);
		n = std::abs(sy);
		dx2 = dx1;
		dy2 = 0;

		if (m < n) {
			m = std::abs(sy);
			n = std::abs(sx);
			dx2 = 0;
			dy2 = dy1;
		}

		x = x1; y = y1;
		cnt = m + 1;
		k = n / 2;
		while (cnt--) {
			if (y >= 0 && y < maxHeight /*&& x >= 0 && x < maxWidth*/) {
				int xtmp;
				if (x < 0)xtmp = 0;
				else if (x >= maxWidth) xtmp = maxWidth - 1;
				else xtmp = x;
				if (contour.count(y) > 0) {
					if (xtmp <= contour[y].x) contour[y].x = xtmp;
					else if (xtmp > contour[y].y) contour[y].y = xtmp;
				}
				else {
					contour.insert(std::pair<const int, Point2>(y, { xtmp,xtmp }));
				}
			}
			k += n;
			if (k < m) {
				x += dx2;
				y += dy2;
			}
			else {
				k -= m;
				x += dx1;
				y += dy1;
			}
		}
	}

	static inline Uint8 concat(const float& a, const Uint8& b) {
		const double res = a + b;
		if (res >= 255)return 255;
		if (res <= 0)return 0;
		return res;
	}
	static inline float edgeFunction(const Vertex& a, const Vertex& b, const Vertex& c) {
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
	}

	// fill triangle with given texture
	// fill triangle with given texture
	static inline void transformv3(const Bitmap& bmp,
		const Point2& dsta, const Point2& dstb, const Point2& dstc,
		const Point2& srca, const Point2& srcb, const Point2& srcc,
		const float& srcadepth, const float& srcbdepth, const float& srccdepth,
		GlobalTexture& globalTexture, const float& light) {
		if (bmp.surface == nullptr)return;
		const Uint32* const srcpixels = (Uint32*)bmp.surface->pixels;
		const int srcw = bmp.surface->w;
		const int srch = bmp.surface->h;
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		const SDL_PixelFormat* srcFormat = bmp.surface->format;
		std::unordered_map<int/*y*/, Point2/*x-min & x-max*/> contour;
		//contour.
		ScanLine(dsta.x, dsta.y, dstb.x, dstb.y, contour, dsth, dstw);
		ScanLine(dstc.x, dstc.y, dstb.x, dstb.y, contour, dsth, dstw);
		ScanLine(dsta.x, dsta.y, dstc.x, dstc.y, contour, dsth, dstw);
		////////////////////////////////////////////

		////////////////////////////////////////////
		// pre calculate values
		float bymincy = dstb.y - dstc.y;
		float cxminbx = dstc.x - dstb.x;
		float axmincx = dsta.x - dstc.x;
		float aymincy = dsta.y - dstc.y;
		float cyminay = dstc.y - dsta.y;
		float divisor = bymincy * axmincx + cxminbx * aymincy;
		bymincy /= divisor;
		cxminbx /= divisor;
		cyminay /= divisor;
		axmincx /= divisor;
		//const float denominator1 = 
		//const float denominator1 =
		// iterate through lines changing y
		for (auto& it : contour) {
			if (it.first < 0 || it.first > dsth)continue;
			// it->first & it->second
			// iterate through pixels from min-x to max-x
			for (int i = it.second.x; i <= it.second.y; i++) {
				if (it.first >= dsth || i >= dstw || it.first < 0 || i < 0)continue;
				const int index = (it.first * dstw) + i;
				// pre calculate some other values
				const float pxmincx = i - dstc.x;
				const float pymincy = it.first - dstc.y;
				// calculate barycentric coordinates
				const float baryA = bymincy * pxmincx + cxminbx * pymincy;
				const float baryB = cyminay * pxmincx + axmincx * pymincy;
				const float baryC = 1.0f - baryA - baryB;
				// get new pixel depth
				const float pixdepth = baryA * srcadepth + baryB * srcbdepth + baryC * srccdepth;
				// calculate point coordinates (with perspective corrected barycentric coordinates)
				Point2 res = { srca.x * baryA + srcb.x * baryB + srcc.x * baryC , srca.y * baryA + srcb.y * baryB + srcc.y * baryC };
				////////////////
				//float z = 1 / (baryA / srcadepth + baryB / srcbdepth + baryC * srccdepth);
				//res = res / z;
				float w_ = (1 / srcadepth) * baryA + (1 / srcbdepth) * baryB + (1 / srccdepth) * baryC;
				float u_ = (srca.x / srcadepth) * baryA + (srcb.x / srcbdepth) * baryB + (srcc.x / srccdepth) * baryC;
				float v_ = (srca.y / srcadepth) * baryA + (srcb.y / srcbdepth) * baryB + (srcc.y / srccdepth) * baryC;
				res = {u_/w_,v_ / w_ };
				////////////////
				/*Point2F resf = { baryA / srcadepth * srca.x + baryB / srcbdepth * srcb.x + baryC / srccdepth * srcc.x ,
					 baryA / srcadepth * srca.y + baryB / srcbdepth * srcb.y + baryC / srccdepth * srcc.y };
				resf.x *= pixdepth;
				resf.y *= pixdepth;
				Point2 res(resf.x, resf.y);*/
				////////////////
				if (res.y < 0 || res.y >= srch || res.x < 0 || res.x >= srcw) continue;
				// get previous pixel depth
				if (globalTexture.zbuffer[index]< pixdepth) continue;
				globalTexture.zbuffer[index] = pixdepth;
				// src pixel position
				const size_t indexsrc = res.y * srcw + res.x;
				// get texture pixel values (dst)
				const Uint8 A = srcpixels[indexsrc];
				const Uint8 R = srcpixels[indexsrc] >> 8;
				const Uint8 G = srcpixels[indexsrc] >> 16;
				const Uint8 B = srcpixels[indexsrc] >> 24;
				/*if (A == 255) {
					//dstpixels[index] = (B << 24) + (G << 16) + (R << 8) + (A);
					//dstpixels[index] = ((int)pixdepth % 255 << 24) + ((int)pixdepth % 255 << 16) + ((int)pixdepth % 255 << 8) + (A);
					dstpixels[index] = (concat(light,B) << 24) + (concat(light, G) << 16) + (concat(light, R) << 8) + (A);
					continue;
				}*/
				// get previous pixel values (src)
				const Uint8 Aprev = globalTexture[index];
				/*if (!Aprev) {
					dstpixels[index] = (concat(light, B) << 24) + (concat(light, G) << 16) + (concat(light, R) << 8) + (A);
					//dstpixels[index] = ((int)pixdepth % 255 << 24) + ((int)pixdepth % 255 << 16) + ((int)pixdepth % 255 << 8) + (A);
					continue;
				}*/
				const Uint8 Rprev = globalTexture[index] >> 8;
				const Uint8 Gprev = globalTexture[index] >> 16;
				const Uint8 Bprev = globalTexture[index] >> 24;
				const float An = (float)A / 255;
				const Uint8 Rres = Rprev + An * (R - Rprev);
				const Uint8 Gres = Gprev + An * (G - Gprev);
				const Uint8 Bres = Bprev + An * (B - Bprev);
				const Uint8 Ares = Aprev + A - Aprev * A;
				globalTexture[index] = (concat(light, Bres) << 24) + (concat(light, Gres) << 16) + (concat(light, Rres) << 8) + (Ares);
				//globalTexture[index] = ((Uint8)pixdepth%255 << 24) + ((Uint8)pixdepth % 255 << 16) + ((Uint8)pixdepth % 255 << 8) + (Ares);
			}
		}
		//const char* error_f = SDL_GetError();
		//if (*error_f) { std::cout << "Error occured in transform(): " << error_f << std::endl; }
	}


	// fill triangle with single color
	static inline void transformv3(const Color& color,
		const Point2& dsta, const Point2& dstb, const Point2& dstc,
		const float& srcadepth, const float& srcbdepth, const float& srccdepth,
		GlobalTexture& globalTexture) {
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		std::unordered_map<int/*y*/, Point2/*x-min & x-max*/> contour;
		//contour.
		ScanLine(dsta.x, dsta.y, dstb.x, dstb.y, contour, dsth, dstw);
		ScanLine(dstc.x, dstc.y, dstb.x, dstb.y, contour, dsth, dstw);
		ScanLine(dsta.x, dsta.y, dstc.x, dstc.y, contour, dsth, dstw);
		const Uint8 R = color.r, G = color.g, B = color.b, A = color.a;
		const float bymincy = dstb.y - dstc.y;
		const float cxminbx = dstc.x - dstb.x;
		const float axmincx = dsta.x - dstc.x;
		const float aymincy = dsta.y - dstc.y;
		const float cyminay = dstc.y - dsta.y;
		const float divisor = bymincy * axmincx + cxminbx * aymincy;
		// iterate through lines changing y
		for (auto& it : contour) {
			// it->first & it->second
			// iterate through pixels from min-x to max-x
			for (size_t i = it.second.x; i <= it.second.y; i++) {
				const size_t index = i + it.first * dstw;
				if (it.first >= dsth || i >= dstw || it.first < 0 || i < 0)continue;
				// pre calculate some other values
				const float pxmincx = i - dstc.x;
				const float pymincy = it.first - dstc.y;
				// determine barycenters
				const float baryA = (bymincy * pxmincx + cxminbx * pymincy) / divisor;
				const float baryB = (cyminay * pxmincx + axmincx * pymincy) / divisor;
				const float baryC = 1 - baryA - baryB;
				// get new pixel depth
				const float pixdepth = baryA * srcadepth + baryB * srcbdepth + baryC * srccdepth;
				// get previous pixel depth
				if (globalTexture.zbuffer[index] < pixdepth) continue;
				globalTexture.zbuffer[index] = pixdepth;
				// test a-value
				/*if (A == 255) {
					dstpixels[index] = (B << 24) + (G << 16) + (R << 8) + (A);
					continue;
				}*/
				// get previous pixel values (src)
				const Uint8 Aprev = globalTexture[index];
				/*if (!Aprev) {
					dstpixels[index] = (B << 24) + (G << 16) + (R << 8) + (A);
					continue;
				}*/
				const Uint8 Rprev = globalTexture[index] >> 8;
				const Uint8 Gprev = globalTexture[index] >> 16;
				const Uint8 Bprev = globalTexture[index] >> 24;
				const float An = (float)A / 255;
				Uint8 Rres = Rprev + An * (R - Rprev);
				Uint8 Gres = Gprev + An * (G - Gprev);
				Uint8 Bres = Bprev + An * (B - Bprev);
				Uint8 Ares = Aprev + A - Aprev * A;

				globalTexture[index] = (Bres << 24) + (Gres << 16) + (Rres << 8) + (Ares);
			}
		}
	}
	inline static const int sign(const Point2& p1, const Point2& p2, const Point2& p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}
	inline static const bool insideTriangle(const Point2& p, const Point2& v1, const Point2& v2, const Point2& v3) {
		const int bary1 = sign(p, v1, v2);
		const int bary2 = sign(p, v2, v3);
		const int bary3 = sign(p, v3, v1);

		const bool has_neg = (bary1 < 0) || (bary2 < 0) || (bary3 < 0);
		const bool has_pos = (bary1 > 0) || (bary2 > 0) || (bary3 > 0);

		return !(has_neg && has_pos);
	}


	// fill triangle with single color
	static inline void transformv4(const Color& color,
		const Point2& dsta, const Point2& dstb, const Point2& dstc,
		const float& srcadepth, const float& srcbdepth, const float& srccdepth,
		GlobalTexture& globalTexture, const float& light) {
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		const Uint8 R = color.r, G = color.g, B = color.b, A = color.a;
		// bounding box + clipping
		Point2 min(Min(dsta.x, Min(dstb.x, dstc.x)), Min(dsta.y, Min(dstb.y, dstc.y)));
		Point2 max(Max(dsta.x, Max(dstb.x, dstc.x)), Max(dsta.y, Max(dstb.y, dstc.y)));
		min.x = Max(min.x, 0);
		min.y = Max(min.y, 0);
		max.x = Min(max.x, dstw - 1);
		max.y = Min(max.y, dsth - 1);
		/*// triangle setup
		int A01 = dsta.y - dstb.y, B01 = dstb.x - dsta.x;
		int A12 = dstb.y - dstc.y, B12 = dstc.x - dstb.x;
		int A20 = dstc.y - dsta.y, B20 = dsta.x - dstc.x;
		// Barycentric coordinates at minX/minY corner
		Point2 p = { min.x, min.y };
		int w0_row = sign(dstb, dstc, p);
		int w1_row = sign(dstc, dsta, p);
		int w2_row = sign(dsta, dstb, p);
		// loop
		for (p.y = min.y; p.y <= max.y; p.y++) {
			// Barycentric coordinates at start of row
			int bary1 = w0_row;
			int bary2 = w1_row;
			int bary3 = w2_row;
			for (p.x = min.x; p.x <= max.x; p.x++) {
				// If p is on or inside all edges, render pixel.
				//const bool has_neg = (bary1 < 0) || (bary2 < 0) || (bary3 < 0);
				//const bool has_pos = (bary1 > 0) || (bary2 > 0) || (bary3 > 0);
				if ((bary1 | bary2 | bary3) >= 0) {
					const int it = p.x + p.y * dstw;
					if (A == 255) {
						dstpixels[it] = (B << 24) + (G << 16) + (R << 8) + (A);
						continue;
					}
					// get previous pixel values (src)
					const Uint8 Aprev = dstpixels[it];
					if (!Aprev) {
						dstpixels[it] = (B << 24) + (G << 16) + (R << 8) + (A);
						continue;
					}
					const Uint8 Rprev = dstpixels[it] >> 8;
					const Uint8 Gprev = dstpixels[it] >> 16;
					const Uint8 Bprev = dstpixels[it] >> 24;
					const float An = (float)A / 255;
					const Uint8 Rres = Rprev + An * (R - Rprev);
					const Uint8 Gres = Gprev + An * (G - Gprev);
					const Uint8 Bres = Bprev + An * (B - Bprev);
					const Uint8 Ares = Aprev + A - Aprev * A;
					dstpixels[it] = (Bres << 24) + (Gres << 16) + (Rres << 8) + (Ares);
				}
				// One step to the right
				bary1 += A12;
				bary2 += A20;
				bary3 += A01;
			}
			// One row step
			w0_row += B12;
			w1_row += B20;
			w2_row += B01;
		}*/
		// pre calculate values
		float bymincy = dstb.y - dstc.y;
		float cxminbx = dstc.x - dstb.x;
		float axmincx = dsta.x - dstc.x;
		float aymincy = dsta.y - dstc.y;
		float cyminay = dstc.y - dsta.y;
		float divisor = bymincy * axmincx + cxminbx * aymincy;
		bymincy /= divisor;
		cxminbx /= divisor;
		cyminay /= divisor;
		axmincx /= divisor;
		for (int y = min.y; y < max.y; y++) {
			const int offset = y * dstw;
			for (int x = min.x; x < max.x; x++) {
				const Point2 p(x, y);
				int bary0 = sign(dstb, dstc, p);
				int bary1 = sign(dstc, dsta, p);
				int bary2 = sign(dsta, dstb, p);
				if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
					// get new pixel depth
					// pre calculate some other values
					const float pxmincx = x - dstc.x;
					const float pymincy = y - dstc.y;
					// calculate barycentric coordinates
					const float baryA = bymincy * pxmincx + cxminbx * pymincy;
					const float baryB = cyminay * pxmincx + axmincx * pymincy;
					const float baryC = 1.0f - baryA - baryB;
					const float pixdepth = baryA * srcadepth + baryB * srcbdepth + baryC * srccdepth;
					// define pos in bitmap
					const int it = x + offset;
					// check with z-buffer
					const Uint8 Aprev = globalTexture[it];
					if (!Aprev) {
						globalTexture[it] = (concat(light, B) << 24) + (concat(light, G) << 16) + (concat(light, R) << 8) + (A);
						continue;
					}
					if (globalTexture.zbuffer[it] < pixdepth/* && Aprev == 255*/) continue;
					globalTexture.zbuffer[it] = pixdepth;
					// color checks
					/*if (A == 255) {
						dstpixels[it] = (concat(light, B) << 24) + (concat(light, G) << 16) + (concat(light, R) << 8) + (A);
						continue;
					}*/
					// get previous pixel values (src)
					const Uint8 Rprev = globalTexture[it] >> 8;
					const Uint8 Gprev = globalTexture[it] >> 16;
					const Uint8 Bprev = globalTexture[it] >> 24;
					const float An = (float)A / 255;
					Uint8 Rres = Rprev + An * (R - Rprev);
					Uint8 Gres = Gprev + An * (G - Gprev);
					Uint8 Bres = Bprev + An * (B - Bprev);
					Uint8 Ares = Aprev + A - Aprev * A;

					globalTexture[it] = (concat(light, Bres) << 24) + (concat(light, Gres) << 16) + (concat(light, Rres) << 8) + (Ares);
				}
			}
		}
	}

	/*void trasnformv5(const Color& color,
		const Point2& dsta, const Point2& dstb, const Point2& dstc,
		const float& srcadepth, const float& srcbdepth, const float& srccdepth,
		GlobalTexture& globalTexture, const float& light)
	{
		Uint32 alpha = color.a + 1;
		Uint32 invAlpha = 257 - alpha;
		Uint32 afg0 = alpha * color.r;
		Uint32 afg1 = alpha * color.g;
		Uint32 afg2 = alpha * color.b;

		for (int y = min.y; y < max.y; y++) {
			const int offset = y * dstw;
			for (int x = min.x; x < max.x; x++) {
				const int start = x + offset;
				const Uint8 Aprev = dstpixels[start];
				if (globalTexture.zbuffer[it] < pixdepth) continue;
				globalTexture.zbuffer[it] = pixdepth;
				const Uint8 Rprev = dstpixels[it] >> 8;
				const Uint8 Gprev = dstpixels[it] >> 16;
				const Uint8 Bprev = dstpixels[it] >> 24;

				bg[0] = cast(ubyte)((afg0 + invAlpha * bg[0]) >> 8);
				bg[1] = cast(ubyte)((afg1 + invAlpha * bg[1]) >> 8);
				bg[2] = cast(ubyte)((afg2 + invAlpha * bg[2]) >> 8);
				bg[3] = 0xff;
			}
		}
	}*/
};