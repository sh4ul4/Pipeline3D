template <class paramType>
class Button {
public:
	const std::string name;
protected:
	Mouse mouse;
	bool clicked = false; // la souris a cliqué dans la zone selectionnable
	bool selected = false; // la souris se trouve dans la zone selectionnable
	bool visible = false;
	Texture2D* backgroundTex = nullptr;
	Color backgroundCol;
	Color contourCol;
	TextBox* textBox = nullptr;

	void (*action)(void) = nullptr;

	void (*fun)(paramType t) = nullptr;

	bool* signal = nullptr;
public:
	void setSignal(bool& signal) {
		signal = false;
		this->signal = &signal;
	}

	void removeSignal() {
		signal = nullptr;
	}

	bool isClicked()const { return clicked; }
	bool isSelected()const { return selected; }

	template <typename paramType>
	void setAction(void (*func)(paramType p)) {
		fun = func;
	}
	void playAction(paramType param) {
		std::cout << "play param" << std::endl;
		if (fun != nullptr) fun(param);
	}

	void setAction(void (*func)(void)) {
		action = func;
	}

	void playAction() {
		std::cout << "play" << std::endl;
		if (action != nullptr) action();
	}

	virtual void render(SDL_Renderer* renderer) const {}

	virtual bool mouseInside(const InputEvent& ie) { return false; }

	virtual bool mouseClickInside(const InputEvent& ie) {
		ie.updateMouse(mouse);
		clicked = mouseInside(ie) && mouse.leftClick;
		return clicked;
	}

	void checkButton(const InputEvent& inputEvent) {
		if (!clicked && mouseClickInside(inputEvent)) {
			playAction();
			*signal = true;
		}
		else mouseClickInside(inputEvent);
	}

protected:
	Button(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb)
		: name(name), backgroundTex(bgTex), backgroundCol(bgCol), contourCol(contCol), textBox(tb) {}

	Button() = delete;
};

template <class paramType>
class RectButton : public Button<paramType> {
public:
	Point2D pos;
	int width;
	int height;

	RectButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb,
		const Point2D& pos, const int& width, const int& height)
		: Button(name, bgTex, bgCol, contCol, tb),
		pos(pos), width(width), height(height) {}

	RectButton() = delete;

	void render(SDL_Renderer* renderer) const {
		SDL_Rect rect{ pos.x, pos.y, width, height };
		if (backgroundTex) backgroundTex->render(renderer, 0, 0);
		else {
			SDL_SetRenderDrawColor(renderer, backgroundCol.r, backgroundCol.g, backgroundCol.b, backgroundCol.a);
			SDL_RenderFillRect(renderer, &rect);
		}
		if (textBox) textBox->render(renderer, 0, 0);
		SDL_SetRenderDrawColor(renderer, contourCol.r, contourCol.g, contourCol.b, contourCol.a);
		SDL_RenderDrawRect(renderer, &rect);
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(mouse);
		const Point2D m(mouse.x, mouse.y);
		selected = m.x < pos.x + width && m.x > pos.x && m.y < pos.y + height && m.y > pos.y;
		return selected;
	}
};

template <class paramType>
class RoundButton : public Button<paramType> {
public:
	Point2D pos;
	int radius;

	RoundButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb,
		const Point2D& pos, const int& radius)
		: Button(name, bgTex, bgCol, contCol, tb),
		pos(pos), radius(radius) {}

	RoundButton() = delete;

	void render(SDL_Renderer* renderer) const {
		if (backgroundTex) backgroundTex->render(renderer, 0, 0);
		else {
			Draw::DrawFillCircle(pos.x, pos.y, radius, backgroundCol, renderer);
		}
		if (textBox) textBox->render(renderer, 0, 0);
		Draw::DrawCircle(pos.x, pos.y, radius, contourCol, renderer);
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(mouse);
		const Point2D mouse(mouse.x, mouse.y);
		selected = mouse.distance(pos) < radius;
		return selected;
	}
};

class ButtonManager {
private:
	const InputEvent& inputEvent;

	bool nameUsed(const std::string& name) const {
		for (int i = 0; i < buttons.size(); i++)
			if (!buttons[i]->name.compare(name)) return true;
		return false;
	}
public:
	ButtonManager(const InputEvent& inputEvent) : inputEvent(inputEvent) {}
	ButtonManager() = delete;

	std::vector<std::unique_ptr<Button<int>>> buttons;

	void addButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb, const Point2D& pos, const int& width, const int& height) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new RectButton<int>(name, bgTex, bgCol, contCol, tb, pos, width, height));
	}

	void addButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb, const Point2D& pos, const int& radius) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new RoundButton<int>(name, bgTex, bgCol, contCol, tb, pos, radius));
	}

	void renderButtons(SDL_Renderer* renderer) const {
		for (int i = 0; i < buttons.size(); i++) buttons[i]->render(renderer);
	}

	void checkButtons() {
		for (int i = 0; i < buttons.size(); i++) {
			if (!buttons[i]->isClicked() && buttons[i]->mouseClickInside(inputEvent)) buttons[i]->playAction();
			else buttons[i]->mouseClickInside(inputEvent);
		}
	}

	Button<int>& getButton(const std::string& name) {
		for (int i = 0; i < buttons.size(); i++)
			if (!buttons[i]->name.compare(name)) return *buttons[i];
		std::cout << "There is no Button named " << name << std::endl;
		exit(1);
	}
};