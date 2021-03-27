#pragma once

//#include "library.h"
#include "Draw.h"
#include "Window.h"

class StableFramerate {
private:
	Uint32 totalFrames = 0; // Compte le nombre de frames crées depuis le début
	Uint32 nextTime = NULL; // Temps du prochain rendu de frame
	Chrono chrono; // Pour attendre 1000
	short int frameCounter = NULL; // compte le nombre de frames pour le framerate
	short int framerate = NULL;
	Uint32 now = NULL; // temps actuel en SDL_GetTicks()
public:
	void stabilizeCalculationAndRendering(const double& framerate = 0){ // limiter les calculs et rendering
	}
	void stabilizeRendering(Window* window, double framerate = 0){ // limiter le rendering pour adapter à 60 framerates
	}
	void renderFrameRate(const int& X, const int& Y, SDL_Renderer* renderer){ // Affiche sur l'écran le framerate
	}
	void newFrame(void) {
		frameCounter++;
	}
};