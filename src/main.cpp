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


struct initPack {
	bool *start;
	int *checkForError;
	std::string w1;
	std::string w3;
};

// Ajouter input pour nom de scène et si vide mettre nom par défaut
void checkInitialization(initPack *i) {
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
	*i->start = true;
}

int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	TextBox::initLibrary();
	ButtonManager bm(inputEvent, window);
	Render r(window, 900, 506); // 16:9
	Draw drawer;
	Mouse mouse;
	Keyboard keyboard;

	bool start = false;
	int checkForError = 0;

	TextBox t_intro("CROUS Simulator", "fonts/calibri.ttf", 40, black, Point2D<int>(500, 260), 400, 40, window.getRenderer());
	int i_TLx = 440, i_TLy = 310;
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

	bm.addRectTextButton<initPack*>("b_initApp", Point2D<int>(440, 415), 400, 40, "Nouvelle scène à partir de la surface donnée");
	bm.addRectTextButton<int*>("b_initImport", Point2D<int>(440, 475), 400, 40, "Importer une scène depuis un fichier");
	initPack i0 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText() };
	bm.getButton<initPack*>("b_initApp").setAction(checkInitialization, &i0);

	while (!start && !keyboard.escape.down) {
		i_mur1.checkForInput(inputEvent, window.getRenderer(), i_mur2);
		i_mur3.checkForInput(inputEvent, window.getRenderer(), i_mur4);

		// Mur 2 et 4 liés aux 1 et 3
		i_mur2.checkForInput(i_mur1, window.getRenderer());
		i_mur4.checkForInput(i_mur3, window.getRenderer());
		inputEvent.update();

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		
		i0 = { &start, &checkForError, i_mur1.getText(), i_mur3.getText() };
		bm.getButton<initPack*>("b_initApp").setParam(&i0);
		bm.checkButtons();
		bm.renderButtons(window.getRenderer());

		i_mur1.render(window.getRenderer(), 0);
		i_mur2.render(window.getRenderer(), 0);
		i_mur3.render(window.getRenderer(), 0);
		i_mur4.render(window.getRenderer(), 0);

		t_intro.render(window.getRenderer(), 0, 0);
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
	bm.removeButton("b_initApp");
	bm.removeButton("b_initImport");

	float w1, w3;
	w1 = stof(i_mur1.getText());
	w3 = stof(i_mur3.getText());
	if(w3 < w1) std::swap(w1, w3);
	HomeDesign hm( "MaChambreCROUS", bm, &manager, window, inputEvent, w1, w3 );
	

	/** ==============================
	 *  Initialisation caméras
	 * ==============================*/
	TextBox current_cam("Vue du haut", "fonts/calibri.ttf", 20, black, Point2D<int>(35, 35), 400, 20, window.getRenderer());
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
	bm.addRectTextButtonCustom<camPack*>("b_editSurface", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Editer la surface de la scene", 16, black);
	// bm.getButton<TextInput*>("b_editSurface").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);
	b_tly += 58;
	bm.addRectTextButtonCustom<camPack*>("b_saveScene", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Enregistrer la scene", 16, black);
	b_tly += 58;
	bm.addRectTextButtonCustom<Render*>("b_exportView", Point2D<int>(b_topleftx, b_tly), b_width, b_height, hd_brownButtons, black, "Exporter la vue actuelle", 16, black);
	bm.getButton<Render*>("b_exportView").setAction(hm.exportImage, &r);
	TextBox sceneTitle(hm.getSceneName(), "fonts/calibri.ttf", 24, black, Point2D<int>(30, 5), 930, 30, window.getRenderer());

	// Liaison caméras / boutons de changement de vues
	camPack p0 = { &topCam, "Vue du haut", &current_cam, &window, &manager, "top" };
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
	r.updateTriangles(manager);
	while (!keyboard.escape.down) {
		inputEvent.update();
		r.updateTriangles(manager);

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

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
		else if (keyboard.l.down) {
			if (!Camera::getCurrent().locked)	Camera::getCurrent().lock();
			// if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			// else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		else if (keyboard.F11.down) {
			window.ToggleWindow(window.getWidth(), window.getHeight());
		}

		// Fond de pipeline et rectangle espace interactions
		// drawer.DrawFillRoundedRectContoured(Point2D<int>(30,30), 900, 506, 5, hd_blueSky, black, window.getRenderer());
		blueSky.render(window.getRenderer(), Point2D<int>(30,30), 900, 506);
		drawer.DrawFillRoundedRectContoured(Point2D<int>(970, 30), 270, 506, 3, white, black, window.getRenderer());

		// if (Camera::getCurrent().locked) r.renderStatic(Point2D<int>(30,30),900,506, window);
		// else r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);
		r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);
		// Print camera position and angle
		// std::cout<<Camera::getCurrent().getCameraPosition()<< " "<<Camera::getCurrent().angleX<<" "<<Camera::getCurrent().angleY<<std::endl;

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		// Render des boutons de vues
		hm.renderViewsButtons(window.getRenderer());
		r.renderOrientation(Point2D<int>(50, 470), 50, window);

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

				// Mouvement avec flèches
					// à passer sur la fonction checkFurnitureClick
					// Maintenant il faut check la collision avec le mur (maxx et maxy de la hitbox ou alors hit2D)
				
				if (keyboard.left.pressed) 
					hm.moveFurniture(manager, 0);
				if (keyboard.right.pressed) 
					hm.moveFurniture(manager, 1);
				if (keyboard.down.pressed) 
					hm.moveFurniture(manager, 2);
				if (keyboard.up.pressed) 
					hm.moveFurniture(manager, 3);

				break;
			default:
				// Render infos sur la scène actuelle
				hm.renderDefault(inputEvent, window);
				break;
		}

		current_cam.render(window.getRenderer(), 0, 0);

		window.RenderScreen();
		window.FillScreen(hd_beigeBackground);
	}

	return 0;
}


// TextInput ti("Hello world!", pth+std::string("fonts/calibri.ttf"), 30, black, Point2D<int>(450, 50), 200, 25, window.getRenderer());
// TextBox tb1("OK!", pth + std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 100), 100, 25, window.getRenderer());
// TextBox tb2("ajouter cube", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 150), 100, 25, window.getRenderer());

// bm.addRectButton<TextInput*>("buttonA", nullptr, dark_gray, black, &tb1, Point2D<int>(1300, 100), 100, 25);
// bm.getButton<TextInput*>("buttonA").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);



// struct Function {
// 	Bitmap* bmp;
// 	ShapeManager* sm;
// 	void f(Function* f) {
// 		f->sm->addCube("cubeX", { 6,6,6 }, 40, bmp);
// 	}
// };
// Function func;
// Bitmap::newBitmap("80s_1", pth + std::string("textures/80s_1.jpg"));//t1
// func.bmp = Bitmap::getBitmap("80s_1");
// func.sm = &manager;
// bm.addRectButton<Function*>("buttonC", nullptr, gray, dark_gray, &tb2, Point2D<int>(1300, 150), 100, 25);
// bm.getButton<Function*>("buttonC").setAction([](Function* f) { f->sm->addCube("cubeX", { 6,6,6 }, 40, f->bmp); }, &func);

// bm.addCheckBox<void*>("cb1", white, dark_gray, Point2D<int>(1300, 200), 20);