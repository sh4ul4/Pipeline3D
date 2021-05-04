#pragma once
/**
 * @class TextureManager pour encapsuler la partie rastérisation et texture mapping
 */
class TextureManager {

	// le constructeur de base est supprimé car la classe est un singleton
	TextureManager() = delete;

public:

	// rasteriser et colorier un triangle avec une couleur unique tout en respectant la perspective
	static inline void rasterize(const Color& color,
		const Point2D<int>& triA, const Point2D<int>& triB, const Point2D<int>& triC,
		float depthA, float depthB, float depthC,
		GlobalTexture& globalTexture, const float& light) {
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		const Uint8 R = color.r, G = color.g, B = color.b, A = color.a;
		// bounding box + clipping
		Point2D<int> min(std::min(triA.x, std::min(triB.x, triC.x)), std::min(triA.y, std::min(triB.y, triC.y)));
		Point2D<int> max(std::max(triA.x, std::max(triB.x, triC.x)), std::max(triA.y, std::max(triB.y, triC.y)));
		min.x = std::max(min.x, 0);
		min.y = std::max(min.y, 0);
		max.x = std::min(max.x, dstw - 1);
		max.y = std::min(max.y, dsth - 1);
		// pre calculate values
		float bymincy = triB.y - triC.y;
		float cxminbx = triC.x - triB.x;
		float axmincx = triA.x - triC.x;
		float aymincy = triA.y - triC.y;
		float cyminay = triC.y - triA.y;
		float divisor = bymincy * axmincx + cxminbx * aymincy;
		bymincy /= divisor;
		cxminbx /= divisor;
		cyminay /= divisor;
		axmincx /= divisor;

		depthA = 1 / depthA;
		depthB = 1 / depthB;
		depthC = 1 / depthC;

		for (int y = min.y; y < max.y; y++) {
			const int offset = y * dstw;
			for (int x = min.x; x < max.x; x++) {
				const Point2D<int> p(x, y);
				int bary0 = Maths::sign(triB, triC, p);
				int bary1 = Maths::sign(triC, triA, p);
				int bary2 = Maths::sign(triA, triB, p);
				if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
					// pre calculate some other values
					const float pxmincx = x - triC.x;
					const float pymincy = y - triC.y;
					// calculate barycentric coordinates
					const float baryA = bymincy * pxmincx + cxminbx * pymincy;
					const float baryB = cyminay * pxmincx + axmincx * pymincy;
					const float baryC = 1.0f - baryA - baryB;
					// add perspective correction
					float w_ = depthA * baryA + depthB * baryB + depthC * baryC;
					// set new-pixel depth
					const float pixdepth = 1 / w_;
					// define pos in bitmap
					const int it = x + offset;
					// check with z-buffer
					const Uint8 Aprev = globalTexture[it];
					if (!Aprev) {
						globalTexture[it] = (Maths::concat(light, B) << 24) + (Maths::concat(light, G) << 16) + (Maths::concat(light, R) << 8) + (A);
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

					globalTexture[it] = (Maths::concat(light, Bres) << 24) + (Maths::concat(light, Gres) << 16) + (Maths::concat(light, Rres) << 8) + (Ares);
				}
			}
		}
	}

	// rasteriser et colorier un triangle avec une texture tout en respectant la perspective
	static inline void rasterize(const Bitmap& bmp,
		const Point2D<int>& triA, const Point2D<int>& triB, const Point2D<int>& triC,
		const Point2D<int>& bmpA, const Point2D<int>& bmpB, const Point2D<int>& bmpC,
		const float& depthA, const float& depthB, const float& depthC,
		GlobalTexture& globalTexture, const float& light) {
		// setup initial values
		if (bmp.surface == nullptr)return;
		const Uint32* const srcpixels = (Uint32*)bmp.surface->pixels;
		const int srcw = bmp.surface->w;
		const int srch = bmp.surface->h;
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		const SDL_PixelFormat* srcFormat = bmp.surface->format;
		// bounding box + clipping
		Point2D<int> min(std::min(triA.x, std::min(triB.x, triC.x)), std::min(triA.y, std::min(triB.y, triC.y)));
		Point2D<int> max(std::max(triA.x, std::max(triB.x, triC.x)), std::max(triA.y, std::max(triB.y, triC.y)));
		min.x = std::max(min.x, 0);
		min.y = std::max(min.y, 0);
		max.x = std::min(max.x, dstw);
		max.y = std::min(max.y, dsth);
		// pre calculate values
		float bymincy = triB.y - triC.y;
		float cxminbx = triC.x - triB.x;
		float axmincx = triA.x - triC.x;
		float aymincy = triA.y - triC.y;
		float cyminay = triC.y - triA.y;
		float divisor = bymincy * axmincx + cxminbx * aymincy;
		bymincy /= divisor;
		cxminbx /= divisor;
		cyminay /= divisor;
		axmincx /= divisor;
		// pixel mapping loop
		for (int y = min.y; y < max.y; y++) {
			const int offset = y * dstw;
			for (int x = min.x; x < max.x; x++) {
				const Point2D<int> p(x, y);
				int bary0 = Maths::sign(triB, triC, p);
				int bary1 = Maths::sign(triC, triA, p);
				int bary2 = Maths::sign(triA, triB, p);
				if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
					// get new pixel depth
					// pre calculate some other values
					const float pxmincx = x - triC.x;
					const float pymincy = y - triC.y;
					// calculate barycentric coordinates
					const float baryA = bymincy * pxmincx + cxminbx * pymincy;
					const float baryB = cyminay * pxmincx + axmincx * pymincy;
					const float baryC = 1.0f - baryA - baryB;
					// add perspective correction
					float w_ = (1 / depthA) * baryA + (1 / depthB) * baryB + (1 / depthC) * baryC;
					float u_ = (bmpA.x / depthA) * baryA + (bmpB.x / depthB) * baryB + (bmpC.x / depthC) * baryC;
					float v_ = (bmpA.y / depthA) * baryA + (bmpB.y / depthB) * baryB + (bmpC.y / depthC) * baryC;
					// set position of source-pixel and new-pixel depth
					const Point2D<int> res = { u_ / w_,v_ / w_ };
					const float pixdepth = 1 / w_;
					// define pos in bitmap
					const int it = x + offset;
					// clipping check
					if (res.y < 0 || res.y >= srch || res.x < 0 || res.x >= srcw) continue;
					// get previous pixel depth
					if (globalTexture.zbuffer[it] < pixdepth) continue;
					globalTexture.zbuffer[it] = pixdepth;
					// src pixel position
					const size_t indexsrc = (size_t)res.y * (size_t)srcw + (size_t)res.x;
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
					const Uint8 Aprev = globalTexture[it];
					/*if (!Aprev) {
						dstpixels[index] = (concat(light, B) << 24) + (concat(light, G) << 16) + (concat(light, R) << 8) + (A);
						//dstpixels[index] = ((int)pixdepth % 255 << 24) + ((int)pixdepth % 255 << 16) + ((int)pixdepth % 255 << 8) + (A);
						continue;
					}*/
					const Uint8 Rprev = globalTexture[it] >> 8;
					const Uint8 Gprev = globalTexture[it] >> 16;
					const Uint8 Bprev = globalTexture[it] >> 24;
					const float An = (float)A / 255;
					const Uint8 Rres = Rprev + An * (R - Rprev);
					const Uint8 Gres = Gprev + An * (G - Gprev);
					const Uint8 Bres = Bprev + An * (B - Bprev);
					const Uint8 Ares = Aprev + A - Aprev * A;
					globalTexture[it] = (Maths::concat(light, Bres) << 24) + (Maths::concat(light, Gres) << 16) + (Maths::concat(light, Rres) << 8) + (Ares);
					//globalTexture[it] = (concat(light, w_*50) << 24) + (concat(light, w_*50) << 16) + (concat(light, w_*50) << 8) + (255);
				}
			}
		}
	}
};