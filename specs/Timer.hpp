#pragma once

/**
 * @class Chrono
 * @brief Crée un Minuteur qui sur demande renvoie si le temps demandé est écoulé ou non
 */
class Chrono {
private:
	Uint32 start = NULL;
	Uint32 stop = NULL;
public:
	// Définit l'heure d'arrêt du minuteur
	Uint32 setEndChrono(const Uint32& end);
	
	// Vérifie si le minuteur est fini
	bool isTimerEnded();
};