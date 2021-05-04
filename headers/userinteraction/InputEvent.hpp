#pragma once
/**
 * @file Issu du module de Intéraction Utilisateur.
 * S'appuie sur les classes Mouse et Keyboard.
 */

/*===========================================================================================
	INPUTEVENT
===========================================================================================*/

/**
 * @class InputEvent pour gérer les entrées utilisateur et diviser leurs interprétations.
 */
class InputEvent {
	/*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/
private:
	// @brief Event SDL pour suivre les entrées utilisateurs diverses.
	SDL_Event event{};

	// @brief Classe pour enregistrer et mettre à jour les données de la souris.
	Mouse mouse;

	// @brief Classe pour enregistrer et mettre à jour les données du clavier.
	Keyboard keyboard;

	// @brief String pour enregistrer les charactères Unicode sur demande.
	std::string text;

	/*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/
public:
	/**
	 * @brief Constructeur qui appelle les constructeur de mouse et keyboard et qui arrête l'input de texte.
	 */
	InputEvent() { SDL_StopTextInput(); }

	/*===========================================================================================
     *      METHODES
    ===========================================================================================*/
public:
	/**
	 * @brief Renvoie une copie du string text.
	 */
	std::string getText()const { return text; }

	/**
	 * @brief Vide le string text.
	 */
	void clearText() { text.clear(); }
	
	/**
	 * @brief Met à jour la souris passée en argument en copiant les informations de la souris locale.
	 * @param mouse Souris à mettre à jour.
	 */
	void updateMouse(Mouse& mouse)const { mouse = this->mouse; }

	/**
	 * @brief Met à jour le clavier passé en argument en copiant les informations du clavier local.
	 * @param keyboard Clavier à mettre à jour.
	 */
	void updateKeyBoard(Keyboard& keyboard)const { keyboard = this->keyboard; }

	/**
	 * @brief Met à jour tout l'input localement en écrivant dans mouse, keyboard et text.
	 */
	void update() {
		//mouse.xmov = mouse.ymov = 0;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) exit(0); // exit the window
			//text
			if (event.type == SDL_TEXTINPUT) {
				text += event.text.text;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && text.size()) {
				text.pop_back();
			}
			//mouse
			SDL_GetRelativeMouseState(&mouse.xmov, &mouse.ymov);
			if (event.type == SDL_MOUSEMOTION) { // mouse motions
				mouse.moving = true;
				//mouse.xmov = event.motion.x;
				//mouse.ymov = event.motion.y;
				SDL_GetMouseState(&mouse.x, &mouse.y);
			}
			else {
				mouse.moving = false;
				//mouse.xmov = mouse.ymov = 0;
			}
			if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT)) { mouse.leftClick = true; }
			else { mouse.leftClick = false; }
			if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_RIGHT)) { mouse.rightClick = true; }
			else { mouse.rightClick = false; }
			//keyboard
			if (event.type == SDL_KEYDOWN) { // if the user presses a key down
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: keyboard.escape.press(); break;
				case SDLK_LEFT: keyboard.left.press(); break;
				case SDLK_RIGHT: keyboard.right.press(); break;
				case SDLK_UP: keyboard.up.press(); break;
				case SDLK_DOWN: keyboard.down.press(); break;
				case SDLK_SPACE: keyboard.space.press(); break;
				case SDLK_RETURN: keyboard.enter.press(); break;
				case SDLK_DELETE: keyboard.delete_.press(); break;
				case SDLK_QUOTE: keyboard.quote.press(); break;
				case SDLK_COMMA: keyboard.comma.press(); break;
				case SDLK_LSHIFT: keyboard.shift.press(); break;
				case SDLK_RSHIFT: keyboard.shift.press(); break;
				case SDLK_BACKSPACE: keyboard.backspace.press(); break;
				case SDLK_a: keyboard.a.press(); break;
				case SDLK_b: keyboard.b.press(); break;
				case SDLK_c: keyboard.c.press(); break;
				case SDLK_d: keyboard.d.press(); break;
				case SDLK_e: keyboard.e.press(); break;
				case SDLK_f: keyboard.f.press(); break;
				case SDLK_g: keyboard.g.press(); break;
				case SDLK_h: keyboard.h.press(); break;
				case SDLK_i: keyboard.i.press(); break;
				case SDLK_j: keyboard.j.press(); break;
				case SDLK_k: keyboard.k.press(); break;
				case SDLK_l: keyboard.l.press(); break;
				case SDLK_m: keyboard.m.press(); break;
				case SDLK_n: keyboard.n.press(); break;
				case SDLK_o: keyboard.o.press(); break;
				case SDLK_p: keyboard.p.press(); break;
				case SDLK_q: keyboard.q.press(); break;
				case SDLK_r: keyboard.r.press(); break;
				case SDLK_s: keyboard.s.press(); break;
				case SDLK_t: keyboard.t.press(); break;
				case SDLK_u: keyboard.u.press(); break;
				case SDLK_v: keyboard.v.press(); break;
				case SDLK_w: keyboard.w.press(); break;
				case SDLK_x: keyboard.x.press(); break;
				case SDLK_y: keyboard.y.press(); break;
				case SDLK_z: keyboard.z.press(); break;
				case SDLK_0: keyboard.zero.press(); break;
				case SDLK_1: keyboard.one.press(); break;
				case SDLK_2: keyboard.two.press(); break;
				case SDLK_3: keyboard.three.press(); break;
				case SDLK_4: keyboard.four.press(); break;
				case SDLK_5: keyboard.five.press(); break;
				case SDLK_6: keyboard.six.press(); break;
				case SDLK_7: keyboard.seven.press(); break;
				case SDLK_8: keyboard.eight.press(); break;
				case SDLK_9: keyboard.nine.press(); break;
				case SDLK_F1: keyboard.F1.press(); break;
				case SDLK_F2: keyboard.F2.press(); break;
				case SDLK_F3: keyboard.F3.press(); break;
				case SDLK_F4: keyboard.F4.press(); break;
				case SDLK_F5: keyboard.F5.press(); break;
				case SDLK_F6: keyboard.F6.press(); break;
				case SDLK_F7: keyboard.F7.press(); break;
				case SDLK_F8: keyboard.F8.press(); break;
				case SDLK_F9: keyboard.F9.press(); break;
				case SDLK_F10: keyboard.F10.press(); break;
				case SDLK_F11: keyboard.F11.press(); break;
				case SDLK_F12: keyboard.F12.press(); break;
				case SDLK_TAB: keyboard.tab.press(); break;
				case SDLK_PLUS: keyboard.plus.press(); break;
				case SDLK_MINUS: keyboard.minus.press(); break;
				case SDLK_SLASH: keyboard.slash.press(); break;
				case SDLK_BACKSLASH: keyboard.backslash.press(); break;
				case SDLK_EQUALS: keyboard.equal.press(); break;
				case SDLK_DECIMALSEPARATOR: keyboard.point.press(); break;
				case SDLK_SEMICOLON: keyboard.semicolon.press(); break;
				default: break;
				}
			}
			if (event.type == SDL_KEYUP) { // if the user releases a key
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: keyboard.escape.release(); break;
				case SDLK_LEFT: keyboard.left.release(); break;
				case SDLK_RIGHT: keyboard.right.release(); break;
				case SDLK_UP: keyboard.up.release(); break;
				case SDLK_DOWN: keyboard.down.release(); break;
				case SDLK_SPACE: keyboard.space.release(); break;
				case SDLK_RETURN: keyboard.enter.release(); break;
				case SDLK_DELETE: keyboard.delete_.release(); break;
				case SDLK_QUOTE: keyboard.quote.release(); break;
				case SDLK_COMMA: keyboard.comma.release(); break;
				case SDLK_LSHIFT: keyboard.shift.release(); break;
				case SDLK_RSHIFT: keyboard.shift.release(); break;
				case SDLK_BACKSPACE: keyboard.backspace.release(); break;
				case SDLK_a: keyboard.a.release(); keyboard.A.release(); break;
				case SDLK_b: keyboard.b.release(); keyboard.B.release(); break;
				case SDLK_c: keyboard.c.release(); keyboard.C.release(); break;
				case SDLK_d: keyboard.d.release(); keyboard.D.release(); break;
				case SDLK_e: keyboard.e.release(); keyboard.E.release(); break;
				case SDLK_f: keyboard.f.release(); keyboard.F.release(); break;
				case SDLK_g: keyboard.g.release(); keyboard.G.release(); break;
				case SDLK_h: keyboard.h.release(); keyboard.H.release(); break;
				case SDLK_i: keyboard.i.release(); keyboard.I.release(); break;
				case SDLK_j: keyboard.j.release(); keyboard.J.release(); break;
				case SDLK_k: keyboard.k.release(); keyboard.K.release(); break;
				case SDLK_l: keyboard.l.release(); keyboard.L.release(); break;
				case SDLK_m: keyboard.m.release(); keyboard.M.release(); break;
				case SDLK_n: keyboard.n.release(); keyboard.N.release(); break;
				case SDLK_o: keyboard.o.release(); keyboard.O.release(); break;
				case SDLK_p: keyboard.p.release(); keyboard.P.release(); break;
				case SDLK_q: keyboard.q.release(); keyboard.Q.release(); break;
				case SDLK_r: keyboard.r.release(); keyboard.R.release(); break;
				case SDLK_s: keyboard.s.release(); keyboard.S.release(); break;
				case SDLK_t: keyboard.t.release(); keyboard.T.release(); break;
				case SDLK_u: keyboard.u.release(); keyboard.U.release(); break;
				case SDLK_v: keyboard.v.release(); keyboard.V.release(); break;
				case SDLK_w: keyboard.w.release(); keyboard.W.release(); break;
				case SDLK_x: keyboard.x.release(); keyboard.X.release(); break;
				case SDLK_y: keyboard.y.release(); keyboard.Y.release(); break;
				case SDLK_z: keyboard.z.release(); keyboard.Z.release(); break;
				case SDLK_0: keyboard.zero.release(); break;
				case SDLK_1: keyboard.one.release(); break;
				case SDLK_2: keyboard.two.release(); break;
				case SDLK_3: keyboard.three.release(); break;
				case SDLK_4: keyboard.four.release(); break;
				case SDLK_5: keyboard.five.release(); break;
				case SDLK_6: keyboard.six.release(); break;
				case SDLK_7: keyboard.seven.release(); break;
				case SDLK_8: keyboard.eight.release(); break;
				case SDLK_9: keyboard.nine.release(); break;
				case SDLK_F1: keyboard.F1.release(); break;
				case SDLK_F2: keyboard.F2.release(); break;
				case SDLK_F3: keyboard.F3.release(); break;
				case SDLK_F4: keyboard.F4.release(); break;
				case SDLK_F5: keyboard.F5.release(); break;
				case SDLK_F6: keyboard.F6.release(); break;
				case SDLK_F7: keyboard.F7.release(); break;
				case SDLK_F8: keyboard.F8.release(); break;
				case SDLK_F9: keyboard.F9.release(); break;
				case SDLK_F10: keyboard.F10.release(); break;
				case SDLK_F11: keyboard.F11.release(); break;
				case SDLK_F12: keyboard.F12.release(); break;
				case SDLK_TAB: keyboard.tab.release(); break;
				case SDLK_PLUS: keyboard.plus.release(); break;
				case SDLK_MINUS: keyboard.minus.release(); break;
				case SDLK_SLASH: keyboard.slash.release(); break;
				case SDLK_BACKSLASH: keyboard.backslash.release(); break;
				case SDLK_EQUALS: keyboard.equal.release(); break;
				case SDLK_DECIMALSEPARATOR: keyboard.point.release(); break;
				case SDLK_SEMICOLON: keyboard.semicolon.release(); break;
				default: break;
				}
			}
			if (keyboard.shift.pressed) { // if the user presses a key down
				if (keyboard.a.pressed) {
					keyboard.a.release(); keyboard.A.press();
				}
				if (keyboard.b.pressed) {
					keyboard.b.release(); keyboard.B.press();
				}
				if (keyboard.c.pressed) {
					keyboard.c.release(); keyboard.C.press();
				}
				if (keyboard.d.pressed) {
					keyboard.d.release(); keyboard.D.press();
				}
				if (keyboard.e.pressed) {
					keyboard.e.release(); keyboard.E.press();
				}
				if (keyboard.f.pressed) {
					keyboard.f.release(); keyboard.F.press();
				}
				if (keyboard.g.pressed) {
					keyboard.g.release(); keyboard.G.press();
				}
				if (keyboard.h.pressed) {
					keyboard.h.release(); keyboard.H.press();
				}
				if (keyboard.i.pressed) {
					keyboard.i.release(); keyboard.I.press();
				}
				if (keyboard.j.pressed) {
					keyboard.j.release(); keyboard.J.press();
				}
				if (keyboard.k.pressed) {
					keyboard.k.release(); keyboard.K.press();
				}
				if (keyboard.l.pressed) {
					keyboard.l.release(); keyboard.L.press();
				}
				if (keyboard.m.pressed) {
					keyboard.m.release(); keyboard.M.press();
				}
				if (keyboard.n.pressed) {
					keyboard.n.release(); keyboard.N.press();
				}
				if (keyboard.o.pressed) {
					keyboard.o.release(); keyboard.O.press();
				}
				if (keyboard.p.pressed) {
					keyboard.p.release(); keyboard.P.press();
				}
				if (keyboard.q.pressed) {
					keyboard.q.release(); keyboard.Q.press();
				}
				if (keyboard.r.pressed) {
					keyboard.r.release(); keyboard.R.press();
				}
				if (keyboard.s.pressed) {
					keyboard.s.release(); keyboard.S.press();
				}
				if (keyboard.t.pressed) {
					keyboard.t.release(); keyboard.T.press();
				}
				if (keyboard.u.pressed) {
					keyboard.u.release(); keyboard.U.press();
				}
				if (keyboard.v.pressed) {
					keyboard.v.release(); keyboard.V.press();
				}
				if (keyboard.w.pressed) {
					keyboard.w.release(); keyboard.W.press();
				}
				if (keyboard.x.pressed) {
					keyboard.x.release(); keyboard.X.press();
				}
				if (keyboard.y.pressed) {
					keyboard.y.release(); keyboard.Y.press();
				}
				if (keyboard.z.pressed) {
					keyboard.z.release(); keyboard.Z.press();
				}
			}
			else {
				if (keyboard.A.pressed) {
					keyboard.a.press(); keyboard.A.release();
				}
				if (keyboard.B.pressed) {
					keyboard.b.press(); keyboard.B.release();
				}
				if (keyboard.C.pressed) {
					keyboard.c.press(); keyboard.C.release();
				}
				if (keyboard.D.pressed) {
					keyboard.d.press(); keyboard.D.release();
				}
				if (keyboard.E.pressed) {
					keyboard.e.press(); keyboard.E.release();
				}
				if (keyboard.F.pressed) {
					keyboard.f.press(); keyboard.F.release();
				}
				if (keyboard.G.pressed) {
					keyboard.g.press(); keyboard.G.release();
				}
				if (keyboard.H.pressed) {
					keyboard.h.press(); keyboard.H.release();
				}
				if (keyboard.I.pressed) {
					keyboard.i.press(); keyboard.I.release();
				}
				if (keyboard.J.pressed) {
					keyboard.j.press(); keyboard.J.release();
				}
				if (keyboard.K.pressed) {
					keyboard.k.press(); keyboard.K.release();
				}
				if (keyboard.L.pressed) {
					keyboard.l.press(); keyboard.L.release();
				}
				if (keyboard.M.pressed) {
					keyboard.m.press(); keyboard.M.release();
				}
				if (keyboard.N.pressed) {
					keyboard.n.press(); keyboard.N.release();
				}
				if (keyboard.O.pressed) {
					keyboard.o.press(); keyboard.O.release();
				}
				if (keyboard.P.pressed) {
					keyboard.p.press(); keyboard.P.release();
				}
				if (keyboard.Q.pressed) {
					keyboard.q.press(); keyboard.Q.release();
				}
				if (keyboard.R.pressed) {
					keyboard.r.press(); keyboard.R.release();
				}
				if (keyboard.S.pressed) {
					keyboard.s.press(); keyboard.S.release();
				}
				if (keyboard.T.pressed) {
					keyboard.t.press(); keyboard.T.release();
				}
				if (keyboard.U.pressed) {
					keyboard.u.press(); keyboard.U.release();
				}
				if (keyboard.V.pressed) {
					keyboard.v.press(); keyboard.V.release();
				}
				if (keyboard.W.pressed) {
					keyboard.w.press(); keyboard.W.release();
				}
				if (keyboard.X.pressed) {
					keyboard.x.press(); keyboard.X.release();
				}
				if (keyboard.Y.pressed) {
					keyboard.y.press(); keyboard.Y.release();
				}
				if (keyboard.Z.pressed) {
					keyboard.z.press(); keyboard.Z.release();
				}
			}
		}
	}
};