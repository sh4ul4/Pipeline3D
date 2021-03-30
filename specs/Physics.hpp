
// Région déclarative contenant des valeurs et des méthodes concernant la physique étudiant le mouvement 
namespace Physics
{
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
	void resolvePhysics(const ShapeManager &manager, const Vertex &pos, const Vertex &step, const float &speed, const bool &falling, const int &depth);

	/**
	 * @brief Appliquer les mouvements sur les formes
	 * 
	 * @param inputEvent ???
	 * @param manager ???
	 */
	void move(InputEvent &inputEvent, const ShapeManager &manager);
}
