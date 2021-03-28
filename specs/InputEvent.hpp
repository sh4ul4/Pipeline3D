class InputEvent {	// Enregistre les actions de l'utilisateur sur la souris et le clavier
public:
	SDL_Event event;
	Mouse mouse;
	KeyBoard keyboard;
	
	// Constructeur
	InputEvent() {
		update();
	}
	
	//Met à jour la valeur suivant l'action faite par l'utilisateur
	void update();
};
