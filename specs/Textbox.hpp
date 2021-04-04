/**
 * @file Module Affichage 2D
 */ 

/** @brief class TextBox
 * La classe TextBox permet d'afficher du texte sous forme de texture directement sur la surface graphique.
 */
class TextBox {
private:

	/**
	 * Coordonnées 2D du point haut-gauche de la texture.
	 */
	Point2D pos;

	/**
	 * Largeur de la texture.
	 */
	int width;

	/**
	 * Hauteur de la texture.
	 */
	int height;

	/**
	 * Texture dans laquelle on dessine le texte.
	 */
	SDL_Texture* texture = nullptr;

	/**
	 * Couleur de la police.
	 */
	Color fontColor;

	/**
	 * Police du texte.
	 */
	TTF_Font* font = nullptr;

public:
	/**
	 * Initier les valeurs et la texture avec une taille donnée en argument.
	 */
	TextBox(std::string text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, const int& width, const int& height, SDL_Renderer* renderer);

	/**
	 * Initier les valeurs et la texture. La taille de la texture s'adapte au texte.
	 */
	TextBox(std::string text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, SDL_Renderer* renderer);

	/**
	 * Suppression du constructeur par défaut.
	 */
	TextBox() = delete;

	/**
	 * Mettre à jour les valeurs et la texture avec une taille donnée en argument.
	 */
	void update(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, const int& width, const int& height, SDL_Renderer* renderer);

	/**
	 * Mettre à jour les valeurs et la texture. La taille de la texture s'adapte au texte.
	 */
	void update(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D& topLeft, SDL_Renderer* renderer);

	/**
	 * Mettre à jour le texte uniquement.
	 */
	void update(const std::string& text, SDL_Renderer* renderer);

	/**
	 * Afficher la texture du texte.
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const;

	/**
	 * Changer la position de la texture du texte.
	 */
	void setPosition(const Point2D& pos);

	/**
	 * Changer les dimensions de la texture du texte.
	 */
	void setSize(const int& w, const int& h);

	/**
	 * Libérer la mémoire allouée dans le destructeur.
	 */
	~TextBox();

	/**
	 * Fonction statique pour initier la librairie nécessaire à SDL_ttf.
	 * Cette fonction n'a besoin d'être appelée qu'une seule fois.
	 */
	static void initLibrary() { TTF_Init(); }
};