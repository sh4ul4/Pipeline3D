#pragma once

class StableFramerate {
private:

	/**
	 * Une variable privée.
	 * Compte le nombre de frames crées depuis le début
	 */
	Uint32 totalFrames = 0;
	
	/**
	 * Une variable privée.
	 * Temps du prochain rendu de frame
	 */
	Uint32 nextTime = 0; 
	
	/**
	 * Une variable privée.
	 * Chronometre pour attendre 1000 ticks
	 */
	Timer timer; 
	
	/**
	 * Une variable privée.
	 * Compte le nombre de frames pour le framerate
	 */
	short int frameCounter = 0; 
	
	/**
	 * Une variable privée.
	 * Framerate actuel 
	 */
	short int framerate = 0;
	
	/**
	 * Une variable privée.
	 * Temps actuel en SDL_GetTicks()
	 */
	Uint32 now = 0;

	/**
	 * Une variable privée.
	 * Temps écoulé en ms
	 */
	float elapsedMS = 0;


public:

	StableFramerate():timer(1000) {}

	/**
	 * Une fonction publique.
	 * @brief Limite les calculs et le rendering
	 * @param framerate Limite imposée
	 */
	void stabilizeCalculationAndRendering(const double& framerate_max = 0, Uint64 *start = 0) {
		Uint32 ticksPerFrame = 1000.0f / float(framerate_max);
		Uint64 end = SDL_GetPerformanceCounter();
		elapsedMS = (end - *start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		// Cap to 60 FPS
		Uint32 waitingTime = floor(ticksPerFrame - elapsedMS);
		// std::cout << elapsedMS << "_" << waitingTime << std::endl;
		std::cout << "Current FPS: " << floor(1000.0f / elapsedMS) << std::endl;
		if (elapsedMS < ticksPerFrame)
		{
			SDL_Delay(waitingTime);
		}
	}
	
	/**
	 * Une fonction publique.
	 * @brief Limite le rendering pour l'adapter a 60 framerates dans la fenetre ( window)
	 * @param framerate Limite imposée
	 * @param window Ecran d'affichage
	 */
	void stabilizeRendering(Window* window, double framerate = 0) {
		// optimized formula with approximative constants
		if (framerate == 60) { framerate = 17; }
		else { framerate = 1.621059 + (181237898 / (1 + pow(framerate / 0.0000460458, 1.156912))); }
		now = SDL_GetTicks(); // current time in SDL_Ticks
		if (nextTime == 0) { nextTime = now + framerate; }
		if (nextTime < now) {
			window->RenderScreen();
			nextTime = 0;
		}
	}

	/**
	 * @brief Affiche a l'ecran les framerate actuels 
	 * 
	 * @param x,y Coordonnées du point où sera affiché le framerate
	 * @param renderer Moteur de rendu SDL
	 */
	void renderFrameRate(const int& X, const int& Y, SDL_Renderer* renderer) {
		frameCounter++;
		// std::cout << "framecounter: " << frameCounter << std::endl;
		// std::cout << "Current FPS: " << floor(1000.0f / elapsedMS) << std::endl;
		
		// if (timer.set(1 * CLOCKS_PER_SEC)) {
		if (time(0) > now){ // Si une seconde en utime s'est écoulée
			framerate = frameCounter; // return the framerate after 1 sec
			frameCounter = 0;
		}
		now = time(0);

		// Draw::RenderDigits(X, Y, 8, framerate, red, renderer);
		Draw::RenderDigits(X, Y, 8, floor(framerate), red, renderer);
	}
	
	/**
	 * Une fonction publique.
	 * @brief Creer un nouveau frame et en augmente le compteur
	 * @param void
	 */
	void newFrame(void) {
		frameCounter++;
	}
};
