#pragma once

/**
 * @file Chrono.hpp
 * @brief Contient la classe Timer, minuteur avec contrôle du lancement et du temps écoulé
 */

/**
 * @class Timer
 * @brief Minuteur avec contrôle du lancement et du temps écoulé
 */

class Timer {
private:
	/**
	 * Une variable privée.
	 * Indique le temps de départ.
	 */
	Uint32 start = 0;

	/**
	 * Une variable privée.
	 * Indique le temps actuel.
	 */
	Uint32 current = 0;

	/**
	 * Une variable privée.
	 * Indique le temps maximal.
	 */
	Uint32 stop = 0;

public:
	// suppression du constructeur de base
	Timer() = delete;

	/**
	 * Un constructeur.
	 * @param end Nombre de secondes jusqu'à l'écoulement du minuteur.
	 * Démarre le minuteur.
	 */
	Timer(const Uint32& end) {
		start = clock(); // gets current time
		stop = start + end;
	}

	/**
	 * @brief Initialise le chrono à @param end après le tic actuel et renvoie vrai si le tic a été dépassé
	 */ 
	bool set(const Uint32& end)  {
		start = clock(); // gets current time

		if (stop == 0)  
			stop = start + end; // inits chrono

		if (start > stop)  {
			start = 0;
			stop = 0;
			return true; // returns 1 if time is elapsed
		}

		return false; // returns 0 in case chrono is still runnning
	}

	/**
	 * Une fonction publique.
	 * Vérifie l'écoulement du minuteur : true si temps écoulé, false sinon.
	 */
	bool checkTimer() {
		start = clock(); // gets current time
		if (start > stop)  {
			start = 0;
			stop = 0;
			return true; // returns 1 if time is elapsed
		}
		return false; // returns 0 in case chrono is still runnning
	}
};
