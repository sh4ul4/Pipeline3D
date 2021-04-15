/**
 * @file Module Interaction utilisateur
 */

 /** @brief class Mouse
  * La classe Mouse gère les interactions utilisateur qui passent par la souris.
  */
class Mouse {
public:
	/** @brief x Position x. */
	int x = 0;

	/** @brief y Position y. */
	int y = 0;

	/** @brief xmov Mouvement x depuis le dernier check. */
	int xmov = 0;

	/** @brief ymov Mouvement y depuis le dernier check. */
	int ymov = 0;

	/** @brief rightClick True si clic droit. */
	bool rightClick = false;

	/** @brief leftClick True si clic gauche. */
	bool leftClick = false;
	
	/** @brief  Indique si la souris est en mouvement ou non. */
	bool moving = false; 

	/** @brief Réinitialise les valeurs à 0. */
	void setBack(void) {
		x = y = rightClick = leftClick = moving = 0;
	}
};
