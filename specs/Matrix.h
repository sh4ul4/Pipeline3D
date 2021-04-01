#pragma once

//#include "library.h"
//#include "Window.h"
#include <array>

namespace Matrix {
	/*
	The matrices used are column-strong :
		|m[0][0] | m[1][0]|
		|m[0][1] | m[1][1]|
	*/

	typedef std::array<std::array<float, 4>, 4> m4d; // 4x4 matrix
	typedef std::array<std::array<float, 3>, 3> m3d; // 3x3 matrix
	typedef std::array<std::array<float, 2>, 2> m2d; // 2x2 matrix
	typedef std::array<float, 3> vec3d; // 3x1 matrix / 3D vector
	inline Vertex toVertex(const vec3d& v) {
		return Vertex(v[0],v[1],v[2]);
	}
	inline Matrix::vec3d toVec3d(const Vertex& v) {
		return { v.x,v.y,v.z };
	}

	m4d V; // view-matrix
	m4d P; // projection-matrix
	m4d Vi; // inversed projection-matrix

	// dot-product of two 3D vectors
	inline float dotV3(const vec3d& v1, const vec3d& v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}
	inline float dot2V3(const vec3d& v) { return dotV3(v, v); }

	// length of relevant 3D coordinates in 4x4 matrix
	inline float lengthV(const m4d& m) {
		return sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]);
	}

	// multiply all vector parameters by given float
	inline vec3d mult(const Matrix::vec3d& v, const float& f) {
		return { v[0] * f,v[1] * f,v[2] * f };
	}

	// substract second vector from first vector
	inline vec3d minus(const Matrix::vec3d& v1, const Matrix::vec3d& v2) {
		return { v1[0] - v2[0],v1[1] - v2[1],v1[2] - v2[2] };
	}

	// length of 3D vector
	inline float lengthV(const vec3d& v) {
		return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	// normalize 3D vector on given length
	vec3d normalizeOnLength(const vec3d& v, const float& length) {
		const float sum = abs(v[0]) + abs(v[1]) + abs(v[2]);
		if (sum == 0)return { 0,0,0 };
		const float normalizedLength = length / sum;
		return { v[0] * normalizedLength,v[1] * normalizedLength,v[2] * normalizedLength, };
	}

	// print 4x4 matrix in console
	void print(const m4d& m) {
		
		for (int x = 0; x < m.size(); x++) {
			std::cout << "{ ";
			for (int y = 0; y < m[x].size(); y++) {
				std::cout << m[x][y] << " ";
			}
			std::cout << " }" << std::endl;
		}
	}

	// multiply 2x2 matrix by 2x2 matrix with Strassen-algorithm
	void mult2x2_2x2(const m2d& m1, const m2d& m2, m2d& res) {
		const float p1 = m1[0][0] * (m2[1][0] - m2[1][1]);
		const float p2 = (m1[0][0] + m1[1][0]) * m2[1][1];
		const float p3 = (m1[0][1] + m1[1][1]) * m2[0][0];
		const float p4 = m1[1][1] * (m2[0][1] - m2[0][0]);
		const float p5 = (m1[0][0] + m1[1][1]) * (m2[0][0] + m2[1][1]);
		const float p6 = (m1[1][0] - m1[1][1]) * (m2[0][1] + m2[1][1]);
		const float p7 = (m1[0][0] - m1[0][1]) * (m2[0][0] + m2[1][0]);
		res[0] = { p5 + p4 - p2 + p6,p1 + p2 };
		res[1] = { p3 + p4, p1 + p5 - p3 - p7 };
	}

	// multiply 3x3 matrix by 3D vector
	inline void mult3x3_3x1(const m3d& m, const vec3d& v, Vertex& res) {
		res = {
			 m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
			 m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
			 m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]
		};
	}

	void optmult(const m4d& m1, const m4d& m2, m4d& dest) {
		dest[0] =
		{
			m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m2[3][0],
			m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m2[3][1],
			m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m2[3][2],
			m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m2[3][3]
		};
	}

	// multiply two 4x4 matrices
	void mult(const m4d& m1, const m4d& m2, m4d& dest) {
		/*(*dest)[0][0] = m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0];
		(*dest)[0][1] = m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1];
		(*dest)[0][2] = m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2];
		(*dest)[0][3] = m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3];*/
		/*(*dest)[1][0] = m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0];
		(*dest)[1][1] = m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1];
		(*dest)[1][2] = m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2];
		(*dest)[1][3] = m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3];*/
		//dest->at(0).assign();
		//print(dest);
		dest[0] =
		{
			m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0],
			m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1],
			m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2],
			m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3] 
		};
		/*dest[1] =
		{ m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0],
		m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1],
		m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2],
		m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3] };
		dest[2] =
		{ m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0],
		m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1],
		m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2],
		m1[2][0] * m2[0][3] + m1[2][1] * m2[1][3] + m1[2][2] * m2[2][3] + m1[2][3] * m2[3][3] };
		dest[3] =
		{ m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0],
		m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1],
		m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2],
		m1[3][0] * m2[0][3] + m1[3][1] * m2[1][3] + m1[3][2] * m2[2][3] + m1[3][3] * m2[3][3] };*/
		//print(*dest);
	}

	// multiply two 4x4 matrices with C(Nlog7)
	inline void multAcc(const m4d& m1, const m4d& m2, m4d* dest) {
		// write Strassen algorithm with 2x2 multiplication (above)
	}

	// normalize relevant 3D coordinates of 4x4 matrix
	inline void normalize(m4d& m) {
		m = { {m[0][0] / m[0][3],m[0][1] / m[0][3],m[0][2] / m[0][3],1} };
	}

	// rotate around X-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateX(Vertex& p, const float& angle) {
		m3d a;
		a[0] = { 1,0,0 };
		a[1] = { 0,cos(angle),-sin(angle) };
		a[2] = { 0,sin(angle),cos(angle) };
		mult3x3_3x1(a, { p.x,p.y,p.z }, p);
	}

	// rotate around Y-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateY(Vertex& p, const float& angle) {
		m3d a;
		a[0] = { cos(angle),0,sin(angle) };
		a[1] = { 0,1,0 };
		a[2] = { -sin(angle),0,cos(angle) };
		mult3x3_3x1(a, { p.x,p.y,p.z }, p);
	}

	// rotate around Z-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateZ(Vertex& p, const float& angle) {
		m3d a;
		a[0] = { cos(angle),-sin(angle),0 };
		a[1] = { sin(angle),cos(angle),0 };
		a[2] = { 0,0,1 };
		mult3x3_3x1(a, { p.x,p.y,p.z }, p);
	}

	// make view-matrix for first-person camera
	// row-major
	m4d makeFPviewMatrix(const vec3d& eye, const float& pitch, const float& yaw) {
		const float cosPitch = cos(pitch);
		const float sinPitch = sin(pitch);
		const float cosYaw = cos(yaw);
		const float sinYaw = sin(yaw);

		const vec3d xaxis = { cosYaw, 0, -sinYaw };
		const vec3d yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
		const vec3d zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

		m4d viewMatrix;
		viewMatrix[0] = { xaxis[0], yaxis[0], zaxis[0], 0 };
		viewMatrix[1] = { xaxis[1], yaxis[1], zaxis[1], 0 };
		viewMatrix[2] = { xaxis[2], yaxis[2], zaxis[2], 0 };
		viewMatrix[3] = { -dotV3(xaxis, eye), -dotV3(yaxis, eye), -dotV3(zaxis, eye), 1 };

		return viewMatrix;
	}


	m4d makeProjectionMatrix(const float& fieldOfView, const float& far, const float& near, const float& depthOfField) {
		const float scale = (float)1.0 / (float)tan(fieldOfView / 2.0 * M_PI / 180);
		const float f1 = -far / (far - near);
		const float f2 = f1 * near;
		m4d projectionMatrix;
		projectionMatrix[0] = { scale,0,0,0 };
		projectionMatrix[1] = { 0,scale,0,0 };
		projectionMatrix[2] = { 0,0,f1,depthOfField };
		projectionMatrix[3] = { 0,0,f2,0 };
		return projectionMatrix;
	}

	m4d inverseMatrix(m4d in) {
		float inv[16];
		const float m[16] = { in[0][0],in[1][0],in[2][0],in[3][0],
								in[0][1],in[1][1],in[2][1],in[3][1],
								in[0][2],in[1][2],in[2][2],in[3][2],
								in[0][3],in[1][3],in[2][3],in[3][3] };

		const float m10x15 = m[10] * m[15];
		const float m11x14 = m[11] * m[14];
		const float m6x15 = m[6] * m[15];
		const float m7x14 = m[7] * m[14];
		const float m6x11 = m[6] * m[11];
		const float m7x10 = m[7] * m[10];
		const float m4x9 = m[4] * m[9];
		const float m11x13 = m[11] * m[13];
		const float m8x5 = m[8] * m[5];
		const float m7x13 = m[7] * m[13];
		const float m12x5 = m[12] * m[5];
		const float m7x9 = m[7] * m[9];
		const float m10x13 = m[10] * m[13];
		const float m2x15 = m[2] * m[15];
		const float m3x14 = m[3] * m[14];
		const float m12x2 = m[12] * m[2];
		const float m12x3 = m[12] * m[3];
		const float m6x13 = m[6] * m[13];
		const float m2x11 = m[2] * m[11];
		const float m3x10 = m[3] * m[10];
		const float m0x9 = m[0] * m[9];
		const float m8x1 = m[8] * m[1];
		const float m8x3 = m[8] * m[3];
		const float m12x1 = m[12] * m[1];
		const float m8x2 = m[8] * m[2];
		const float m2x7 = m[2] * m[7];
		const float m3x6 = m[3] * m[6];
		const float m0x5 = m[0] * m[5];
		const float m4x1 = m[4] * m[1];
		const float m4x3 = m[4] * m[3];
		const float m4x2 = m[4] * m[2];

		inv[0] = m[5] * m10x15 + m[9] * m7x14 + m[13] * m6x11 - (m[5] * m11x14 + m[9] * m6x15 + m[13] * m7x10);
		inv[4] = m[12] * m7x10 + m[4] * m11x14 + m[8] * m6x15 - (m[8] * m7x14 + m[12] * m6x11 + m[4] * m10x15);
		inv[8] = m4x9 * m[15] + m[8] * m7x13 + m12x5 * m[11] - (m[12] * m7x9 + m[4] * m11x13 + m8x5 * m[15]);
		inv[12] = m[4] * m10x13 + m8x5 * m[14] + m[12] * m[6] * m[9] - (m4x9 * m[14] + m[8] * m6x13 + m12x5 * m[10]);
		inv[1] = m[1] * m11x14 + m[9] * m2x15 + m[13] * m3x10 - (m[1] * m10x15 + m[9] * m3x14 + m[13] * m2x11);
		inv[5] = m[0] * m10x15 + m[8] * m3x14 + m12x2 * m[11] - (m12x3 * m[10] + m[0] * m11x14 + m[8] * m2x15);
		inv[9] = m[0] * m11x13 + m8x1 * m[15] + m12x3 * m[9] - (m8x3 * m[13] + m12x1 * m[11] + m0x9 * m[15]);
		inv[13] = m0x9 * m[14] + m8x2 * m[13] + m12x1 * m[10] - (m12x2 * m[9] + m[0] * m10x13 + m8x1 * m[14]);
		inv[2] = m[1] * m6x15 + m[5] * m3x14 + m[13] * m2x7 - (m[13] * m3x6 + m[1] * m7x14 + m[5] * m2x15);
		inv[6] = m12x3 * m[6] + m[0] * m7x14 + m[4] * m2x15 - (m[4] * m3x14 + m12x2 * m[7] + m[0] * m6x15);
		inv[10] = m0x5 * m[15] + m4x3 * m[13] + m12x1 * m[7] - (m12x3 * m[5] + m[0] * m7x13 + m4x1 * m[15]);
		inv[14] = m12x2 * m[5] + m[0] * m6x13 + m4x1 * m[14] - (m4x2 * m[13] + m12x1 * m[6] + m0x5 * m[14]);
		inv[3] = m[9] * m3x6 + m[1] * m7x10 + m[5] * m2x11 - (m[5] * m3x10 + m[9] * m2x7 + m[1] * m6x11);
		inv[7] = m[0] * m6x11 + m[4] * m3x10 + m8x2 * m[7] - (m8x3 * m[6] + m[0] * m7x10 + m[4] * m2x11);
		inv[11] = m[0] * m7x9 + m4x1 * m[11] + m8x3 * m[5] - (m8x1 * m[7] + m0x5 * m[11] + m4x3 * m[9]);
		inv[15] = m0x5 * m[10] + m4x2 * m[9] + m8x1 * m[6] - (m8x2 * m[5] + m[0] * m[6] * m[9] + m4x1 * m[10]);

		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return Matrix::m4d({});

		det = 1.0 / det;

		for (int i = 0; i < 16; i++)
			inv[i] *= det;

		for (int i = 0; i < 16; i++) {
			in[(int)(i / 4)][(int)(i % 4)] = m[i]; // transposition
		}
		return in;
	}

	inline vec3d cross(const vec3d& v1, const vec3d& v2) {
		return {
			v1[1] * v2[2] - v1[2] * v2[1],
			v1[2] * v2[0] - v1[0] * v2[2],
			v1[0] * v2[1] - v1[1] * v2[0]
		};
	}

	inline void makeWorldToCameraMatrix(void) {
		Vi = inverseMatrix(V);
	}
}
