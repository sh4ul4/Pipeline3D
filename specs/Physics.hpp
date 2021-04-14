
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
		/* ----- FORMULES PHYSIQUES ----- */

		/**
		 * @brief Calcul la force d'attraction entre deux corps
		 * 
		 * @param l1 Premier corp
		 * @param l2 Deuxième corp
		 * @return  
		 */
		float attractionForce(Luminary *l1, luminary *l2);

		/**
		 * @brief Calcul l'accélération d'un astre en fonction des forces exercées sur celui-ci
		 * 
		 * @param mass Masse de la planète
		 * @param force Forces exercées sur la planète (il faudrait un std::vector des forces)
		 * @return std::pair<float, float> --> vecteur accélération
		 */
		std::pair<float, float> Newton2nd(float mass, std::pair<float, float> force);
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
