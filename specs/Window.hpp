/**
 * @class Window pour permet d'encapsuler les informations nécessaires à la fenêtre graphique générée avec SDL
 */
class Window
{

private:
	/**********************************************************************************************
	 * 		Attributs
	 * *******************************************************************************************/

	/**
	 * Une variable privée.
	 * Largeur de la fenêtre graphique
	 */

	
	int width = NULL;
	
	
	/**
	 * Une variable privée.
	 * Hauteur de la fenêtre graphique
	 */

	
	int height = NULL;
	
	/**
	 * Une variable privée.
	 * Résolution de la fenêtre graphique selon l'axe x
	 */

	
	int renderWidth = NULL;
	
	/**
	 * Une variable privée.
	 * Résolution de la fenêtre graphique selon l'axe y
	 */	

	int renderHeight = NULL;
	
	/**
	 * Une variable privée.
	 * Booléen pour gérer l'activation du mode plein écran
	 */	

	bool fullScreen = false;
	
	/**
	 * Une variable privée.
	 * Variable SDL pour contenir les données de la fenêtre graphique
	 */	

	SDL_Window *window = NULL;
	
	/**
	 * Une variable privée.
	 * Variable SDL pour contenir le pinceau pour dessiner dans la fenêtre graphique
	 */	

	SDL_Renderer *renderer = NULL;
	
	/**
	 * Une variable privée.
	 * Variable pour gérer la fermeture de la fenêtre
	 */	

	bool shuttingDown = false;

	/**********************************************************************************************
	 * 		Méthode privée
	 * *******************************************************************************************/
	 
	 
	/**
	 * Une fonction privée.
	 * @brief Fermeture des différentes composantes de l'environnement graphique
	 */	 

	void Shutdown();

public:
	/**********************************************************************************************
	 * 		Méthodes permettant l'accès à une donnée spécifique
	 * *******************************************************************************************/

	/**
	 * Une fonction publique.
	 * @return Retoune la largeur de la fenêtre graphique
	 */	

	int getWidth() const;
	
	/**
	 * Une fonction publique.
	 * @return Retoune la hauteur de la fenêtre graphique
	 */		
	
	int getHeight() const;
	
	/**
	 * Une fonction publique.
	 * @return Retoune la résolution de la fenêtre graphique sur l'axe x
	 */		
	
	int getRenderWidth() const;
	
	/**
	 * Une fonction publique.
	 * @return Retoune la résolution de la fenêtre graphique sur l'axe y
	 */		
	

	int getRenderHeight() const;
	
	/**
	 * Une fonction publique.
	 * @return Retourne la coordonné x du centre de la fenêtre graphique
	 */		
	
	int getWidthCenter() const;
	
	/**
	 * Une fonction publique.
	 * @return Retourne la coordonné y du centre de la fenêtre graphique
	 */		
	

	int getHeightCenter() const;
	
	/**
	 * Une fonction publique.
	 * @return Retourne le pointeur sur le render (SDL)
	 */		
	

	SDL_Renderer *getRenderer() const;
	
	/**
	 * Une fonction publique.
	 * @return Retourne le pointeur sur la fenêtre (SDL)
	 */		
	
	SDL_Window *getWindow() const;
	
	/**
	 * Une fonction publique.
	 * @brief Suppression du constructeur par défaut
	 */		


	Window() = delete;
	
	/**
	 * Un constructeur 
	 * @brief Instantier la fenêtre graphique avec une résolution et des dimensions
	 * @param resolutionwidth Largeur de la résolution
	 * @param resolutionHeight Longueur de la résolution
	 * @param width Largeur de la fenetre 
	 * @param height Longueur se la fenetre
	 */		

	// Instantier la fenêtre graphique avec une résolution et des dimensions
	Window(const int &resolutionWidth, const int &resolutionHeight, const int &width, const int &height)
		: width(width), height(height), renderWidth(resolutionWidth), renderHeight(resolutionHeight) {}

	/**
	 * Un constructeur 
	 * @brief Instantier la fenêtre graphique avec des dimensions
	 * @param width Largeur de la fenetre 
	 * @param height Longueur se la fenetre
	 */		

	Window(const int &width, const int &height)
		: width(width), height(height), renderWidth(width), renderHeight(height) {}

	/**
	 * Un destructeur.
	 * @brief Le destructeur appellera la méthode Shutdown
	 */	

	~Window();
	
	/**
	 * Une fonction publique.
	 * @brief Mets en pause la fenêtre graphique pour un lapse de temps donné
	 * @param time Lapse de temps de pause 
	 */		

	void Wait(const Uint32 &time) const;
	
	/**
	 * Une fonction publique.
	 * @brief Remplis la fenêtre avec une couleur donnée
	 * @param color Couleur de remplissage
	 */		

	void FillScreen(const Color &color) const;

	/**
	 * Une fonction publique.
	 * @brief Mets à jour l'affichage de la fenêtre
	 */		
	
	void RenderScreen() const;
	
	/**
	 * Une fonction publique.
	 * @brief Change de plein écran à fenêtré ou inversement
	 * @param width Largeur de la fenetre
	 * @param height Hauteur de la fenetre 
	 */		

	void ToggleWindow(const int &width, const int &height);
};