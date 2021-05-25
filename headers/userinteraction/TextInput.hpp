#pragma once
/**
 * @file Issu du module de Intéraction Utilisateur.
 * Extension de la classe TextBox.
 */

/*===========================================================================================
	TEXTINPUT
===========================================================================================*/

/**
 * @class TextInput permet de gérer l'input dynamique de texte.
 */
class TextInput : public TextBox {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
private:
	// @brief copie locale de l'état des touches du clavier.
	Mouse mouse;

	// @brief copie locale de l'état des touches du clavier.
	Keyboard keyboard;

	// @brief Texte mis à jour avec l'input.
	std::string text;

	// @brief Indique si la classe est en train d'enregistrer l'input utilisateur.
	bool running = false;

	/*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/
public:
	/**
	 * @brief Constructeur qui construit l'objet TextBox par polymorphisme.
	 * @param text Texte de départ.
	 * @param fontPath Chemin d'accès vers la police dans la mémoire.
	 * @param fontSize Taille de la police.
	 * @param fontColor Couleur de la police.
	 * @param topLeft Point haut-gauche de la texture résultante.
	 * @param renderer Renderer SDL.
	 */
	TextInput(const std::string& text, const std::string& fontPath, const int& fontSize, const Color& fontColor,
		const Point2D<int>& topLeft, const int& width, const int& height, SDL_Renderer* renderer)
		: TextBox(text, FIND_FILE(fontPath),fontSize,fontColor,Point2D<int>(topLeft + 2),width - 4,height - 4,renderer), text(text) {}

	// suppression du constructeur par défaut
	TextInput() = delete;

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Afficher la texture du texte.
	 * @param renderer Renderer SDL
	 * @param flip Application de l'effet miroir (horizontal ou vertical)
	 * @param angle Rotation de la texture lors de l'affichage
	 */
	void render(SDL_Renderer* renderer, const size_t DRAWTYPE = DRAWFILLCONTOURRECT) const {
		switch (DRAWTYPE) {
		case DRAWRECT: if (running) Draw::DrawRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, dark_gray, renderer);
					 else Draw::DrawRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, Color(30, 30, 30), renderer); break;
		case DRAWFILLRECT: if (running) Draw::DrawFillRect(pos - 2, maxWidth + 4, maxHeight + 4, light_gray, renderer);
					 else Draw::DrawFillRect(pos - 2, maxWidth + 4, maxHeight + 4, gray, renderer); break;
		case DRAWFILLCONTOURRECT: if (running) Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, light_gray, Color(30,30,30), renderer);
								else Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, gray, Color(30, 30, 30), renderer); break;
		case DRAWFILLROUNDEDCONTOURRECT: if (running) Draw::DrawFillRoundedRectContoured(pos - 2, maxWidth + 4, maxHeight + 4, 3, light_gray, Color(30, 30, 30), renderer);
									   else Draw::DrawFillRoundedRectContoured(pos - 2, maxWidth + 4, maxHeight + 4, 3, gray, Color(30, 30, 30), renderer); break;
		default : if (running) Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, light_gray, Color(30, 30, 30), renderer);
				else Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, gray, Color(30, 30, 30), renderer); break;
		}
		if (!text.empty()) TextBox::render(renderer);
	}

	void centerizedRender(Point2D<int> tl, int maxwidth, SDL_Renderer* renderer, const size_t DRAWTYPE = DRAWFILLCONTOURRECT) const {
		switch (DRAWTYPE) {
		case DRAWRECT: if (running) Draw::DrawRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, dark_gray, renderer);
					 else Draw::DrawRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, Color(30, 30, 30), renderer); break;
		case DRAWFILLRECT: if (running) Draw::DrawFillRect(pos - 2, maxWidth + 4, maxHeight + 4, light_gray, renderer);
					 else Draw::DrawFillRect(pos - 2, maxWidth + 4, maxHeight + 4, gray, renderer); break;
		case DRAWFILLCONTOURRECT: if (running) Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, light_gray, Color(30,30,30), renderer);
								else Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, gray, Color(30, 30, 30), renderer); break;
		case DRAWFILLROUNDEDCONTOURRECT: if (running) Draw::DrawFillRoundedRectContoured(pos - 2, maxWidth + 4, maxHeight + 4, 3, light_gray, Color(30, 30, 30), renderer);
									   else Draw::DrawFillRoundedRectContoured(pos - 2, maxWidth + 4, maxHeight + 4, 3, gray, Color(30, 30, 30), renderer); break;
		default : if (running) Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, light_gray, Color(30, 30, 30), renderer);
				else Draw::DrawFillContouredRect(pos - 2, maxWidth + 4, maxHeight + 4, 1, gray, Color(30, 30, 30), renderer); break;
		}
		if (!text.empty()) TextBox::centerizedRender(tl, maxwidth, renderer);
	}

	/**
	 * @brief Démarrer l'enregistrement de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 */
	void start(InputEvent& ie) {
		running = true;
		ie.clearText();
		SDL_StartTextInput();
	}

	/**
	 * @brief Arrêter l'enregistrement de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 */
	void stop(InputEvent& ie) {
		running = false;
		ie.clearText();
		SDL_StopTextInput();
	}

	bool checkClick(Mouse mouse)  {
		return mouse.x < pos.x + maxWidth && mouse.x > pos.x && mouse.y < pos.y + maxHeight && mouse.y > pos.y;
	}

	/**
	 * @brief Mettre à jour le texte et son affichage en fonction de l'input utilisateur.
	 * @param ie Event pour suivre l'input utilisateur.
	 * @param renderer Renderer SDL
	 */
	void checkForInput(InputEvent& ie, SDL_Renderer* renderer) {
		ie.updateMouse(mouse);
		ie.updateKeyBoard(keyboard);
		if (!running && mouse.leftClick && checkClick(mouse))
			start(ie);
		if ( (running && keyboard.enter.down) || (running && mouse.leftClick && !checkClick(mouse)) )
			stop(ie);
		if (!running)return;
		const int size1 = text.length();
		text = ie.getText();
		const int size2 = text.length();
		if (size1 != size2 && !text.empty()) {
			update(text, renderer);
		}
	}

	/**
	 * @brief Cas textInput liée à une autre
	 * 
	 * @param other textInput fille liée à la courante
	 */
	void checkForInput(InputEvent& ie, SDL_Renderer* renderer, TextInput& other) {
		ie.updateMouse(mouse);
		ie.updateKeyBoard(keyboard);
		if (!running && mouse.leftClick && ( checkClick(mouse) || other.checkClick(mouse) ))
			start(ie);
		if ( (running && keyboard.enter.down) || (running && mouse.leftClick && !checkClick(mouse) && !other.checkClick(mouse)) )
			stop(ie);
		if (!running)return;
		const int size1 = text.length();
		text = ie.getText();
		const int size2 = text.length();
		if (size1 != size2 && !text.empty()) {
			update(text, renderer);
		}
	}

	/**
	 * @brief TextInput fille dépendante d'une autre
	 * 
	 * @param master TextInput maitre de référence
	 */
	void checkForInput(TextInput& master, SDL_Renderer* renderer) {
		if (master.running)
			running = true;
		else
			running = false;

		const int size1 = text.length();
		text = master.getText();
		const int size2 = text.length();
		if (size1 != size2 && !text.empty()) {
			update(text, renderer);
		}
	}

	std::string getText()const {
		return text;
	}
};