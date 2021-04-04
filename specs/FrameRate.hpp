#pragma once

//#include "library.h"
#include "Draw.h"
#include "Window.h"

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
	
	Chrono chrono; 
	
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

	/**
	 * Une fonction publique.
	 * @brief Limite les calculs et le rendering
	 * @param framerate Limite imposée
	 */	

	void stabilizeCalculationAndRendering(const double& framerate = 0);
	
	/**
	 * Une fonction publique.
	 * @brief Limite le rendering pour l'adapter a 60 framerates dans la fenetre ( window)
	 * @param framerate Limite imposée
	 * @param window Ecran d'affichage
	 */		
	
	void stabilizeRendering(Window* window, double framerate = 0);

	/**
	 * Une fonction publique.
	 * @brief Affiche a l'ecran les framerate actuels 
	 * @param x,y Coordonnées du point où sera affiché le framerate
	 * @param renderer Moteur de rendu SDL
	 */		
	
	void renderFrameRate(const int& X, const int& Y, SDL_Renderer* renderer);
	
	/**
	 * Une fonction publique.
	 * @brief Creer un nouveau frame et en augmente le compteur
	 * @param void
	 */	
	
	void newFrame(void);
};