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
	Uint32 nextTime = NULL; 
	
	/**
	 * Une variable privée.
	 * Chronometre pour attendre 1000 ticks
	 */
	Timer timer; 
	
	/**
	 * Une variable privée.
	 * Compte le nombre de frames pour le framerate
	 */
	short int frameCounter = NULL; 
	
	/**
	 * Une variable privée.
	 * Framerate actuel 
	 */
	short int framerate = NULL;
	
	/**
	 * Une variable privée.
	 * Temps actuel en SDL_GetTicks()
	 */
	Uint32 now = NULL; 
public:

	StableFramerate():timer(0) {}

	/**
	 * Une fonction publique.
	 * @brief Limite les calculs et le rendering
	 * @param framerate Limite imposée
	 */
	void stabilizeCalculationAndRendering(const double& framerate = 0) {
		if (framerate == 60) nextTime += 17;
		else if (framerate == 25)nextTime += 43.65954;
		else { nextTime += 1.621059 + 181237898.4 / (1 + pow(framerate / 0.0000460458, 1.156912)); }
		now = clock(); // current time in SDL_Ticks
		if (nextTime > now) {
			SDL_Delay(Uint32(nextTime - now)); // time to wait before rendering next frame
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
	 * Une fonction publique.
	 * @brief Affiche a l'ecran les framerate actuels 
	 * @param x,y Coordonnées du point où sera affiché le framerate
	 * @param renderer Moteur de rendu SDL
	 */
	void renderFrameRate(const int& X, const int& Y, SDL_Renderer* renderer) {
		frameCounter++;
		if (timer.set(1000)) {
			framerate = frameCounter; // return the framerate after 1 sec
			frameCounter = 0;
		}
		Draw::RenderDigits(X, Y, 8, framerate, red, renderer);
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