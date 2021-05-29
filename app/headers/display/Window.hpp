#pragma once

/**
 * @file Window.hpp
 * @brief Affichage 2D : permet d'encapsuler les informations nécessaires à la fenêtre graphique générée avec SDL.
 */ 

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
	int width = 1280;
	
	
	/**
	 * Une variable privée.
	 * Hauteur de la fenêtre graphique
	 */
	int height = 720;
	
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

public:
	/**
	 * Une fonction privée.
	 * @brief Fermeture des différentes composantes de l'environnement graphique
	 */	 
	void Shutdown() {
		std::cout << "Prepare to shut down.\n";
		shuttingDown = true;
		//---------------------------------------
		// SDLK_CLEAR;
		// std::cout << "Keys cleared.\n";
		//---------------------------------------
		Mix_FreeMusic(0);
		Mix_FreeChunk(0);
		std::cout << "Channels cleared.\n";
		//---------------------------------------
		Mix_Quit();
		std::cout << "Mixer cleared.\n";
		//---------------------------------------
		TTF_Quit();
		//---------------------------------------
		if (renderer) SDL_DestroyRenderer(renderer);
		std::cout << "Renderer destroyed.\n";
		//---------------------------------------
		if (window) SDL_DestroyWindow(window);
		std::cout << "Window destroyed.\n";
		//---------------------------------------
		std::cout << "Shutdown complete." << std::endl;
	}

public:
	/**********************************************************************************************
	 * 		Méthodes permettant l'accès à une donnée spécifique
	 * *******************************************************************************************/

	/**
	 * Une fonction publique.
	 * @return Retoune la largeur de la fenêtre graphique
	 */
	int getWidth() const { return width; }
	
	/**
	 * Une fonction publique.
	 * @return Retoune la hauteur de la fenêtre graphique
	 */
	int getHeight() const { return height; }
	
	/**
	 * Une fonction publique.
	 * @return Retourne la coordonné x du centre de la fenêtre graphique
	 */
	int getWidthCenter() const { return width / 2; }
	
	/**
	 * Une fonction publique.
	 * @return Retourne la coordonné y du centre de la fenêtre graphique
	 */
	int getHeightCenter() const { return height / 2; }
	
	/**
	 * Une fonction publique.
	 * @return Retourne le pointeur sur le render (SDL)
	 */
	SDL_Renderer* getRenderer() const { return renderer; }
	
	/**
	 * Une fonction publique.
	 * @return Retourne le pointeur sur la fenêtre (SDL)
	 */
	SDL_Window* getWindow() const { return window; }
	
	/**
	 * Une fonction publique.
	 * @brief Suppression du constructeur par défaut
	 */
	Window() = delete;

	/**
	 * Un constructeur 
	 * @brief Instantier la fenêtre graphique avec des dimensions
	 * @param width Largeur de la fenetre 
	 * @param height Longueur se la fenetre
	 */
	Window(const int& width, const int& height)
		: width(width), height(height) {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Init(SDL_INIT_TIMER);
		window = SDL_CreateWindow("Pipeline3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_ShowCursor(1);
		SDL_SetWindowResizable(window, SDL_TRUE);
		SDL_SetWindowMinimumSize(window, width, height);
	}

	/**
	 * Un destructeur.
	 * @brief Le destructeur appellera la méthode Shutdown
	 */
	~Window() { Shutdown(); }
	
	/**
	 * Une fonction publique.
	 * @brief Mets en pause la fenêtre graphique pour un lapse de temps donné
	 * @param time Lapse de temps de pause 
	 */
	void Wait(const Uint32& time) const { SDL_Delay(time); }
	
	/**
	 * Une fonction publique.
	 * @brief Remplis la fenêtre avec une couleur donnée
	 * @param color Couleur de remplissage
	 */
	void FillScreen(const Color& color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		SDL_RenderClear(renderer);
	}

	/**
	 * Une fonction publique.
	 * @brief Mets à jour l'affichage de la fenêtre
	 */
	void RenderScreen() { SDL_RenderPresent(renderer); }
	
	/**
	 * Une fonction publique.
	 * @brief Change de plein écran à fenêtré ou inversement
	 * @param width Largeur de la fenetre
	 * @param height Hauteur de la fenetre 
	 */
	void ToggleWindow(const int& width, const int& height) {
		fullScreen = !fullScreen;
		SDL_SetWindowResizable(window, SDL_TRUE);
		if (!fullScreen) {
			SDL_DisplayMode DM;
			SDL_GetCurrentDisplayMode(0, &DM);
			this->width = DM.w;
			this->height = DM.h;
			SDL_SetWindowSize(window, this->width, this->height);
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else if (fullScreen) {
			this->width = width;
			this->height = height;
			SDL_SetWindowSize(window, this->width, this->height);
			SDL_SetWindowFullscreen(window, 0);
		}
		SDL_SetWindowResizable(window, SDL_FALSE);
	}
};