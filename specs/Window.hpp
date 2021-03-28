/**
 * @class Window pour permet d'encapsuler les informations nécessaires à la fenêtre graphique générée avec SDL
 */
class Window
{

private:
	/**********************************************************************************************
	 * 		Attributs
	 * *******************************************************************************************/

	// Largeur de la fenêtre graphique
	int width = NULL;

	// Hauteur de la fenêtre graphique
	int height = NULL;

	// Résolution de la fenêtre graphique selon l'axe x
	int renderWidth = NULL;

	// Résolution de la fenêtre graphique selon l'axe y
	int renderHeight = NULL;

	// Booléen pour gérer l'activation du mode plein écran
	bool fullScreen = false;

	// Variable SDL pour contenir les données de la fenêtre graphique
	SDL_Window *window = NULL;

	// Variable SDL pour contenir le pinceau pour dessiner dans la fenêtre graphique
	SDL_Renderer *renderer = NULL;

	// Variable pour gérer la fermeture de la fenêtre
	bool shuttingDown = false;

	/**********************************************************************************************
	 * 		Méthode privée
	 * *******************************************************************************************/

	// Fermeture des différentes composantes de l'environnement graphique
	void Shutdown();

public:
	/**********************************************************************************************
	 * 		Méthodes permettant l'accès à une donnée spécifique
	 * *******************************************************************************************/

	// Retoune la largeur de la fenêtre graphique
	int getWidth() const;
	// Retoune la hauteur de la fenêtre graphique
	int getHeight() const;
	// Retoune la résolution de la fenêtre graphique sur l'axe x
	int getRenderWidth() const;
	// Retoune la résolution de la fenêtre graphique sur l'axe y
	int getRenderHeight() const;
	// Retourne la coordonné x du centre de la fenêtre graphique
	int getWidthCenter() const;
	// Retourne la coordonné y du centre de la fenêtre graphique
	int getHeightCenter() const;
	// Retourne le pointeur sur le render (SDL)
	SDL_Renderer *getRenderer() const;
	// Retourne le pointeur sur la fenêtre (SDL)
	SDL_Window *getWindow() const;

	// Suppression du constructeur par défaut
	Window() = delete;

	// Instantier la fenêtre graphique avec une résolution et des dimensions
	Window(const int &resolutionWidth, const int &resolutionHeight, const int &width, const int &height)
		: width(width), height(height), renderWidth(resolutionWidth), renderHeight(resolutionHeight) {}

	// Instantier la fenêtre graphique avec des dimensions
	Window(const int &width, const int &height)
		: width(width), height(height), renderWidth(width), renderHeight(height) {}

	// destructeur de la classe (le destructeur appellera la méthode Shutdown)
	~Window();

	// Mettre en pause la fenêtre graphique pour un lapse de temps donné
	void Wait(const Uint32 &time) const;

	// Remplir la fenêtre avec une couleur donnée
	void FillScreen(const Color &color) const;

	// Mettre à jour l'affichage de la fenêtre
	void RenderScreen() const;

	// Changer de plein écran à fenêtré ou inversement
	void ToggleWindow(const int &width, const int &height);
};