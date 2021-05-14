#pragma once
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
	Point2D<int> pos;

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
	Texture2D(const std::string& path, SDL_Renderer* renderer) {
		SDL_Surface* surface = IMG_Load(FIND_FILE(path).c_str());
		width = surface->w;
		height = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	/**
	 * @brief Création de la texture à partir d'une image dans la mémoire en spécifiant ses dimensions d'affichage.
	 * @param path Chemin d'accès pour charger l'image
	 * @param renderer Renderer SDL
	 * @param pos Position de l'image lors de son affichage
	 * @param width Largeur de l'image
	 * @param height Hauteur de l'image
	 */
	Texture2D(const std::string& path, SDL_Renderer* renderer, const Point2D<int>& pos, const int& width, const int& height)
		: pos(pos), width(width), height(height) {
		SDL_Surface* surface = IMG_Load(FIND_FILE(path).c_str());
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	/**
	 * @brief Libérer la mémoire de la texture.
	 */
	~Texture2D() { SDL_DestroyTexture(texture); }

	/**
	 * Affichage de la texture avec les dimensions de départ.
	 * @param renderer Renderer SDL
	 */
	void render(SDL_Renderer* renderer) const {
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		const SDL_Rect dstrect{ pos.x, pos.y, width, height };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	}

	/**
	 * Affichage de la texture avec les dimensions de départ.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const {
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		SDL_Rect dstrect{ pos.x, pos.y, width, height };
		switch (flip) {
		case 0: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		case 1: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_HORIZONTAL); break;
		case 2: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_VERTICAL); break;
		default: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		}
	}

	/**
	 * Affichage de la texture avec les dimensions données en paramètre.
	 * @param renderer Renderer SDL
	 * @param position Position de l'image lors de l'affichage
	 * @param w Largeur de l'image affichée
	 * @param h Hauteur de l'image affichée
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const Point2D<int>& position, const int& w, const int& h) const {
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		const SDL_Rect dstrect{ position.x, position.y, w, h };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	}

	/**
	 * Affichage de la texture avec les dimensions données en paramètre.
	 * @param renderer Renderer SDL
	 * @param position Position de l'image lors de l'affichage
	 * @param w Largeur de l'image affichée
	 * @param h Hauteur de l'image affichée
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const Point2D<int>& position, const int& w, const int& h, const int& flip, const double& angle) const {
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		SDL_Rect dstrect{ position.x, position.y, w, h };
		switch (flip) {
		case 0: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		case 1: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_HORIZONTAL); break;
		case 2: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_VERTICAL); break;
		default: SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		}
	}
};