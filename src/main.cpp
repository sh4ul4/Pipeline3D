#include "headers.hpp"

Vector getZoomValue(std::string camId)  {
	if      (!camId.compare("topCam"))    return Vector( 0, -5,  0);
	else if (!camId.compare("faceCam"))   return Vector(-5,  0,  0);
	else if (!camId.compare("backCam"))   return Vector( 5,  0,  0);
	else if (!camId.compare("gaucheCam")) return Vector( 0,  0,  5);
	else if (!camId.compare("droitCam"))  return Vector( 0,  0, -5);
	else								  return Vector( 0,  0,  0);
}

Vector getDezoomValue(std::string camId)  {
	if      (!camId.compare("topCam"))    return Vector( 0,  5,  0);
	else if (!camId.compare("faceCam"))   return Vector( 5,  0,  0);
	else if (!camId.compare("backCam"))   return Vector(-5,  0,  0);
	else if (!camId.compare("gaucheCam")) return Vector( 0,  0, -5);
	else if (!camId.compare("droitCam"))  return Vector( 0,  0,  5);
	else								  return Vector( 0,  0,  0);
}

void zoomCam(void *p)  {
	Camera::getCurrent().moveLockedCam(getZoomValue(Camera::getCurrent().getCamId()));
}

void dezoomCam(void *p)  {
	Camera::getCurrent().moveLockedCam(getDezoomValue(Camera::getCurrent().getCamId()));
}


// Ajouter input pour nom de scène et si vide mettre nom par défaut
void checkInitialization(initPack *i) {
	if (!i->mode)  {
		// Mode avec murs
		if (i->w1.empty() || i->w3.empty()) {
			*i->checkForError = 2;
			return;
		}

		try  {
			stof(i->w1);
			stof(i->w3);
		}  catch (const std::invalid_argument& ia)  {
			*i->checkForError = 1; // Flottant invalide
			// Ajouter un if pour gérer les valeurs max.
			return;
		}
		*i->checkForError = 3; // Flottants valides !
		*i->start = 1;
	}
	else  {
		// Mode importation
		std::string file = FIND_FILE_BUT_DONT_LEAVE(i->import_path);
		if (file.empty())  {
			*i->checkForError = 4; // Path introuvable
			return;
		}
		if (!(file.substr( file.length() - 4 ) == ".3dh"))  {
			*i->checkForError = 5; // Fichier pas d'application
			return;
		}

		*i->checkForError = 6; // Path trouvé
		*i->start = 2;
	}
}

int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	TextBox::initLibrary();
	ButtonManager bm(inputEvent, window);
	ButtonManager bmstart(inputEvent, window);
	Render r(window, 900, 506); // 16:9
	Draw drawer;
	Mouse mouse;
	Keyboard keyboard;

	int start = 0;
	int checkForError = 0;

	TextBox t_intro("Conception d'un aménagement intérieur", "fonts/Calibri Bold.ttf", 40, hd_brownButtons, Point2D<int>(500, 200), 1280, 40, window.getRenderer());
	int i_TLx = 440, i_TLy = 280;
	TextBox t_mur1("Mur 1", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mur1("5 m", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx+60, i_TLy), 60, 25, window.getRenderer());
	TextBox t_mur2("Mur 2", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy+45), 60, 20, window.getRenderer());
	TextInput i_mur2("5 m", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx+60, i_TLy+45), 60, 25, window.getRenderer());

	i_TLx += 280;
	TextBox t_mur3("Mur 3", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mur3("6 m", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx+60, i_TLy), 60, 25, window.getRenderer());
	TextBox t_mur4("Mur 4", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy+45), 60, 20, window.getRenderer());
	TextInput i_mur4("6 m", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx+60, i_TLy+45), 60, 25, window.getRenderer());
	
	TextBox t_error("", "fonts/calibri.ttf", 20, red, Point2D<int>(0, i_TLy+80), 1000, 25, window.getRenderer());

	TextInput scene_name("<nom de scène>", "fonts/calibri.ttf", 21, black, Point2D<int>(440, 390), 400, 25, window.getRenderer());
	bmstart.addRectTextButton<initPack*>("b_initApp", Point2D<int>(440, 415), 400, 40, "Nouvelle scène à partir de la surface donnée");

	TextInput import_path("<chemin d'accès>", "fonts/calibri.ttf", 21, black, Point2D<int>(440, 475), 400, 25, window.getRenderer());
	bmstart.addRectTextButton<initPack*>("b_initImport", Point2D<int>(440, 500), 400, 40, "Importer une scène depuis un fichier");
	initPack i0 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), import_path.getText(), 0 };
	initPack i1 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), import_path.getText(), 1 };
	
	bmstart.getButton<initPack*>("b_initApp").setAction(checkInitialization, &i0);
	bmstart.getButton<initPack*>("b_initImport").setAction(checkInitialization, &i0);

	while (!start) {
		if (keyboard.ctrl.pressed && keyboard.q.down)
			exit(1);

		scene_name.checkForInput(inputEvent, window.getRenderer());
		import_path.checkForInput(inputEvent, window.getRenderer());
		i_mur1.checkForInput(inputEvent, window.getRenderer(), i_mur2);
		i_mur3.checkForInput(inputEvent, window.getRenderer(), i_mur4);

		// Mur 2 et 4 liés aux 1 et 3
		i_mur2.checkForInput(i_mur1, window.getRenderer());
		i_mur4.checkForInput(i_mur3, window.getRenderer());
		inputEvent.update();

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		
		i0 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), import_path.getText(), 0 };
		i1 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), import_path.getText(), 1 };
		bmstart.getButton<initPack*>("b_initApp").setParam(&i0);
		bmstart.getButton<initPack*>("b_initImport").setParam(&i1);
		bmstart.checkButtons();
		bmstart.renderButtons(window.getRenderer());

		scene_name.centerizedRender(Point2D<int>(440, 375), 400, window.getRenderer(), 0);
		import_path.centerizedRender(Point2D<int>(440, 375), 400, window.getRenderer(), 0);
		i_mur1.render(window.getRenderer(), 0);
		i_mur2.render(window.getRenderer(), 0);
		i_mur3.render(window.getRenderer(), 0);
		i_mur4.render(window.getRenderer(), 0);

		t_intro.centerizedRender(Point2D<int>(0,0), 1280, window.getRenderer());
		t_mur1.render(window.getRenderer(), 0, 0);
		t_mur2.render(window.getRenderer(), 0, 0);
		t_mur3.render(window.getRenderer(), 0, 0);
		t_mur4.render(window.getRenderer(), 0, 0);
		if (checkForError)  {
			switch (checkForError)  {
			case 1:
				t_error.update("Merci d'entrer un nombre flottant valide pour vos dimensions !", window.getRenderer());
				break;
			case 2:
				t_error.update("Merci d'entrer des dimensions valides en cliquant sur les barres de saisie !", window.getRenderer());
				break;
			case 3:
				t_error.update("Dimensions valides ! Chargement de la pièce...", "fonts/calibri.ttf", 20, hd_greenButtons, window.getRenderer());
				break;
			case 4:
				t_error.update("Le fichier est introuvable !", window.getRenderer());
				break;
			case 5:
				t_error.update("Le fichier ne correspond pas à un fichier d'application '.3dh' !", window.getRenderer());
				break;
			case 6:
				t_error.update("Fichier localisé ! Chargement de la pièce...", "fonts/calibri.ttf", 20, hd_greenButtons, window.getRenderer());
				break;
			default:
				t_error.update("Erreur innatendue !", window.getRenderer());
				break;
			}
			t_error.center(Point2D<int>(0, i_TLy+90), 1280);
			t_error.render(window.getRenderer(), 0, 0);
		}

		window.RenderScreen();
		window.FillScreen(hd_beigeBackground);
	}

	float w1 = 0, w3 = 0;

	bool mode = true;
	if (start == 1)  {
		w1 = stof(i_mur1.getText());
		w3 = stof(i_mur3.getText());
		if(w3 < w1) std::swap(w1, w3);
		mode = false;
	}
	HomeDesign hm( i0.scene_name, bm, &manager, window, inputEvent, r, mouse, keyboard, w1, w3, i1.import_path, mode);
	

	/** ==============================
	 *  Initialisation caméras
	 * ==============================*/
	TextBox current_cam("Vue du haut : W pour afficher / cacher les murs", "fonts/Calibri Bold.ttf", 20, maroon, Point2D<int>(35, 35), 500, 20, window.getRenderer());
	// Top: -90° / 230° (?) / y -= 5 pour zoomer
	Camera topCam("topCam", {manager.getShape("floor").center.x, manager.getShape("floor").center.y + 150, manager.getShape("floor").center.z}, 60, -1.5708, 4.71239);
	Camera freeCam("freeCam", { 120,300,65 }, 60, 0, 4);
	// -90°, PI (180°) / x -= 5 pour zoomer
	Camera faceCam("faceCam", {manager.getShape("frontWall").center.x + 100, manager.getShape("frontWall").center.y, manager.getShape("frontWall").center.z}, 60, -1.5708, 3.14159);
	// Back: 90°, PI (180°) / x += 5 pour zoomer
	Camera backCam("backCam", {manager.getShape("backWall").center.x - 100, manager.getShape("backWall").center.y, manager.getShape("backWall").center.z}, 60, 1.5708, 3.14159);
	// Gauche: 0, PI / z += 5 pour zoomer
	Camera gaucheCam("gaucheCam", {manager.getShape("leftWall").center.x, manager.getShape("leftWall").center.y, manager.getShape("leftWall").center.z - 100}, 60, 0, 3.1416);
	// Droit: PI, PI / z -= 5 pour zoomer
	Camera droitCam("droitCam", {manager.getShape("rightWall").center.x, manager.getShape("rightWall").center.y, manager.getShape("rightWall").center.z + 100}, 60, 3.14159, 3.14159);
	topCam.lock(); freeCam.lock();  faceCam.lock();  backCam.lock();  gaucheCam.lock();  droitCam.lock();
	// topCam.setControlDragAndDrop(); 

	
	// 3. Boutons interaction globale
	/**
	 * @brief Boutons d'interaction globale avec l'application 
	 */
	int b_width = 270, b_height = 38;
	int b_topleftx = 970, b_tly = 546;
	bm.addRectTextButtonCustom<HomeDesign*>("b_editParam", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Éditer les paramètres de la scène", 16, black);
	bm.getButton<HomeDesign*>("b_editParam").setAction(hm.editSceneParams, &hm);
	b_tly += 58;
	bm.addRectTextButtonCustom<HomeDesign*>("b_saveScene", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Enregistrer la scene", 16, black);
	bm.getButton<HomeDesign*>("b_saveScene").setAction(hm.saveScene, &hm);
	b_tly += 58;
	bm.addRectTextButtonCustom<HomeDesign*>("b_exportView", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Exporter la vue actuelle", 16, black);
	bm.getButton<HomeDesign*>("b_exportView").setAction(hm.exportImage, &hm);
	TextBox sceneTitle(hm.getSceneName(), "fonts/calibri.ttf", 24, black, Point2D<int>(30, 5), 930, 30, window.getRenderer());

	// Liaison caméras / boutons de changement de vues
	camPack p0 = { &topCam, "Vue du haut : W pour afficher / cacher les murs", &current_cam, &window, &manager, "top" };
	camPack p1 = { &faceCam, "Vue de face", &current_cam, &window, &manager, "frontWall" };
	camPack p2 = { &gaucheCam, "Vue de gauche", &current_cam, &window, &manager, "leftWall" };
	camPack p3 = { &droitCam, "Vue de droite", &current_cam, &window, &manager, "rightWall" };
	camPack p4 = { &backCam, "Vue de l'arrière", &current_cam, &window, &manager, "backWall" };
	camPack p5 = { &freeCam, "Déplacement libre", &current_cam, &window, &manager, "none", &r };
	hm.setViewsButtonsAction(p0,p1,p2,p3,p4,p5);
	

	// Boutons Zoom/Dezoom
	bm.addRectTextButton<void*>("b_zoom", Point2D<int>(850, 496), 30, b_height-8, "+"); 
	bm.getButton<void*>("b_zoom").setAction(zoomCam, nullptr);
	bm.addRectTextButton<void*>("b_dezoom", Point2D<int>(890, 496), 30, b_height-8, "-"); 
	bm.getButton<void*>("b_dezoom").setAction(dezoomCam, nullptr);
	
	Texture2D blueSky("HM-Res/blue-sky.jpeg", window.getRenderer());
	Texture2D LOGO("HM-Res/3DHomeProject.png", window.getRenderer());

	r.updateTriangles(manager);
	bool wbool = true, meshbool = false, depthbool = false;
	while (!(keyboard.ctrl.pressed && keyboard.q.down)) {
		inputEvent.update();
		r.updateTriangles(manager);

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		if (keyboard.escape.down) {
			if (!Camera::getCurrent().locked)	Camera::getCurrent().lock();
			// if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			// else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}

		if (HomeDesign::interactSpace != 4)  {// Hors mode insertion meuble
			// Mouvement avec flèches
			if (keyboard.left.pressed) 
				hm.moveCamera(0);
			if (keyboard.right.pressed) 
				hm.moveCamera(1);
			if (keyboard.down.pressed) 
				hm.moveCamera(2);
			if (keyboard.up.pressed) 
				hm.moveCamera(3);
			
			// Ajouter ici déplacement de caméra par touches fléchées
		}

		// Fond de pipeline et rectangle espace interactions
		// drawer.DrawFillRoundedRectContoured(Point2D<int>(30,30), 900, 506, 5, hd_blueSky, black, window.getRenderer());
		blueSky.render(window.getRenderer(), Point2D<int>(30,30), 900, 506);
		LOGO.render(window.getRenderer(), Point2D<int>(800,2), 130, 25);
		drawer.DrawFillRoundedRectContoured(Point2D<int>(970, 30), 270, 506, 3, white, black, window.getRenderer());

		// if (Camera::getCurrent().locked) r.renderStatic(Point2D<int>(30,30),900,506, window);
		// else r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);
		if (meshbool)
			r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager, 0, false, true);
		else if (depthbool)
			r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager, 0, true, false);
		else
			r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);
		// Print camera position and angle
		// std::cout<<Camera::getCurrent().getCameraPosition()<< " "<<Camera::getCurrent().angleX<<" "<<Camera::getCurrent().angleY<<std::endl;

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		// Render des boutons de vues
		hm.renderViewsButtons(window.getRenderer());
		r.renderOrientation(Point2D<int>(50, 470), 50, window);

		sceneTitle.update(hm.getSceneName(), window.getRenderer());
		sceneTitle.render(window.getRenderer(), 0, 0);
		
		// Check de clic souris si on est en vue du haut
		if (Camera::getCurrent().getCamId() != "freeCam")
			hm.checkFurnitureClick(mouse, manager, window);

		switch (HomeDesign::interactSpace)  {
			case 1:
				hm.renderInsertion1(inputEvent, window);
				break;
			case 2:
				hm.renderInsertion2(inputEvent, window);
				break;
			case 3: // Insertion Obj
				hm.renderObjInsertionSpace(inputEvent, window);
				break;
			case 4:
				// Clic sur un meuble !
				hm.renderFurnitureInteractionSpace(inputEvent, window, manager);
				manager.getShape(hm.getSelectedShape()).drawHit2D(Point2D<int>(30,30), window);

				// Déplacement du meuble par touches directionnelles
				if (keyboard.left.pressed) 
					hm.moveFurniture(manager, keyboard, 0);
				if (keyboard.right.pressed) 
					hm.moveFurniture(manager, keyboard, 1);
				if (keyboard.down.pressed) 
					hm.moveFurniture(manager, keyboard, 2);
				if (keyboard.up.pressed) 
					hm.moveFurniture(manager, keyboard, 3);
				break;
			default:
				// Render infos sur la scène actuelle
				hm.renderDefault(inputEvent, window);

				// Changement de caméra
				if (keyboard.one.down) 
					switchCam(&p0);
				else if (keyboard.two.down)
					switchCam(&p1);
				else if (keyboard.three.down)
					switchCam(&p2);
				else if (keyboard.four.down)
					switchCam(&p3);
				else if (keyboard.five.down)
					switchCam(&p4);
				else if (keyboard.w.down && Camera::getCurrent().getCamId() == "topCam")  {
					editWallsVisibility(manager, wbool);
					wbool = 1 - wbool;
					manager.pushShapesEditing();
				}
				else if (keyboard.m.down)  {
					meshbool = 1 - meshbool;
					depthbool = 0;
				}
				else if (keyboard.d.down)  {
					depthbool = 1 - depthbool;
					meshbool = 0;
				}
				// else if (keyboard.F11.down) {
				// 	window.ToggleWindow(window.getWidth(), window.getHeight());
				// }
				break;
		}

		current_cam.render(window.getRenderer(), 0, 0);

		window.RenderScreen();
		window.FillScreen(hd_beigeBackground);
	}

	hm.saveScene(&hm);
	std::cout << "Application quittée, la scène '"+ hm.getSceneName() +"' a été sauvegardée automatiquement.\n";
	return 0;
}
