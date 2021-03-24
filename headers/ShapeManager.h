#pragma once

#include "Shape.h"

 class ShapeManager {
public:
	// global vector for all the shapes
	std::vector<std::unique_ptr<Shape>> shapes;
	// subject
	int subject = -1;
	void setSubject(const int& val) {
		subject = val;
		if (val >= 0 && val < shapes.size()) {
			Camera::getCurrent().setSubject(shapes[val]->center);
		}
		else Camera::getCurrent().hasSubject = false;
	}
	// add a shape to shapes
	void addShape(const std::vector<Triangle>& triangles, const Vertex& center, Bitmap* bmp = nullptr) {
		shapes.emplace_back(new Shape(triangles, center, bmp));
	}
	void addShape(const Shape& shape) {
		shapes.emplace_back(new Shape(shape));
	}
	void addSphere(const Vertex& center, const double& radius, Bitmap* bmp, const int& precision = 20) {
		shapes.emplace_back(new Sphere(center, radius, bmp, precision));
	}
	void addSphere(const Vertex& center, const double& radius, const Color& color, const int& precision = 20) {
		shapes.emplace_back(new Sphere(center, radius, color, precision));
	}
	void addSphere(const Sphere& sphere) {
		shapes.emplace_back(new Sphere(sphere));
	}
	void addCube(const Vertex& center, const double& width, Bitmap* bmp) {
		shapes.emplace_back(new Cube(center, width, bmp));
	}
	void addCube(const Cube& cube) {
		shapes.emplace_back(new Cube(cube));
	}
	void addRectangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr) {
		shapes.emplace_back(new Rectangle(a, b, c, d, bmp));
	}
	void addRectangle(const Rectangle& rectangle) {
		shapes.emplace_back(new Rectangle(rectangle));
	}
	void set2ds(const Window& window) {
		for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
			for (int j = 0; j < static_cast<int>(shapes[i]->triangles.size()); j++)
				shapes[i]->triangles[j].set2d(window, true);
		}
	}
	bool getFirstInteraction(const Vertex& startingPos, const Vertex& movement, Triangle& intersectionZone, Vertex& intersectionPoint, const float& interactionDistance)const {
		const Vertex goalPos = startingPos + movement;
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i]->triangles.size(); j++) {
				/*if (shapes[i]->triangles[j].a.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].b.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].c.distance(startingPos) > interactionDistance + 10)continue;*/
				const Vertex triangleNormal(Vertex::cross(shapes[i]->triangles[j].b - shapes[i]->triangles[j].a, shapes[i]->triangles[j].c - shapes[i]->triangles[j].a));
				const Vertex planePoint(shapes[i]->triangles[j].a);
				const Vertex tmp(Triangle::getIntersectionWithPlane(startingPos, movement, triangleNormal, planePoint)); // check if there is an intersection between movement-ray and triangle-plane
				if (tmp.getLength() != 0) {
					intersectionPoint = tmp; // copy value (not address)
					if (abs(shapes[i]->triangles[j].distanceToPoint(intersectionPoint)) > interactionDistance)continue; // check if point is on triangle
					if (abs(Vertex::distanceToLine(intersectionPoint, startingPos, goalPos)) > interactionDistance) continue; // check if point is on segment
					intersectionZone = shapes[i]->triangles[j];
					return true;
				}
				else continue; // no intersection
			}
		}
		return false;
	}
};