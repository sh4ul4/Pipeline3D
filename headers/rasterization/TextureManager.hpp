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
		GlobalTexture& globalTexture,
		const float lightIntensity, const Color& lightColor) {
		const Uint8 R = color.r, G = color.g, B = color.b, A = color.a;
		// setup initial values
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		// bounding box + clipping
		Point2D<int> min(std::min(triA.x, std::min(triB.x, triC.x)), std::min(triA.y, std::min(triB.y, triC.y)));
		Point2D<int> max(std::max(triA.x, std::max(triB.x, triC.x)), std::max(triA.y, std::max(triB.y, triC.y)));
		min.x = std::max(min.x, 0);
		min.y = std::max(min.y, 0);
		max.x = std::min(max.x, dstw);
		max.y = std::min(max.y, dsth);
		// pre calculate values
		const float depthABC = depthA * depthB * depthC;
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
			// pre calculate some other values
			const int offset = y * dstw;
			const float pymincy = y - triC.y;
			// optimization test
			int bary0 = Maths::sign(triB, triC, Point2D<int>(min.x - 1, y));
			const int bary0step = -(triC.y - triB.y) * 1;
			int bary1 = Maths::sign(triC, triA, Point2D<int>(min.x - 1, y));
			const int bary1step = -(triA.y - triC.y) * 1;
			int bary2 = Maths::sign(triA, triB, Point2D<int>(min.x - 1, y));
			const int bary2step = -(triB.y - triA.y) * 1;
			for (int x = min.x; x < max.x; x++) {
				bary0 += bary0step;
				bary1 += bary1step;
				bary2 += bary2step;
				if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
					// pre calculate some other values
					const float pxmincx = x - triC.x;
					// calculate barycentric coordinates
					const float baryA = bymincy * pxmincx + cxminbx * pymincy;
					const float baryB = cyminay * pxmincx + axmincx * pymincy;
					const float baryC = 1.0f - baryA - baryB;
					// add perspective correction
					const float pixdepth = depthABC / (baryA * depthB * depthC + baryB * depthA * depthC + baryC * depthA * depthB);
					// define pos in bitmap
					const int it = x + offset;
					// clipping check & pixel depth check
					if (globalTexture.zbuffer[it] <= pixdepth)continue;
					globalTexture.zbuffer[it] = pixdepth;
					//globalTexture[it] = (B << 24) + (G << 16) + (R << 8) + A;
					//globalTexture[it] = ((B+light) << 24) + ((G + light) << 16) + ((R + light) << 8) + A;
					globalTexture[it] = (Maths::concatF(lightIntensity * lightColor.b, B) << 24)
						+ (Maths::concatF(lightIntensity * lightColor.g, G) << 16)
						+ (Maths::concatF(lightIntensity * lightColor.r, R) << 8)
						+ (A);
					//globalTexture[it] = (Maths::clamp0_255(255-pixdepth*800) << 24) + (Maths::clamp0_255(255 - pixdepth * 800) << 16) + (Maths::clamp0_255(255 - pixdepth * 800) << 8) + (255);
				}
			}
		}
	}

	// rasteriser et colorier un triangle avec une texture tout en respectant la perspective
	static inline void rasterize(const Bitmap& bmp,
		const Point2D<int>& triA, const Point2D<int>& triB, const Point2D<int>& triC,
		const Point2D<int>& bmpA, const Point2D<int>& bmpB, const Point2D<int>& bmpC,
		const float& depthA, const float& depthB, const float& depthC,
		GlobalTexture& globalTexture, const float lightIntensity, const Color& lightColor) {
		// setup initial values
		const Uint32* const srcpixels = (Uint32*)bmp.surface->pixels;
		const int srcw = bmp.surface->w;
		const int srch = bmp.surface->h;
		const int dstw = globalTexture.getWidth();
		const int dsth = globalTexture.getHeight();
		// bounding box + clipping
		Point2D<int> min(std::min(triA.x, std::min(triB.x, triC.x)), std::min(triA.y, std::min(triB.y, triC.y)));
		Point2D<int> max(std::max(triA.x, std::max(triB.x, triC.x)), std::max(triA.y, std::max(triB.y, triC.y)));
		min.x = std::max(min.x, 0);
		min.y = std::max(min.y, 0);
		max.x = std::min(max.x, dstw);
		max.y = std::min(max.y, dsth);
		// pre calculate values
		const float depthABC = depthA * depthB * depthC;
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
		for (int y = min.y; y <= max.y; y++) {
			// pre calculate some other values
			const int offset = y * dstw;
			const float pymincy = y - triC.y;
			// optimization test
			int bary0 = Maths::sign(triB, triC, Point2D<int>(min.x - 1, y));
			const int bary0step = -(triC.y - triB.y) * 1;
			int bary1 = Maths::sign(triC, triA, Point2D<int>(min.x - 1, y));
			const int bary1step = -(triA.y - triC.y) * 1;
			int bary2 = Maths::sign(triA, triB, Point2D<int>(min.x - 1, y));
			const int bary2step = -(triB.y - triA.y) * 1;
			for (int x = min.x; x <= max.x; x++) {
				bary0 += bary0step;
				bary1 += bary1step;
				bary2 += bary2step;
				if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
					// pre calculate some other values
					const float pxmincx = x - triC.x;
					// calculate barycentric coordinates
					const float baryA = bymincy * pxmincx + cxminbx * pymincy;
					const float baryB = cyminay * pxmincx + axmincx * pymincy;
					const float baryC = 1.0f - baryA - baryB;
					// add perspective correction
					const float pixdepth = depthABC / (baryA*depthB*depthC + baryB*depthA*depthC + baryC*depthA*depthB);
					const float u_ = (bmpA.x / depthA) * baryA + (bmpB.x / depthB) * baryB + (bmpC.x / depthC) * baryC;
					const float v_ = (bmpA.y / depthA) * baryA + (bmpB.y / depthB) * baryB + (bmpC.y / depthC) * baryC;
					// set position of source-pixel and new-pixel depth
					const Point2D<int> res = { u_ * pixdepth,v_ * pixdepth };
					// define pos in bitmap
					const int it = x + offset;
					// clipping check & pixel depth check
					if (globalTexture.zbuffer[it] <= pixdepth || res.y < 0 || res.y >= srch || res.x < 0 || res.x >= srcw)continue;
					globalTexture.zbuffer[it] = pixdepth;
					// src pixel position
					const size_t indexsrc = (size_t)res.y * (size_t)srcw + (size_t)res.x;
					// get texture pixel values (dst)
					const Uint8 A = srcpixels[indexsrc];
					const Uint8 R = srcpixels[indexsrc] >> 8;
					const Uint8 G = srcpixels[indexsrc] >> 16;
					const Uint8 B = srcpixels[indexsrc] >> 24;
					//globalTexture[it] = (B << 24) + (G << 16) + (R << 8) + A;
					//globalTexture[it] = ((B+light) << 24) + ((G + light) << 16) + ((R + light) << 8) + A;
					globalTexture[it] = (Maths::concatF(lightIntensity * lightColor.b, B) << 24)
						+ (Maths::concatF(lightIntensity * lightColor.g, G) << 16)
						+ (Maths::concatF(lightIntensity * lightColor.r, R) << 8)
						+ (A);
					//globalTexture[it] = (Maths::clamp0_255(255-pixdepth*800) << 24) + (Maths::clamp0_255(255 - pixdepth * 800) << 16) + (Maths::clamp0_255(255 - pixdepth * 800) << 8) + (255);
				}
			}
		}
	}
};