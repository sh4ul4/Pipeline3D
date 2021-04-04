/**
 * @file Module Affichage 2D
 */ 

/** @brief class Texture2D
 * La classe Texture2D permet de contenir les données d'une texture graphique à affichage accéléré.
 * Cette texture a l'avantage d'être rapide à l'affichage mais lente en modification des pixels.
 */
class Texture2D {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
public:
	// Coordonnées 2D du point haut-gauche de la texture.
	Point2D pos;

	// Largeur de la texture.
	int width = 0;

	// Hauteur de la texture.
	int height = 0;
private:
	// Structure SDL pour contenir la texture.
	SDL_Texture* texture;

	/*===========================================================================================
     *      CONSTRUCTEURS / DESTRUCTEURS
    ===========================================================================================*/
public:
	// Suppression du constructeur de base.
	Texture2D() = delete;

	/**
	 * @brief Création de la texture à partir d'une image dans la mémoire.
	 * @param path Chemin d'accès pour charger l'image
	 * @param renderer Renderer SDL
	 */
	Texture2D(const std::string& path, SDL_Renderer* renderer);

	/**
	 * @brief Création de la texture à partir d'une image dans la mémoire en spécifiant ses dimensions d'affichage.
	 * @param path Chemin d'accès pour charger l'image
	 * @param renderer Renderer SDL
	 * @param pos Position de l'image lors de son affichage
	 * @param width Largeur de l'image
	 * @param height Hauteur de l'image
	 */
	Texture2D(const std::string& path, SDL_Renderer* renderer, const Point2D& pos, const int& width, const int& height);

	/**
	 * @brief Libérer la mémoire de la texture.
	 */
	~Texture2D();

	/**
	 * Affichage de la texture avec les dimensions de départ.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const;

	/**
	 * Affichage de la texture avec les dimensions données en paramètre.
	 * @param renderer Renderer SDL
	 * @param position Position de l'image lors de l'affichage
	 * @param w Largeur de l'image affichée
	 * @param h Hauteur de l'image affichée
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const Point2D& position, const int& w, const int& h, const int& flip, const double& angle);
};