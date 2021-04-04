/**
 * @file Module Affichage 2D
 */ 

/** @brief class Texture2D
 * La classe Texture2D permet de contenir les données d'une texture graphique à affichage accéléré.
 * Cette texture a l'avantage d'être rapide à l'affichage mais lente en modification des pixels.
 */
class Texture2D {
public:

	/**
	 * Coordonnées 2D du point haut-gauche de la texture.
	 */
	Point2D pos;

	/**
	 * Largeur de la texture.
	 */
	int width = 0;

	/**
	 * Hauteur de la texture.
	 */
	int height = 0;

private:

	/**
	 * Structure SDL pour contenir la texture.
	 */
	SDL_Texture* texture;

public:

	/**
	 * Suppression du constructeur de base.
	 */
	Texture2D() = delete;

	/**
	 * Création de la texture à partir d'une image dans la mémoire.
	 */
	Texture2D(const std::string& path, SDL_Renderer* renderer);

	/**
	 * Création de la texture à partir d'une image dans la mémoire en spécifiant ses dimensions d'affichage.
	 */
	Texture2D(const std::string& path, SDL_Renderer* renderer, const Point2D& pos, const int& width, const int& height);

	/**
	 * Affichage de la texture avec les dimensions de départ.
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const;

	/**
	 * Affichage de la texture avec les dimensions données en paramètre.
	 */
	void render(SDL_Renderer* renderer, const Point2D& position, const int& w, const int& h, const int& flip, const double& angle);

	/**
	 * Libérer la mémoire de la texture.
	 */
	~Texture2D();
};