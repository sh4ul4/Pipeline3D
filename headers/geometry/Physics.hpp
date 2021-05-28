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
			const Vertex real(Maths::ClosestPointOnPlane(triangle.a, triangle.b, triangle.c, irreal)); // real is closest point to irreal from triangle
			const Vertex tmp(Maths::ClosestPointOnPlane(triangle.a, triangle.b, triangle.c, pos));

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
			resolvePhysics(manager, pos, v2, speed, falling, depth + 1);

		}
		else {
			Camera::getCurrent().moveCameraPreCalculated(step);
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
		intentionalMoving = step[0] != 0 || step[1] != 0 || step[2] != 0;
		if (keyboard.space.pressed) {
			step[2] = speed;
		}
		if (keyboard.c.pressed) step[2] -= speed;
		bool falling;
		if (step[1] < 0)falling = true;
		else falling = false;
		Vector stepWithGravity = Camera::getCurrent().getMovementVector(step[0], step[1], step[2], speed);
		Vertex subject = Camera::getCurrent().getSubjectPosition();
		playerHB = { subject.x, subject.y, subject.z };
		resolvePhysics(manager, playerHB, stepWithGravity, speed, falling, 0);
	}
}
