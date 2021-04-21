#pragma once

// base-class of a button without the template parameters
class ButtonBase {
public:
	const std::string name;
protected:
	// non-template parameters
	Mouse mouse;
	bool clicked = false; // la souris a cliqué dans la zone selectionnable
	bool selected = false; // la souris se trouve dans la zone selectionnable
	bool visible = false;
	Texture2D* backgroundTex = nullptr;
	Color backgroundCol;
	Color contourCol;
	TextBox* textBox = nullptr;
	bool* signal = nullptr;
public:
	// virtual functions declarations
	virtual void setSignal(bool& signal) {}
	virtual void removeSignal() {}
	virtual bool isClicked()const { return clicked; }
	virtual bool isSelected()const { return selected; }
	virtual void playAction() {}
	virtual void render(SDL_Renderer* renderer) const {}
	virtual bool mouseInside(const InputEvent& ie) { return false; }
	virtual bool mouseClickInside(const InputEvent& ie) { return false; }
	virtual void checkButton(const InputEvent& inputEvent) {}
protected:
	// constructor only accessible via derived button classes
	ButtonBase(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb)
		: name(name), backgroundTex(bgTex), backgroundCol(bgCol), contourCol(contCol), textBox(tb) {}
	ButtonBase() = delete;
};

// base-class-extension for a button with template parameters and function delcarations
template <class paramType>
class Button : public ButtonBase {
protected:
	// template function pointer with parameter
	void (*fun)(paramType t) = nullptr;

	// template parameter
	paramType param = NULL;
public:
	// give a reference to a boolean that works as a signal
	void setSignal(bool& signal) {
		signal = false;
		this->signal = &signal;
	}

	// remove the signal pointer for the current button
	void removeSignal() {
		signal = nullptr;
	}

	// returns value of clicked without updating it
	virtual bool isClicked() const { return clicked; }

	// returns value of selected without updating it
	bool isSelected() const { return selected; }

	// give a function pointer and a parameter that can be called by the ButtonManager
	void setAction(void (*func)(paramType p), paramType parameter) {
		param = parameter;
		fun = func;
	}

	// give a function pointer that can be called by the ButtonManager
	void setAction(void (*func)(paramType p)) {
		param = nullptr;
		fun = func;
	}

	// set the parameter for the current function pointer
	void setParam(paramType parameter) {
		param = parameter;
	}

	// call the function pointed to by fun with a given parameter
	void playAction(paramType param) const {
		if (fun != nullptr && param != nullptr) fun(param);
	}

	// call the function pointed to by fun with the stocked parameter
	void playAction() {
		if (fun != nullptr) fun(param);
	}

	// render the button depending on its state
	virtual void render(SDL_Renderer* renderer) const {}

	// check if the mouse is inside the button-zone
	virtual bool mouseInside(const InputEvent& ie) { return false; }

	// check if the mouse clicked inside the button-zone
	virtual bool mouseClickInside(const InputEvent& ie) {
		ie.updateMouse(mouse);
		clicked = mouseInside(ie) && mouse.leftClick;
		return clicked;
	}

	// check if the mouse clicked and/or is inside the button-zone and handle accordingly
	virtual void checkButton(const InputEvent& inputEvent) {
		if (!clicked && mouseClickInside(inputEvent)) {
			playAction();
			if(signal) *signal = true;
		}
		else mouseClickInside(inputEvent);
	}

protected:
	// constructor only accessible via derived button classes
	Button(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb)
		: ButtonBase(name,bgTex,bgCol,contCol,tb) {}

	Button() = delete;
};

template <class paramType>
class RectButton : public Button<paramType> {
public:
	Point2D<int> pos;
	int width;
	int height;

	RectButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb,
		const Point2D<int>& pos, const int& width, const int& height) : Button<paramType>(name, bgTex, bgCol, contCol, tb),
		pos(pos), width(width), height(height) {}

	RectButton() = delete;

	void render(SDL_Renderer* renderer) const {
		if (ButtonBase::backgroundTex) ButtonBase::backgroundTex->render(renderer, 0, 0);
		else {
			Color bg = ButtonBase::backgroundCol;
			if (ButtonBase::selected) {
				bg.r = Maths::concat(20, bg.r);
				bg.g = Maths::concat(20, bg.g);
				bg.b = Maths::concat(20, bg.b);
			}
			Draw::DrawFillRoundedRectContoured(pos, width, height, 6, bg, ButtonBase::contourCol, renderer);
		}
		if (ButtonBase::textBox) {
			const Point2D<int> center(pos.x + width / 2, pos.y + height / 2);
			const Point2D<int> textBoxPos(center.x - ButtonBase::textBox->width / 2, center.y - ButtonBase::textBox->height / 2);
			ButtonBase::textBox->render(renderer, textBoxPos, 0, 0);
		}
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(ButtonBase::mouse);
		const Point2D<int> m(ButtonBase::mouse.x, ButtonBase::mouse.y);
		ButtonBase::selected = m.x < pos.x + width && m.x > pos.x && m.y < pos.y + height && m.y > pos.y;
		return ButtonBase::selected;
	}
};

template <class paramType>
class RectTextButton : public Button<paramType> {
public:
	Point2D<int> pos;
	int width;
	int height;

	RectTextButton(const std::string& name, const Point2D<int>& pos, const int& width, const int& height, const std::string& text, const Window& window)
		: Button<paramType>(name, nullptr, dark_gray, black,
			new TextBox(text, pth + std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(0, 0), window.getRenderer())),
		pos(pos), width(width), height(height) {}

	RectTextButton() = delete;

	~RectTextButton() { delete tb; }

	void render(SDL_Renderer* renderer) const {
		if (ButtonBase::backgroundTex) ButtonBase::backgroundTex->render(renderer, 0, 0);
		else {
			Color bg = ButtonBase::backgroundCol;
			if (ButtonBase::selected) {
				bg.r = Maths::concat(20, bg.r);
				bg.g = Maths::concat(20, bg.g);
				bg.b = Maths::concat(20, bg.b);
			}
			Draw::DrawFillRoundedRectContoured(pos, width, height, 6, bg, ButtonBase::contourCol, renderer);
		}
		if (ButtonBase::textBox) {
			const Point2D<int> center(pos.x + width / 2, pos.y + height / 2);
			const Point2D<int> textBoxPos(center.x - ButtonBase::textBox->width / 2, center.y - ButtonBase::textBox->height / 2);
			ButtonBase::textBox->render(renderer, textBoxPos, 0, 0);
		}
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(ButtonBase::mouse);
		const Point2D<int> m(ButtonBase::mouse.x, ButtonBase::mouse.y);
		ButtonBase::selected = m.x < pos.x + width && m.x > pos.x && m.y < pos.y + height && m.y > pos.y;
		return ButtonBase::selected;
	}
};

template <class paramType>
class CheckBox : public Button<paramType> {
public:
	Point2D<int> pos;
	int size;
	bool checked = false;

	CheckBox(const std::string& name, const Color& bgCol, const Color& contCol,
		const Point2D<int>& pos, const int& size) : Button<paramType>(name, nullptr, bgCol, contCol, nullptr),
		pos(pos), size(size) {}

	CheckBox() = delete;

	void render(SDL_Renderer* renderer) const {
		Color bg = ButtonBase::backgroundCol;
		if (checked) {
			bg.r = Maths::concat(-40, bg.r);
			bg.g = Maths::concat(-40, bg.g);
			bg.b = Maths::concat(-40, bg.b);
		}
		if (ButtonBase::selected) {
			bg.r = Maths::concat(20, bg.r);
			bg.g = Maths::concat(20, bg.g);
			bg.b = Maths::concat(20, bg.b);
		}
		Draw::DrawFillRoundedRectContoured(pos, size, size, 6, bg, ButtonBase::contourCol, renderer);
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(ButtonBase::mouse);
		const Point2D<int> m(ButtonBase::mouse.x, ButtonBase::mouse.y);
		ButtonBase::selected = m.x < pos.x + size && m.x > pos.x && m.y < pos.y + size && m.y > pos.y;
		return ButtonBase::selected;
	}

	bool mouseClickInside(const InputEvent& ie) {
		ie.updateMouse(ButtonBase::mouse);
		ButtonBase::clicked = mouseInside(ie) && ButtonBase::mouse.leftClick;
		return ButtonBase::clicked;
	}

	// check if the mouse clicked and/or is inside the button-zone and handle accordingly
	void checkButton(const InputEvent& inputEvent) {
		if (!ButtonBase::clicked && mouseClickInside(inputEvent)) {
			ButtonBase::playAction();
			if (ButtonBase::signal) *ButtonBase::signal = true;
			this->checked = !this->checked;
		}
		else mouseClickInside(inputEvent);
	}

	bool isClicked() const { return checked; }
};

template <class paramType>
class RoundButton : public Button<paramType> {
public:
	Point2D<int> pos;
	int radius;

	RoundButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb,
		const Point2D<int>& pos, const int& radius)
		: Button<paramType>(name, bgTex, bgCol, contCol, tb),
		pos(pos), radius(radius) {}

	RoundButton() = delete;

	void render(SDL_Renderer* renderer) const {
		if (ButtonBase::backgroundTex) ButtonBase::backgroundTex->render(renderer, 0, 0);
		else {
			Draw::DrawFillCircle(pos.x, pos.y, radius, ButtonBase::backgroundCol, renderer);
		}
		if (ButtonBase::textBox) ButtonBase::textBox->render(renderer, 0, 0);
		Draw::DrawCircle(pos.x, pos.y, radius, ButtonBase::contourCol, renderer);
	}

	bool mouseInside(const InputEvent& ie) {
		ie.updateMouse(ButtonBase::mouse);
		const Point2D<int> mouse2(ButtonBase::mouse.x, ButtonBase::mouse.y);
		ButtonBase::selected = mouse2.distance(pos) < radius;
		return ButtonBase::selected;
	}
};

class ButtonManager {
private:
	const InputEvent& inputEvent;
	const Window& window;

	std::vector<std::unique_ptr<ButtonBase>> buttons;
public:
	ButtonManager(const InputEvent& inputEvent, const Window& window) : inputEvent(inputEvent),window(window) {}
	ButtonManager() = delete;
private:
	bool nameUsed(const std::string& name) const {
		for (int i = 0; i < buttons.size(); i++)
			if (!buttons[i]->name.compare(name)) return true;
		return false;
	}
public:
	template <class paramType>
	void addRectButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb, const Point2D<int>& pos, const int& width, const int& height) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new RectButton<paramType>(name, bgTex, bgCol, contCol, tb, pos, width, height));
	}

	template <class paramType>
	void addCheckBox(const std::string& name, const Color& bgCol, const Color& contCol, const Point2D<int>& pos, const int& size) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new CheckBox<paramType>(name, bgCol, contCol, pos, size));
	}

	template <class paramType>
	void addRoundButton(const std::string& name, Texture2D* bgTex, const Color& bgCol, const Color& contCol, TextBox* tb, const Point2D<int>& pos, const int& radius) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new RoundButton<paramType>(name, bgTex, bgCol, contCol, tb, pos, radius));
	}

	template <class paramType>
	void addRectTextButton(const std::string& name, const Point2D<int>& pos, const int& width, const int& height, const std::string& text) {
		if (nameUsed(name))std::cout << "Warning : A Button named " << name << " already exists" << std::endl;
		buttons.emplace_back(new RectTextButton<paramType>(name, pos, width, height, text, window));
	}

	void removeButton(const std::string& name) {
		for (int i = 0; i < buttons.size(); i++)
			if (!buttons[i]->name.compare(name)) {
				buttons.erase(buttons.begin() + i);
				buttons.shrink_to_fit();
				return;
			}
		std::cout << "Warning : A Button named " << name << " does not exist" << std::endl;
	}

	void renderButtons(SDL_Renderer* renderer) const {
		for (int i = 0; i < buttons.size(); i++) buttons[i]->render(renderer);
	}

	void checkButtons() const {
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i]->checkButton(inputEvent);
			//if (!buttons[i]->isClicked() && buttons[i]->mouseClickInside(inputEvent)) buttons[i]->playAction();
			//else buttons[i]->mouseClickInside(inputEvent);
		}
	}

	template <class paramType>
	Button<paramType>& getButton(const std::string& name) const {
		ASSERT(nameUsed(name), "There is no Button with this name.\n");
		for (int i = 0; i < buttons.size(); i++)
			if (!buttons[i]->name.compare(name)) return dynamic_cast<Button<paramType>&>(*buttons[i]);
	}
};