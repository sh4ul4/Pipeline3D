/**
 * @file Issu du module de Intéraction Utilisateur.
 * Extension de la classe TextBox.
 */

/*===========================================================================================
	TEXTINPUT
===========================================================================================*/

/**
 * @class TextInput permet de gérer l'input dynamique de texte.
 */
class TextInput : public TextBox {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
private:
	// @brief copie locale de l'état des touches du clavier.
	Keyboard keyboard;

	// @brief Texte mis à jour avec l'input.
	std::string text;

	// @brief Indique si la classe est en train d'enregistrer l'input utilisateur.
	bool running = false;

	/*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/
public:
	/**
	 * @brief Constructeur qui construit l'objet TextBox par polymorphisme.
	 * @param text Texte de départ.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire.
	 * @param fontSize Taille de la police.
	 * @param fontColor Couleur de la police.
	 * @param topLeft Point haut-gauche de la texture résultante.
	 * @param renderer Renderer SDL.
	 */
	TextInput(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, SDL_Renderer* renderer) : TextBox(text,fontPath,fontSize,fontColor,topLeft,renderer), text(text) {}

	// suppression du constructeur par défaut
	TextInput() = delete;

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Afficher la texture du texte.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const;

	/**
	 * @brief Démarrer l'enregistrement de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 */
	void start(InputEvent& ie);

	/**
	 * @brief Arrêter l'enregistrement de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 */
	void stop(InputEvent& ie);

	/**
	 * @brief Mettre à jour le texte et son affichage en fonction de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 * @param renderer Renderer SDL
	 */
	void checkForInput(InputEvent& ie, SDL_Renderer* renderer);
};