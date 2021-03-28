
class Mouse {
public:
	int x = 0; // Mouse-X-Position
	int y = 0; // Mouse-Y-Position
	int xmov = 0;
	int ymov = 0;
	bool rightClick = false; // 1 if clic else 0
	bool leftClick = false; // 1 if clic else 0
	
	// Indique si la souris est en mouvement ou non
	bool moving = false; 
	// Réinitialise les valeurs à 0
	void setBack(void) {
		x = y = rightClick = leftClick = moving = 0;
	}
};
