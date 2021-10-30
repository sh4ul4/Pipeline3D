#pragma once
#ifdef _CUDA

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

__device__
void optimizedProductCuda(float* m1, float* m2, float* res, int i) {
	float a = m1[i * 16 + 0] * m2[0 * 4 + 0] + m1[i * 16 + 1] * m2[1 * 4 + 0] + m1[i * 16 + 2] * m2[2 * 4 + 0] + m2[3 * 4 + 0];
	float b = m1[i * 16 + 0] * m2[0 * 4 + 1] + m1[i * 16 + 1] * m2[1 * 4 + 1] + m1[i * 16 + 2] * m2[2 * 4 + 1] + m2[3 * 4 + 1];
	float c = m1[i * 16 + 0] * m2[0 * 4 + 2] + m1[i * 16 + 1] * m2[1 * 4 + 2] + m1[i * 16 + 2] * m2[2 * 4 + 2] + m2[3 * 4 + 2];
	float d = m1[i * 16 + 0] * m2[0 * 4 + 3] + m1[i * 16 + 1] * m2[1 * 4 + 3] + m1[i * 16 + 2] * m2[2 * 4 + 3] + m2[3 * 4 + 3];
	res[i * 16 + 0] = a;
	res[i * 16 + 1] = b;
	res[i * 16 + 2] = c;
	res[i * 16 + 3] = d;
	
	for (int j = 4; j < 16; j++) {
		res[i * 16 + j] = 0;
	}
}

__device__
float optimizedLengthCuda(float* m, int i) {
	return sqrtf(m[i * 16 + 0] * m[i * 16 + 0] + m[i * 16 + 1] * m[i * 16 + 1] + m[i * 16 + 2] * m[i * 16 + 2]);
}

__global__
void bnw(int n, Uint32* data) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	if (i < n) {
		Uint32 p = data[i];
		Uint8 a = (Uint8)(p >> 24);
		Uint8 r = (Uint8)(p >> 16);
		Uint8 g = (Uint8)(p >> 8);
		Uint8 b = (Uint8)(p);
		//const Uint8 grayscale = ((Uint8)(p >> 8) + (Uint8)(p >> 16) + (Uint8)(p >> 24)) / 3;

		Uint8 rred = (r * .393) + (g * .769) + (b * .189);
		Uint8 rgreen = (r * .349) + (g * .686) + (b * .168);
		Uint8 rblue = (r * .272) + (g * .534) + (b * .131);
		Uint8 ra = a;
		//p = (grayscale << 24) + (grayscale << 16) + (grayscale << 8) + (Uint8)p;
		p = (ra << 24) + (rblue << 16) + (rgreen << 8) + (Uint8)rred;
		data[i] = p;
		i += stride;
	}
}

__global__
void get2dCuda(int n, float* m, bool* clip, int centerx, int centery,
	float far, float* viewMatrix, float* projectionMatrix, float* res) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	while (i < n) {
		clip[i] = false;
		m[i * 16 + 0 * 4 + 3] = 1;
		optimizedProductCuda(m, viewMatrix, m, i);
		// camera space
		if (optimizedLengthCuda(m, i) > far) {
			clip[i] = true;
			res[i * 3 + 0] = (int)m[i * 16 + 0 * 4 + 0];
			res[i * 3 + 1] = (int)m[i * 16 + 0 * 4 + 1];
			res[i * 3 + 2] = 0;
			i += stride;
			continue;
		}
		optimizedProductCuda(m, projectionMatrix, m, i);
		//homogeneous clip space
		int x = (int)(m[i * 16 + 0 * 4 + 0] / m[i * 16 + 0 * 4 + 3]);
		int y = (int)(m[i * 16 + 0 * 4 + 1] / m[i * 16 + 0 * 4 + 3]);
		//NDC space[-1,1]
		x += centerx;
		y += centery;
		//raster space
		res[i * 3 + 0] = x;
		res[i * 3 + 1] = y;
		res[i * 3 + 2] = m[i * 16 + 0 * 4 + 3];
		i += stride;
	}
	__syncthreads();
}

__device__
int sign(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y) {
	//return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	return (p2x - p1x) * (p3y - p1y) - (p2y - p1y) * (p3x - p1x);
}

__device__
Uint8 clamp0_255(int n) {
	int tmp = 255;
	tmp -= n;
	tmp >>= 31;
	tmp |= n;
	n >>= 31;
	n = ~n;
	n &= tmp;
	return n;
}

// concaténer une valeur de pixel et un flottant
__device__
Uint8 concatF(const Uint8& a, const Uint8& b) {
	return clamp0_255(a + b);
}

__device__ float fatomicMin(float* addr, float value)
{
	float old = *addr, assumed;
	if (old <= value) return old;
	while (old != assumed)
	{
		assumed = old;
		old = atomicCAS((unsigned int*)addr, __float_as_int(assumed), __float_as_int(value));
	}
	return old;
}

__global__
void rasterizeCuda(int n, int minx, int miny, int maxx, int maxy,
	int triAx, int triAy, int triBx, int triBy, int triCx, int triCy,
	int bmpAx, int bmpAy, int bmpBx, int bmpBy, int bmpCx, int bmpCy,
	Uint32* globalTexture, Uint32* srcpixels, float* zbuf, int srcw, int dstw, int srch, int dsth,
	float lightIntensity, Uint8 lightColorR, Uint8 lightColorG, Uint8 lightColorB,
	float depthA, float depthB, float depthC, float depthABC) {

	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;

	float bymincy = triBy - triCy;
	float cxminbx = triCx - triBx;
	float axmincx = triAx - triCx;
	float aymincy = triAy - triCy;
	float cyminay = triCy - triAy;
	float divisor = bymincy * axmincx + cxminbx * aymincy;
	bymincy /= divisor;
	cxminbx /= divisor;
	cyminay /= divisor;
	axmincx /= divisor;

	while (i < n) {
		int x = minx + (i % (maxx - minx));
		int y = miny + (i / (maxx - minx));
		if (x > maxx || y > maxy)return;// continue;
		int bary0 = sign(triBx, triBy, triCx, triCy, minx - 1, y) + (x - minx + 1) * -(triCy - triBy) * 1;
		int bary1 = sign(triCx, triCy, triAx, triAy, minx - 1, y) + (x - minx + 1) * -(triAy - triCy) * 1;
		int bary2 = sign(triAx, triAy, triBx, triBy, minx - 1, y) + (x - minx + 1) * -(triBy - triAy) * 1;
		if (!(((bary0 < 0) || (bary1 < 0) || (bary2 < 0)) && ((bary0 > 0) || (bary1 > 0) || (bary2 > 0)))) {
			// pre calculate some other values
			const float pxmincx = x - triCx;
			// calculate barycentric coordinates
			const float baryA = bymincy * pxmincx + cxminbx * (y - triCy);
			const float baryB = cyminay * pxmincx + axmincx * (y - triCy);
			const float baryC = 1.0f - baryA - baryB;
			// add perspective correction
			const float pixdepth = depthABC / (baryA * depthB * depthC + baryB * depthA * depthC + baryC * depthA * depthB);
			const float u_ = (bmpAx / depthA) * baryA + (bmpBx / depthB) * baryB + (bmpCx / depthC) * baryC;
			const float v_ = (bmpAy / depthA) * baryA + (bmpBy / depthB) * baryB + (bmpCy / depthC) * baryC;
			// set position of source-pixel and new-pixel depth
			const int resx = u_ * pixdepth;
			const int resy = v_ * pixdepth;
			// define pos in bitmap
			const int it = x + y * dstw;
			// clipping check & pixel depth check
			fatomicMin(&zbuf[it], pixdepth);/////////////////////////////////////////////////////////////////////////////////////////
			if (zbuf[it] != pixdepth || resy < 0 || resy >= srch || resx < 0 || resx >= srcw) {
				i += stride;
				continue;
			}
			//zbuf[it] = pixdepth;///////////////////////////////////////////////////////////////////////////////////////////////////
			// src pixel position
			const size_t indexsrc = (size_t)resy * (size_t)srcw + (size_t)resx;
			// get texture pixel values (dst)
			const Uint8 A = srcpixels[indexsrc];
			const Uint8 R = srcpixels[indexsrc] >> 8;
			const Uint8 G = srcpixels[indexsrc] >> 16;
			const Uint8 B = srcpixels[indexsrc] >> 24;
			//globalTexture[it] = (B << 24) + (G << 16) + (R << 8) + A;
			//globalTexture[it] = ((B+light) << 24) + ((G + light) << 16) + ((R + light) << 8) + A;
			//__syncthreads();
			globalTexture[it] = (concatF(lightIntensity * lightColorB, B) << 24)
				+ (concatF(lightIntensity * lightColorG, G) << 16)
				+ (concatF(lightIntensity * lightColorR, R) << 8)
				+ (A);
			//globalTexture[it] = (Maths::clamp0_255(255-pixdepth*800) << 24) + (Maths::clamp0_255(255 - pixdepth * 800) << 16) + (Maths::clamp0_255(255 - pixdepth * 800) << 8) + (255);
		}
		i += stride;
	}
}

#endif