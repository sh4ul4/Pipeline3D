#pragma once

/**
 * @class Chronomètre
 * @brief Crée un chronomètre avec l'heure de début et dans une fonction retourne le temps écoulé
 */

class Chrono {
private:
	Uint32 start = NULL; // temps début chrono
	Uint32 now = NULL; // temps chrono actuel
public:
	// Définie le début du chronomètre
	Uint32 setStartChrono(const Uint32& end){ }

	// Retourne le temps écoulé du chronomètre
	Float getTimeElapsed(void){ }
};