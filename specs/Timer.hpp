#pragma once

/**
 * @brief Minuteur
 * Crée un Minuteur qui sur demande renvoie si le temps demandé est écoulé ou non
 */
class Chrono {
private:
	Uint32 start = NULL;
	Uint32 stop = NULL;
public:
	Uint32 setEndChrono(const Uint32& end){ // Définit l'heure d'arrêt du minuteur
	}
	bool isTimerEnded(void) { // Vérifie si le minuteur est fini
	}
};