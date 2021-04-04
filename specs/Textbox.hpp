/**
 * @file Module Affichage 2D
 */ 

/** @brief class TextBox
 * La classe TextBox permet d'afficher du texte sous forme de texture directement sur la surface graphique.
 */
class TextBox {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
private:
	// Coordonnées 2D du point haut-gauche de la texture.
	Point2D pos;

	// Largeur de la texture.
	int width;

	// Hauteur de la texture.
	int height;

	// Texture dans laquelle on dessine le texte.
	SDL_Texture* texture = nullptr;

	// Couleur de la police.
	Color fontColor;

	// Police du texte.
	TTF_Font* font = nullptr;

	/*===========================================================================================
     *      CONSTRUCTEURS / DESTRUCTEURS
    ===========================================================================================*/
public:
	/**
	 * @brief Initier les valeurs et la texture avec une taille donnée en argument.
	 * @param text Texte à transformer en texture.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire
	 * @param fontSize Taille de la police
	 * @param fontColor Couleur de la police
	 * @param topLeft Point haut-gauche de la texture résultante
	 * @param width Largeur imposée de la texture
	 * @param height Hauteur imposée de la texture
	 * @param renderer Renderer SDL
	 */
	TextBox(std::string text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, const int& width, const int& height, SDL_Renderer* renderer);

	/**
	 * @brief Initier les valeurs et la texture. La taille de la texture s'adapte au texte.
	 * @param text Texte à transformer en texture.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire
	 * @param fontSize Taille de la police
	 * @param fontColor Couleur de la police
	 * @param topLeft Point haut-gauche de la texture résultante
	 * @param renderer Renderer SDL
	 */
	TextBox(std::string text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, SDL_Renderer* renderer);

	// Suppression du constructeur par défaut.
	TextBox() = delete;

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Mettre à jour les valeurs et la texture avec une taille donnée en argument.
	 * @param text Texte à transformer en texture.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire
	 * @param fontSize Taille de la police
	 * @param fontColor Couleur de la police
	 * @param topLeft Point haut-gauche de la texture résultante
	 * @param width Largeur imposée de la texture
	 * @param height Hauteur imposée de la texture
	 * @param renderer Renderer SDL
	 */
	void update(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, const int& width, const int& height, SDL_Renderer* renderer);

	/**
	 * @brief Mettre à jour les valeurs et la texture. La taille de la texture s'adapte au texte.
	 * @param text Texte à transformer en texture.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire
	 * @param fontSize Taille de la police
	 * @param fontColor Couleur de la police
	 * @param topLeft Point haut-gauche de la texture résultante
	 * @param renderer Renderer SDL
	 */
	void update(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, SDL_Renderer* renderer);

	/**
	 * @brief Mettre à jour le texte uniquement.
	 * @param text Texte à transformer en texture.
	 * @param renderer Renderer SDL
	 */
	void update(const std::string& text, SDL_Renderer* renderer);

	/**
	 * @brief Afficher la texture du texte.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const;

	/**
	 * @brief Changer la position de la texture du texte.
	 * @param pos Position de la texture lors de l'affichage
	 */
	void setPosition(const Point2D& pos);

	/**
	 * @brief Changer les dimensions de la texture du texte.
	 * @param w Largeur imposée de la texture
	 * @param h Hauteur imposée de la texture
	 */
	void setSize(const int& w, const int& h);

	/**
	 * @brief Libérer la mémoire allouée dans le destructeur.
	 */
	~TextBox();

	/**
	 * @brief Fonction statique pour initier la librairie nécessaire à SDL_ttf.
	 * Cette fonction n'a besoin d'être appelée qu'une seule fois.
	 */
	static void initLibrary() { TTF_Init(); }
};