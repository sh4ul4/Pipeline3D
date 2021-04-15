
// Région déclarative contenant des valeurs et des méthodes concernant la physique étudiant le mouvement
namespace Physics
{
	Keyboard keyboard;
	/* ----- Valeurs correspondant aux lois physiques utilisées ----- */

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
		float getGravityForce();
	}

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
	void resolvePhysics(const ShapeManager &manager, const Vertex &pos, const Vertex &step, const float &speed, const bool &falling, const int &depth);

	/**
	 * @brief Appliquer les mouvements sur les formes
	 * 
	 * @param inputEvent ???
	 * @param manager ???
	 */
	void move(InputEvent &inputEvent, const ShapeManager &manager);
}
