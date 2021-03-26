/**
 * @class Window pour permet d'encapsuler les informations nécessaires à la fenêtre graphique générée avec SDL
 */
class Window {

private:

	// largeur de la fenêtre graphique
	int width = NULL;

	// hauteur de la fenêtre graphique
	int height = NULL;

	// résolution de la fenêtre graphique selon l'axe x
	int renderWidth = NULL;

	// résolution de la fenêtre graphique selon l'axe x
	int renderHeight = NULL;

	// booléen pour gérer l'activation du mode plein écran
	bool fullScreen = false;

	// variable SDL pour contenir les données de la fenêtre graphique
	SDL_Window* window = NULL;

	// variable SDL pour contenir le pinceau pour dessiner dans la fenêtre graphique
	SDL_Renderer* renderer = NULL;

	// variable pour gérer la fermeture de la fenêtre
	bool shuttingDown = false;

public:

	// fonctions pour donner accès à la donnée spécifique
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getRenderWidth() const { return renderWidth; }
	int getRenderHeight() const { return renderHeight; }
	int getWidthCenter() const { return width / 2; }
	int getHeightCenter() const { return height / 2; }
	SDL_Renderer* getRenderer() const { return renderer; }
	SDL_Window* getWindow() const { return window; }

	// suppression du constructeur par défaut
	Window() = delete;

	// instantier la fenêtre graphique avec une résolution et des dimensions
	Window(const int& resolutionWidth, const int& resolutionHeight, const int& width, const int& height)
	 : width(width), height(height), renderWidth(resolutionWidth), renderHeight(resolutionHeight) {}

	// instantier la fenêtre graphique avec des dimensions
	Window(const int& width, const int& height)
	 : width(width), height(height), renderWidth(width), renderHeight(height) {}

	// destructeur de la classe
	~Window() { Shutdown(); }

private:

	// fermeture des différentes composantes de l'environnement graphique
	void Shutdown() {}

public:

	// mettre en pause la fenêtre graphique pour un lapse de temps donné
	void Wait(const Uint32& time) const { SDL_Delay(time); }

	// remplir la fenêtre avec une couleur donnée
	void FillScreen(const Color& color) const
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(renderer);
	}

	// mettre à jour l'affichage de la fenêtre
	void RenderScreen() const { SDL_RenderPresent(renderer); }

	// changer de plein écran à fenêtré ou inversement
	void ToggleWindow(const int& width, const int& height) {}

};
