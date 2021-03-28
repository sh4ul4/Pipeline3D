#pragma once

//#include "library.h"
#include "ShapeManager.h"

namespace Physics
{
	bool applyPhysics = true;
	Vertex playerHB = { 0,0,0 };
	float interactionDistance = 0.001;// 2;
	float speed = 1;
	float slippiness = 0.1f;
	bool intentionalMoving = false;
	namespace G {
		float gravity = 9.8f;
		bool applyGravity = false;
		int timeFalling = 0;
		float getGravityForce() {
			if (applyGravity == false) return 0;
			//timeFalling++;
			//return gravity / 50;
			return timeFalling * gravity / 1000;
		}
	}
	void resolvePhysics(const ShapeManager& manager, const Vertex& pos, const Vertex& step, const float& speed, const bool& falling, const int& depth) {
		if (applyPhysics == false) {
			Camera::getCurrent().moveCameraPreCalculated(step);
			if(Camera::getCurrent().hasSubject)manager.shapes[manager.subject]->move(step);
			return;
		}
		Triangle triangle;
		Vertex irreal;
		if (manager.getFirstInteraction(pos, step, triangle, irreal, interactionDistance)) {
			
			if (depth > 0) {
				G::timeFalling = 0;
				return; // too many recursive calls
			}
			const Vertex real(triangle.ClosestPointOnPlane(triangle.a, triangle.b, triangle.c,irreal)); // real is closest point to irreal from triangle
			const Vertex tmp(triangle.ClosestPointOnPlane(triangle.a, triangle.b, triangle.c, pos));

			Vertex v1 = pos - tmp;
			//v1.normalizeOnLength(interactionDistance + 0.01);
			//v1.normalizeOnLength(v1.getLength() + 0.01);
			//if (v1.getLength() < interactionDistance)v1.normalizeOnLength(interactionDistance);
			const Vertex corrected(real + v1);
			Vertex v2 = corrected - pos;
			v2.normalize();                                                //removed
			if(intentionalMoving == false)v2 *= slippiness;
			//v2.normalizeOnLength(v2.getLength() * slippiness);
			//if (v2.getLength() > speed)v2.normalizeOnLength(speed);
			//v2.normalizeOnLength(speed);
			if (falling) G::timeFalling = 0;
			resolvePhysics(manager, pos, v2, speed, falling, depth+1);
			
		}
		else {
			Camera::getCurrent().moveCameraPreCalculated(step);
			if (Camera::getCurrent().hasSubject)manager.shapes[manager.subject]->move(step);
			G::timeFalling++;
		}
	}
	void move(InputEvent& inputEvent, const ShapeManager& manager) {
		//if (!Camera::currentExists())return;
		Matrix::vec3d step = { 0,0,0 };// -G::getGravityForce()}; // front side up
		if (inputEvent.keyboard.up) step[0] += speed;
		if (inputEvent.keyboard.down) step[0] -= speed;
		if (inputEvent.keyboard.left) step[1] += speed;
		if (inputEvent.keyboard.right) step[1] -= speed;
		intentionalMoving = abs(step[0]) > 0 || abs(step[1]) > 0 || abs(step[2]) > 0;
		if (inputEvent.keyboard.space) {
			G::timeFalling = 0;
			step[2] = speed;
		}
		if (inputEvent.keyboard.c) step[2] -= speed;
		bool falling;
		if (step[1] < 0)falling = true;
		else falling = false;
		Vertex stepWithGravity = Camera::getCurrent().getMovementVector(step[0], step[1], step[2], speed);
		stepWithGravity.y -= G::getGravityForce();
		playerHB = { Camera::getCurrent().getSubjectPosition()[0], Camera::getCurrent().getSubjectPosition()[1] /*- 5*/, Camera::getCurrent().getSubjectPosition()[2] };
		//playerHB = Matrix::toVertex(Camera::getCurrent().pos) + Camera::getCurrent().getMovementVector(1, 0, 0, 10);// { Camera::getCurrent().pos[0], Camera::getCurrent().pos[1]/* - 20*/, Camera::getCurrent().pos[2] };
		resolvePhysics(manager, playerHB,stepWithGravity,speed,falling,0);
	}
}