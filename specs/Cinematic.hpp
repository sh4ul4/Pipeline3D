namespace Cinematic
	{
		float time = 0; // Représente le temps qui s'écoule dans le système stellaire

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
		 * @param mass La masse de l'astre
		 * @param forces Liste des forces exercées sur l'astre
		 * @return Vecteur acceleration 
		 */
		std::pair<double, double> Newton2nd(const double &mass, const std::vector<std::pair<double, double>> &forces);

		/**
		 * @brief Calcul le vecteur vitesse d'un astre à partir de son acceleration
		 * 
		 * @param acceleration Vecteur acceleration
		 * @return Vecteur vitesse
		 */
		std::pair<double, double> getSpeedFromAcceleration(const std::pair<double, double> &acceleration);

		/**
		 * @brief Calcul la position de l'astre
		 * 
		 * @param speed Vecteur vitesse
		 * @return Vecteur position / point 2D
		 */
		Point2D getPositionFromSpeed(const std::pair<double, double> &speed);
	}