class InputEvent {
public:
	InputEvent() { SDL_StopTextInput(); }
	SDL_Event event;
	Mouse mouse;
	Keyboard keyboard;
	std::string text;
	void updateMouse(Mouse& mouse) { mouse = this->mouse; }
	void updateKeyBoard(Keyboard& keyboard) { keyboard = this->keyboard; }
	void update();
};