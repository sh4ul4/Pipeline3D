#pragma once
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
public:
	// Coordonnées 2D du point haut-gauche de la texture.
	Point2D<int> pos;

	// Largeur de la texture.
	int maxWidth = 999999;

	// Hauteur de la texture.
	int maxHeight = 999999;

	// Largeur de la texture.
	int width = 0;

	// Hauteur de la texture.
	int height = 0;
protected:

	// Texture dans laquelle on dessine le texte.
	SDL_Texture* texture = nullptr;

	// Couleur de la police.
	Color fontColor;

	// Police du texte.
	TTF_Font* font = nullptr;

	// texte affiché
	std::string text;

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
		const Point2D<int>& topLeft, const Uint32& maxw, const Uint32& maxh, SDL_Renderer* renderer)
		: pos(topLeft), maxWidth(maxw), maxHeight(maxh), fontColor(fontColor), text(text) {
		if (font) TTF_CloseFont(font);
		font = TTF_OpenFont(FIND_FILE(fontPath).c_str(), fontSize);
		if (!font)FATAL_ERR("could not load font");
		if (text.empty()) text = " ";
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fontColor.toSDL_Color(), maxWidth);
		width = surface->w;
		height = surface->h;
		if (height > maxHeight) height = maxHeight;
		if (texture) SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

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
		const Point2D<int>& topLeft, SDL_Renderer* renderer)
		: pos(topLeft), fontColor(fontColor), text(text) {
		if (font) TTF_CloseFont(font);
		font = TTF_OpenFont(FIND_FILE(fontPath).c_str(), fontSize);
		if (!font)FATAL_ERR("could not load font");
		if (TTF_SizeText(font, text.c_str(), &width, &height)) FATAL_ERR("Failed assigning text dimensions in TextBox.");
		if (width > maxWidth)width = maxWidth;
		if (height > maxHeight)height = maxHeight;
		if (text.empty()) text = " ";
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), fontColor.toSDL_Color());
		if (texture) SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}

	// Suppression du constructeur par défaut.
	TextBox() = delete;

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Mettre à jour les valeurs et la texture. La taille de la texture s'adapte au texte.
	 * @param text Texte à transformer en texture.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire
	 * @param fontSize Taille de la police
	 * @param fontColor Couleur de la police
	 * @param topLeft Point haut-gauche de la texture résultante
	 * @param renderer Renderer SDL
	 */
	void update(std::string text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		SDL_Renderer* renderer) {
		if (font) TTF_CloseFont(font);
		font = TTF_OpenFont(FIND_FILE(fontPath).c_str(), fontSize);
		if (!font)FATAL_ERR("could not load font");
		if (text.empty()) text = " ";
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fontColor.toSDL_Color(), maxWidth);
		width = surface->w;
		height = surface->h;
		if (height > maxHeight) height = maxHeight;
		if (texture) SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		this->text = text;
	}

	/**
	 * @brief Mettre à jour le texte uniquement.
	 * @param text Texte à transformer en texture.
	 * @param renderer Renderer SDL
	 */
	void update(std::string text, SDL_Renderer* renderer) {
		if (text.empty()) text = " ";
		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), fontColor.toSDL_Color(), maxWidth);
		width = surface->w;
		height = surface->h;
		if (height > maxHeight) height = maxHeight;
		if (texture) SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		this->text = text;
	}

	/**
	 * @brief Afficher la texture du texte.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer) const {
		if (renderer == nullptr || texture == nullptr) { PRINT_ON_ERR("Error occured in renderTexture()"); return; }
		const SDL_Rect srcrect{ 0, 0, width, height };
		const SDL_Rect dstrect{ pos.x, pos.y, width, height };
		SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	}

	/**
	 * @brief Afficher la texture du texte.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const int& flip, const double& angle) const {
		if (renderer == nullptr || texture == nullptr) { PRINT_ON_ERR("Error occured in renderTexture()"); return; }
		const SDL_Rect srcrect{ 0, 0, width, height };
		const SDL_Rect dstrect{ pos.x, pos.y, width, height };
		switch (flip) {
		case 0: SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		case 1: SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, NULL, SDL_FLIP_HORIZONTAL); break;
		case 2: SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, NULL, SDL_FLIP_VERTICAL); break;
		default: SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, NULL, SDL_FLIP_NONE); break;
		}
	}

	/**
	 * @brief Changer la position de la texture du texte.
	 * @param pos Position de la texture lors de l'affichage
	 */
	void setPosition(const Point2D<int>& pos) { this->pos = pos; }

	void center(const Point2D<int>& tl, int max_width)  {
		int perfect = ((max_width - this->width) / 2) + tl.x;
		setPosition(Point2D<int>(perfect, pos.y));
	}

	/**
	 * @brief Changer les dimensions de la texture du texte.
	 * @param w Largeur imposée de la texture
	 * @param h Hauteur imposée de la texture
	 */
	void setSize(const int& w, const int& h) {
		if (w <= maxWidth)width = w;
		if (h <= maxHeight)height = h;
	}

	/**
	 * @brief Changer les dimensions de la texture du texte en fonction de la police et la taille.
	 */
	void setSize() {
		TTF_SizeText(font, text.c_str(), &width, &height);
	}

	/**
	 * @brief Renvoie le texte affiché
	 */
	std::string getText() const { return text; }

	/**
	 * @brief Libérer la mémoire allouée dans le destructeur.
	 */
	~TextBox() {
		SDL_DestroyTexture(texture);
		if (font) TTF_CloseFont(font);
	}

	/**
	 * @brief Fonction statique pour initier la librairie nécessaire à SDL_ttf.
	 * Cette fonction n'a besoin d'être appelée qu'une seule fois.
	 */
	static void initLibrary() { TTF_Init(); }
};