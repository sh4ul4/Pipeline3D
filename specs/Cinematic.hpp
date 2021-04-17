namespace Cinematic
{
	float time = 0;						 // Représente le temps qui s'écoule dans le système stellaire
	const float G = 6.67 * pow(10, -11); // Contante de gravitation

	/* ----- CALCULS MATHEMATIQUES ----- */

	/**
	 * @brief Calcul le vecteur AB, dont la norme est la distance entre le point A et B, et la direction, celle du segment AB
	 * 
	 * @param p1 Le premier point
	 * @param p2 Le deuxième point
	 * @return Vecteur p1p2 avec ses coordonnées x et y 
	 */
	std::pair<double, double> vectorPoints(const Point2D &p1, const Point2D &p2);

	/**
	 * @brief Calcul le vecteur unitaire du vecteur en argument
	 * 
	 * @param vector vecteur dont on veut le vecteur unitaire
	 * @return Vecteur unitaire
	 */
	std::pair<float, float> getUnitVector(const std::pair<double, double> &vector);

	/* ----- FORMULES PHYSIQUES ----- */

	/**
	 * @brief Calcul la force d'attraction entre deux astres
	 * 
	 * @param l1 Premier astre
	 * @param l2 Deuxième astre
	 * @return Vecteur représentant le force exercé de l1 sur l2
	 */
	std::pair<double, double> attractionForce(Luminary *l1, luminary *l2);

	/**
	 * @brief Calcul l'accélération d'un astre en fonction des forces exercées sur celui-ci
	 * 
	 * @param force La somme des forces exercées sur l'astre l
	 * @param l L'astre sur lequel les forces sont exercées. L'attribut 'acceleration' est mis à jour
	 */
	void Newton2nd(const std::pair<double, double> &force, Luminary *l);

	/**
	 * @brief Calcul le vecteur vitesse d'un astre à partir de son acceleration
	 * 
	 * @param l L'astre concerné. l'attribut 'speed' est mis à jour
	 */
	void getSpeedFromAcceleration(Luminary *l);

	/**
	 * @brief Calcul la position de l'astre
	 * 
	 * @param l L'astre concerné. l'attribut 'position' est mis à jour
	 */
	void getPositionFromSpeed(Luminary *l);
}