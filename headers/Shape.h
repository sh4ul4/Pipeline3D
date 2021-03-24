#pragma once

//#include "Texture.h"
#include "Triangle.h"
//#include "Draw.h"
//#include "library.h"

class Shape {
public:
	Shape(){}
	Shape(const std::vector<Triangle>& triangles, const Vertex& center, Bitmap* bmp = nullptr) :triangles(triangles), center(center), bmp(bmp) {}
	Shape(const Shape& shape):triangles(shape.triangles),center(shape.center),visible(shape.visible),bmp(shape.bmp) {}
	std::vector<Triangle> triangles;
	Vertex center;
	bool visible = true;
	Bitmap* bmp = nullptr;

	void rotateX(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Matrix::rotateX(triangles[i].a, angle);
			Matrix::rotateX(triangles[i].b, angle);
			Matrix::rotateX(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Matrix::rotateX(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}
	void rotateY(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Matrix::rotateY(triangles[i].a, angle);
			Matrix::rotateY(triangles[i].b, angle);
			Matrix::rotateY(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Matrix::rotateY(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}
	void rotateZ(const Vertex& p, const float& angle) {
		for (int i = 0; i < triangles.size(); i++) {
			triangles[i].a -= p;
			triangles[i].b -= p;
			triangles[i].c -= p;
			Matrix::rotateZ(triangles[i].a, angle);
			Matrix::rotateZ(triangles[i].b, angle);
			Matrix::rotateZ(triangles[i].c, angle);
			triangles[i].a += p;
			triangles[i].b += p;
			triangles[i].c += p;
		}
		center.x -= p.x; center.y -= p.y; center.z -= p.z;
		Matrix::rotateZ(center, angle);
		center.x += p.x; center.y += p.y; center.z += p.z;
	}
	void move(const Vertex& movement) {
		for (Triangle& t : triangles)t.move(movement);
	}
};

class Sphere : public Shape {
public:
	Sphere(const Vertex& center, const double& radius, Bitmap* bmp, const int& precision = 20) :Shape({}, center), radius(radius) {
		//int precision = precision;
		const float deltaTheta = M_PI / precision;
		const float deltaPhi = 2 * M_PI / precision;
		float theta = 0;
		float phi = 0;
		std::vector<Vertex> ps;
		ps.push_back(Vertex( 0, 0, static_cast<float>(radius))); //north pole end cap
		for (int ring = 0; ring < precision; ring++) { //move to a new z - offset
			theta += deltaTheta;
			for (int point = 0; point < precision; point++) { // draw a ring
				phi += deltaPhi;
				float x = sin(theta) * cos(phi) * radius;
				float y = sin(theta) * sin(phi) * radius;
				float z = cos(theta) * radius;
				ps.push_back({ x, y, z });
			}
		}
		ps.push_back({ 0, 0, static_cast<float>(-radius) });
		for (size_t i = 2; i < precision && i < ps.size(); i += 2) {
			//triangles.push_back({ ps[static_cast<unsigned>(i) - 2],ps[static_cast<unsigned>(i) - 1],ps[0] });
			triangles.push_back(Triangle(ps[i - 2], ps[i - 1], ps[0], black, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
			triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = precision; i < ps.size(); i++) {
			triangles.push_back({ ps[i],ps[(int)(i - 1)],ps[(int)(i - precision)],black, bmp,{ 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h } });
			triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = 0; i < ps.size() && i < triangles.size(); i++) {
			triangles[i].a.x += center.x;
			triangles[i].b.x += center.x;
			triangles[i].c.x += center.x;

			triangles[i].a.y += center.y;
			triangles[i].b.y += center.y;
			triangles[i].c.y += center.y;

			triangles[i].a.z += center.z;
			triangles[i].b.z += center.z;
			triangles[i].c.z += center.z;
		}
	}
	Sphere(const Vertex& center, const double& radius, const Color& color, const int& precision = 20) :Shape({}, center), radius(radius) {
		//int precision = precision;
		const float deltaTheta = M_PI / precision;
		const float deltaPhi = 2 * M_PI / precision;
		float theta = 0;
		float phi = 0;
		std::vector<Vertex> ps;
		ps.push_back(Vertex(0, 0, static_cast<float>(radius))); //north pole end cap
		for (int ring = 0; ring < precision; ring++) { //move to a new z - offset
			theta += deltaTheta;
			for (int point = 0; point < precision; point++) { // draw a ring
				phi += deltaPhi;
				float x = sin(theta) * cos(phi) * radius;
				float y = sin(theta) * sin(phi) * radius;
				float z = cos(theta) * radius;
				ps.push_back({ x, y, z });
			}
		}
		ps.push_back({ 0, 0, static_cast<float>(-radius) });
		for (size_t i = 2; i < precision && i < ps.size(); i += 2) {
			//triangles.push_back({ ps[static_cast<unsigned>(i) - 2],ps[static_cast<unsigned>(i) - 1],ps[0] });
			triangles.push_back(Triangle(ps[i - 2], ps[i - 1], ps[0], color, false));
			//triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = precision; i < ps.size(); i++) {
			triangles.push_back(Triangle(ps[i],ps[(int)(i - 1)],ps[(int)(i - precision)],color, false));
			//triangles[triangles.size() - 1].fillIt(1);
		}
		for (int i = 0; i < ps.size() && i < triangles.size(); i++) {
			triangles[i].a.x += center.x;
			triangles[i].b.x += center.x;
			triangles[i].c.x += center.x;

			triangles[i].a.y += center.y;
			triangles[i].b.y += center.y;
			triangles[i].c.y += center.y;

			triangles[i].a.z += center.z;
			triangles[i].b.z += center.z;
			triangles[i].c.z += center.z;
		}
	}
	Sphere(const Sphere& sphere) :Shape(sphere), radius(sphere.radius) {}
	int radius;
};

class Cube : public Shape {
public:
	Cube(const Vertex& center, const double& width, Bitmap* bmp = nullptr) :Shape({}, center, bmp), width(width) {
		const int half_width = static_cast<int>(width) / 2;
		// set 8 points
		const Vertex a(center.x + half_width, center.y - half_width, center.z - half_width);
		const Vertex b(center.x + half_width, center.y + half_width, center.z - half_width);
		const Vertex c(center.x + half_width, center.y + half_width, center.z + half_width);
		const Vertex d(center.x + half_width, center.y - half_width, center.z + half_width);
		const Vertex e(center.x - half_width, center.y - half_width, center.z - half_width);
		const Vertex f(center.x - half_width, center.y + half_width, center.z - half_width);
		const Vertex g(center.x - half_width, center.y + half_width, center.z + half_width);
		const Vertex h(center.x - half_width, center.y - half_width, center.z + half_width);
		// set 12 triangles
		triangles.push_back(Triangle(a, b, d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = blue; triangles.back().fillIt(true);
		triangles.push_back(Triangle(c,b,d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = red; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,c, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = green; triangles.back().fillIt(true);
		triangles.push_back(Triangle(b,g,f, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = white; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,c, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = black; triangles.back().fillIt(true);
		triangles.push_back(Triangle(d,g,h, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = yellow; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,h, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = cyan; triangles.back().fillIt(true);
		triangles.push_back(Triangle(e,g,f, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,d, blue,bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = dark_gray; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,h,e, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = lime; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,e, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = maroon; triangles.back().fillIt(true);
		triangles.push_back(Triangle(a,f,b, blue, bmp, { 0,bmp->surface->h }, { bmp->surface->w,0 }, { 0,bmp->surface->h }));
		triangles.back().color = olive; triangles.back().fillIt(true);
		for (int i = 0; i < static_cast<int>(triangles.size()); i++) {
			triangles[i].fillIt(true);
		}
	}
	Cube(const Cube& cube) :Shape(cube), width(cube.width) {}
	int width;
};

class Rectangle : public Shape {
public:
	Rectangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr) {
		if (bmp == nullptr) {
			this->center = a + b + c + d;
			this->center /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c));
			triangles.push_back(Triangle(c, b, d));
			triangles[0].fillIt(0);
			triangles[0].contourIt(1);
			triangles[1].fillIt(0);
			triangles[1].contourIt(1);
		}
		else {
			this->center = a + b + c + d;
			this->center /= 4;
			// set 2 triangles
			triangles.push_back(Triangle(a, b, c, black, bmp, { 0,bmp->surface->h }, { bmp->surface->w,bmp->surface->h }, { 0,0 }));
			triangles.push_back(Triangle(c, b, d, black, bmp, { 0,0 } ,{ bmp->surface->w,bmp->surface->h }, { bmp->surface->w,0 }));
			triangles[0].fillIt(1);
			triangles[0].contourIt(0);
			triangles[1].fillIt(1);
			triangles[1].contourIt(0);
		}
	}
	Rectangle(const Rectangle& rectangle) :Shape(rectangle) {}
};