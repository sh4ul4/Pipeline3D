#pragma once
// Région déclarative contenant des valeurs et des méthodes concernant la physique étudiant le mouvement 
namespace Physics
{
	/* ----- Valeurs correspondant aux lois physiques utilisées ----- */

	Keyboard keyboard;
	bool applyPhysics = false;
	Vertex playerHB = {0, 0, 0};
	float interactionDistance = 0.001;
	float speed = 1;
	float slippiness = 0.1f;
	bool intentionalMoving = false;

	// Valeurs des différentes constantes gravitationnelles
	namespace Gravity
	{
		float gravity = 9.8f;
		bool applyGravity = false;
		int timeFalling = 0;
		float getGravityForce()
		{
			if (applyGravity == false)
				return 0;
			//timeFalling++;
			//return gravity / 50;
			return timeFalling * gravity / 1000;
		}
	}

	/* ----- Méthodes ----- */

	/**
	 * @brief Résoudre les intéractions trouvées entre les formes
	 * 
	 * @param manager ???
	 * @param pos ???
	 * @param step ???
	 * @param speed ???
	 * @param falling ???
	 * @param depth ???
	 */
	void resolvePhysics(ShapeManager& manager, const Vertex& pos, const Vector& step, const float& speed, const bool& falling, const int& depth) {
		if (applyPhysics == false) {
			Camera::getCurrent().moveCameraPreCalculated(step);
			return;
		}
		Triangle triangle;
		Vertex irreal;
		if (manager.getFirstInteraction(pos, step, triangle, irreal, interactionDistance)) {

			if (depth > 0) {
				Gravity::timeFalling = 0;
				return; // too many recursive calls
			}
			const Vertex real(triangle.ClosestPointOnPlane(triangle.a, triangle.b, triangle.c, irreal)); // real is closest point to irreal from triangle
			const Vertex tmp(triangle.ClosestPointOnPlane(triangle.a, triangle.b, triangle.c, pos));

			Vector v1 = pos - tmp;
			//v1.normalizeOnLength(interactionDistance + 0.01);
			//v1.normalizeOnLength(v1.getLength() + 0.01);
			//if (v1.getLength() < interactionDistance)v1.normalizeOnLength(interactionDistance);
			const Vertex corrected(real + v1);
			Vector v2 = corrected - pos;
			v2.normalize();                                                //removed
			if (intentionalMoving == false)v2 *= slippiness;
			//v2.normalizeOnLength(v2.getLength() * slippiness);
			//if (v2.getLength() > speed)v2.normalizeOnLength(speed);
			//v2.normalizeOnLength(speed);
			if (falling) Gravity::timeFalling = 0;
			resolvePhysics(manager, pos, v2, speed, falling, depth + 1);

		}
		else {
			Camera::getCurrent().moveCameraPreCalculated(step);
			Gravity::timeFalling++;
		}
	}

	/**
	 * @brief Appliquer les mouvements sur les formes
	 * 
	 * @param inputEvent ???
	 * @param manager ???
	 */
	void move(InputEvent& inputEvent, ShapeManager& manager) {
		inputEvent.updateKeyBoard(keyboard);
		//if (!Camera::currentExists())return;
		Vector step( 0,0,0 );// -G::getGravityForce()}; // front side up
		if (keyboard.up.pressed) step[0] += speed;
		if (keyboard.down.pressed) step[0] -= speed;
		if (keyboard.left.pressed) step[1] += speed;
		if (keyboard.right.pressed) step[1] -= speed;
		intentionalMoving = abs(step[0]) > 0 || abs(step[1]) > 0 || abs(step[2]) > 0;
		if (keyboard.space.pressed) {
			Gravity::timeFalling = 0;
			step[2] = speed;
		}
		if (keyboard.c.pressed) step[2] -= speed;
		bool falling;
		if (step[1] < 0)falling = true;
		else falling = false;
		Vector stepWithGravity = Camera::getCurrent().getMovementVector(step[0], step[1], step[2], speed);
		stepWithGravity.y -= Gravity::getGravityForce();
		playerHB = { Camera::getCurrent().getSubjectPosition().x, Camera::getCurrent().getSubjectPosition().x /*- 5*/, Camera::getCurrent().getSubjectPosition().z };
		//playerHB = Matrix::toVertex(Camera::getCurrent().pos) + Camera::getCurrent().getMovementVector(1, 0, 0, 10);// { Camera::getCurrent().pos[0], Camera::getCurrent().pos[1]/* - 20*/, Camera::getCurrent().pos[2] };
		resolvePhysics(manager, playerHB, stepWithGravity, speed, falling, 0);
	}
}
