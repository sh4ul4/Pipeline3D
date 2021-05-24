#pragma once

namespace Cinematic
{
	float time = 500;						 // Repr�sente le temps qui s'�coule dans le syst�me stellaire
	const float G = 6.67 * pow(10, -11); // Contante de gravitation

	/* ----- CALCULS MATHEMATIQUES ----- */

	/**
	 * @brief Calcul le vecteur AB, dont la norme est la distance entre le point A et B, et la direction, celle du segment AB
	 *
	 * @param p1 Le premier point
	 * @param p2 Le deuxi�me point
	 * @return Vecteur p1p2 avec ses coordonn�es x et y
	 */
	Point2D<double> vectorPoints(const Point2D<double>& p1, const Point2D<double>& p2)
	{
		Point2D<double> vector;
		vector.x = p2.x - p1.x;
		vector.y = p2.y - p1.y;

		return vector;
	}

	/**
	 * @brief Calcul le vecteur unitaire du vecteur en argument
	 *
	 * @param vector vecteur dont on veut le vecteur unitaire
	 * @return Vecteur unitaire
	 */
	Point2D<double> getUnitVector(const Point2D<double>& vector)
	{
		Point2D<double> unitVector;
		double norm = vector.distance({ 0, 0 }); // Norme du vecteur
		unitVector.x = vector.x / norm;
		unitVector.y = vector.y / norm;

		// Règle le problème du zéro négatif
		if (unitVector.x < 0.00001 && unitVector.x > -0.00001)
			unitVector.x = 0;
		if (unitVector.y < 0.00001 && unitVector.y > -0.00001)
			unitVector.y = 0;

		return unitVector;
	}

	/* ----- FORMULES PHYSIQUES ----- */

	/**
	 * @brief Calcul la force d'attraction entre deux astres
	 *
	 * @param l1 Premier astre
	 * @param l2 Deuxi�me astre
	 * @return Vecteur repr�sentant le force exerc� de l1 sur l2
	 */
	Point2D<double> attractionForce(Luminary* l1, Luminary* l2)
	{
		Point2D<double> force, vector, unitVector;
		vector = vectorPoints(l1->getPosition(), l2->getPosition());
		unitVector = getUnitVector(vector);
		const double denominator = pow(vector.distance({ 0, 0 }), 2);
		//printf("distance = %f\n", vector.distance({ 0, 0 }));
		const double value = (G * l1->getMass() * l2->getMass()) / denominator;
		force.x = value * unitVector.x;
		force.y = value * unitVector.y;
		return force;
	}
	/**
	 * @brief Calcul l'acc�l�ration d'un astre en fonction des forces exerc�es sur celui-ci
	 *
	 * @param force La somme des forces exerc�es sur l'astre l
	 * @param p La plan�te sur laquelle les forces sont exerc�es. L'attribut 'acceleration' est mis � jour
	 */
	void Newton2nd(const Point2D<double>& force, Planet* p)
	{
		const double mass = p->getMass();
		const double x = force.x / mass;
		const double y = force.y / mass;
		p->setAcceleration({ x, y });
	}

	/**
	 * @brief Calcul le vecteur vitesse d'un astre � partir de son acceleration
	 *
	 * @param p La plan�te concern�e. l'attribut 'speed' est mis � jour
	 */
	void getSpeedFromAcceleration(Planet* p)
	{
		Point2D<double> acceleration = p->getAcceleration();
		Point2D<double> initialSpeed = p->getInitialSpeed();

		double x = (acceleration.x * time) + initialSpeed.x;
		double y = (acceleration.y * time) + initialSpeed.y;
		p->setSpeed({ x, y });
	}

	/**
	 * @brief Calcul la position de l'astre
	 *
	 * @param p La plan�te concern�e. l'attribut 'position' est mis � jour
	 */
	void getPositionFromSpeed(Planet* p)
	{
		Point2D<double> initialSpeed = p->getInitialSpeed();
		Point2D<double> initialPosition = p->getInitialPosition();
		Point2D<double> speed = p->getSpeed();
		
		double x = ((speed.x / 2) * pow(time, 2)) + (initialSpeed.x * time) + initialPosition.x;
		double y = ((speed.y / 2) * pow(time, 2)) + (initialSpeed.y * time) + initialPosition.y;
		Point2D<double> p_set(x, y);
		p->setPosition(p_set);	//Erreur : non - const lvalue reference to type 'Point2D<double>' cannot bind to an initializer list temporary
	}
}