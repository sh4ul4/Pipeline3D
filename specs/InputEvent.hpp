/**
 * @file Issu du module de Intéraction Utilisateur.
 * S'appuie sur les classes Mouse et Keyboard.
 */

/*===========================================================================================
	INPUTEVENT
===========================================================================================*/

/**
 * @class InputEvent pour gérer les entrées utilisateur et diviser leurs interprétations.
 */
class InputEvent {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
private:
	// @brief Event SDL pour suivre les entrées utilisateurs diverses.
	SDL_Event event;

	// @brief Classe pour enregistrer et mettre à jour les données de la souris.
	Mouse mouse;

	// @brief Classe pour enregistrer et mettre à jour les données du clavier.
	Keyboard keyboard;

	// @brief String pour enregistrer les charactères Unicode sur demande.
	std::string text;

	/*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/
public:
	/**
	 * @brief Constructeur qui appelle les constructeur de mouse et keyboard et qui arrête l'input de texte.
	 */
	InputEvent() { SDL_StopTextInput(); }

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Renvoie une copie du string text.
	 */
	std::string getText()const { return text; }

	/**
	 * @brief Vide le string text.
	 */
	void clearText() { text.clear(); }
	
	/**
	 * @brief Met à jour la souris passée en argument en copiant les informations de la souris locale.
	 * @param mouse Souris à mettre à jour.
	 */
	void updateMouse(Mouse& mouse)const { mouse = this->mouse; }

	/**
	 * @brief Met à jour le clavier passé en argument en copiant les informations du clavier local.
	 * @param keyboard Clavier à mettre à jour.
	 */
	void updateKeyBoard(Keyboard& keyboard)const { keyboard = this->keyboard; }

	/**
	 * @brief Met à jour tout l'input localement en écrivant dans mouse, keyboard et text.
	 */
	void update();
};