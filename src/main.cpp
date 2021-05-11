#include "headers.hpp"


void startFunc(int *s) {
	*s = 0; 
}


struct camPack {
	Camera *cam;
	std::string viewName;
	TextBox *current_cam_t;
	Window *window;
};


// Faire un Struct avec la caméra, le mur à retirer et une textbox à modifier pour écrire le nom de la cam
void changeCamTest(camPack *p) {
	p->cam->setCurrent();
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	// if (shp) shp.visible = false;
}

void goFreeView(Camera *cam) {
	cam->setCurrent();
	cam->unlock();
}


int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	TextBox::initLibrary();
	ButtonManager bm(inputEvent, window);
	Render r(window, 900, 506); // 16:9
	Mouse mouse;
	Keyboard keyboard;

	TextBox current_cam("Vue du haut", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(35, 35), 400, 20, window.getRenderer());


	int start = 1;

	TextBox t_intro("CROUS Simulator", pth+std::string("fonts/calibri.ttf"), 40, black, Point2D<int>(500, 260), 400, 40, window.getRenderer());
	int i_TLx = 440, i_TLy = 310;
	TextBox t_mur1("Mur 1", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mur1("12  m", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx+60, i_TLy), 60, 25, window.getRenderer());
	TextBox t_mur2("Mur 2", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy+45), 60, 20, window.getRenderer());
	TextInput i_mur2("12  m", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx+60, i_TLy+45), 60, 25, window.getRenderer());

	i_TLx += 280;
	TextBox t_mur3("Mur 3", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mur3("12  m", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx+60, i_TLy), 60, 25, window.getRenderer());
	TextBox t_mur4("Mur 4", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy+45), 60, 20, window.getRenderer());
	TextInput i_mur4("12  m", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx+60, i_TLy+45), 60, 25, window.getRenderer());
	

	bm.addRectTextButton<int*>("b_initApp", Point2D<int>(440, 400), 400, 40, "Nouvelle scene a partir de la surface donnee");
	bm.getButton<int*>("b_initApp").setAction(startFunc, &start);
	bm.addRectTextButton<int*>("b_initImport", Point2D<int>(440, 460), 400, 40, "Importer une scene depuis un fichier");


	while (start && !keyboard.escape.down) {
		i_mur1.checkForInput(inputEvent, window.getRenderer(), i_mur2);
		i_mur3.checkForInput(inputEvent, window.getRenderer(), i_mur4);

		// Mur 2 et 4 liés aux 1 et 3
		i_mur2.checkForInput(i_mur1, window.getRenderer());
		i_mur4.checkForInput(i_mur3, window.getRenderer());
		inputEvent.update();

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		
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

		window.RenderScreen();
		window.FillScreen(hd_beigeBackground);	
	}
	bm.removeButton("b_initApp");
	bm.removeButton("b_initImport");

	float w1, w3;
	try  {
		w1 = stof(i_mur1.getText());
		w3 = stof(i_mur3.getText());
	}  catch (const std::invalid_argument& ia)  {
		std::cerr << "\033[31mERR - Merci d'entrer un nombre flottant valide pour vos dimensions: \033[0m" << ia.what() << '\n';
		exit(1);
  	}

	HomeDesign hm( bm, manager, window, inputEvent, w1, w3 );


	/** ==============================
	 *  Initialisation caméras
	 * ===============================
	 */
	// Top / Gauche - OK
	Camera topCam({manager.getShape("floor").center.x, 200, manager.getShape("floor").center.z}, 60, -1.5708, 4.71239);
	topCam.lock();

	Camera freeCam({ 120,300,65 }, 60, 0, 4);
	freeCam.lock();

	Camera faceCam({manager.getShape("frontWall").center.x, manager.getShape("frontWall").center.y, manager.getShape("frontWall").center.z - 100}, 60, 0, 3.1416);
	faceCam.lock();

	// Gauche OK
	Camera gaucheCam({manager.getShape("leftWall").center.x, manager.getShape("leftWall").center.y, manager.getShape("leftWall").center.z - 100}, 60, 0, 3.1416);
	gaucheCam.lock();
	
	Camera droitCam({manager.getShape("rightWall").center.x, manager.getShape("rightWall").center.y, manager.getShape("rightWall").center.z - 100}, 60, 0, 0.3183);
	droitCam.lock();

	Camera backCam({manager.getShape("backWall").center.x, manager.getShape("backWall").center.y, manager.getShape("backWall").center.z - 100}, 60, 0, 3.1416);
	backCam.lock();
	

	// TextInput ti("Hello world!", pth+std::string("fonts/calibri.ttf"), 30, black, Point2D<int>(450, 50), 200, 25, window.getRenderer());
	// TextBox tb1("OK!", pth + std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 100), 100, 25, window.getRenderer());
	// TextBox tb2("ajouter cube", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 150), 100, 25, window.getRenderer());

	// bm.addRectButton<TextInput*>("buttonA", nullptr, dark_gray, black, &tb1, Point2D<int>(1300, 100), 100, 25);
	// bm.getButton<TextInput*>("buttonA").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);

	// 3. Boutons interaction globale
	int b_width = 270, b_height = 38;
	int b_topleftx = 970, b_tly = 546;
	TextBox tb3("Editer la surface de la scene", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_editSurface", nullptr, hd_brownButtons, black, &tb3, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	// bm.getButton<TextInput*>("b_editSurface").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);
	b_tly += 58;
	TextBox tb4("Enregistrer la scene", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_saveScene", nullptr, hd_brownButtons, black, &tb4, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	b_tly += 58;
	TextBox tb5("Exporter la vue actuelle", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_exportView", nullptr, hd_brownButtons, black, &tb5, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	
	TextBox sceneTitle("MaChambreCROUS.flanf", pth+std::string("fonts/calibri.ttf"), 24, black, Point2D<int>(30, 5), 930, 30, window.getRenderer());


	TextBox tb1(" ", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(970, 30), 270, 506, window.getRenderer());
	bm.addRectButton<TextInput*>("b_espInter", nullptr, white, black, &tb1, Point2D<int>(970, 30), 270, 506);
	

	camPack p0 = { &topCam, "Vue du haut", &current_cam, &window };
	camPack p1 = { &faceCam, "Vue de face", &current_cam, &window };
	camPack p2 = { &gaucheCam, "Vue de gauche", &current_cam, &window };
	camPack p3 = { &droitCam, "Vue de droite", &current_cam, &window };
	camPack p4 = { &backCam, "Vue de l'arrière", &current_cam, &window };
	// 2. Boutons interaction Frame : vues
	b_width = 120, b_height = 30;
	b_topleftx = 30, b_tly = 546;
	TextBox tb7("Vue Haut", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<camPack*>("b_mainView", nullptr, dark_gray, black, &tb7, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<camPack*>("b_mainView").setAction(changeCamTest, &p0);
	b_topleftx += 140;

	TextBox tb8("Vue Face", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<camPack*>("b_View1", nullptr, dark_gray, black, &tb8, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<camPack*>("b_View1").setAction(changeCamTest, &p1);
	b_topleftx += 140;

	TextBox tb9("Vue 2", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<camPack*>("b_View2", nullptr, dark_gray, black, &tb9, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<camPack*>("b_View2").setAction(changeCamTest, &p2);
	b_topleftx += 140;

	TextBox tb10("Vue 3", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<camPack*>("b_View3", nullptr, dark_gray, black, &tb10, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<camPack*>("b_View3").setAction(changeCamTest, &p3);
	b_topleftx += 140;

	TextBox tb11("Vue 4", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<camPack*>("b_View4", nullptr, dark_gray, black, &tb11, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<camPack*>("b_View4").setAction(changeCamTest, &p4);
	b_topleftx += 140;

	TextBox tb12("Deplacement libre", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Camera*>("b_freeMotionView", nullptr, hd_greenButtons, black, &tb12, Point2D<int>(b_topleftx, b_tly), b_width+80, b_height);
	bm.getButton<Camera*>("b_freeMotionView").setAction(goFreeView, &freeCam);


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
	
	
	r.updateTriangles(manager);
	while (!keyboard.escape.down) {
		inputEvent.update();
		r.updateTriangles(manager);

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		if (keyboard.one.down) 
			changeCamTest(&p0);
		else if (keyboard.two.down)
			changeCamTest(&p1);
		else if (keyboard.three.down)
			changeCamTest(&p2);
		else if (keyboard.four.down)
			changeCamTest(&p3);
		else if (keyboard.five.down)
			changeCamTest(&p4);
		else if (keyboard.l.down) {
			if (!Camera::getCurrent().locked)	Camera::getCurrent().lock();
			// if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			// else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		else if (keyboard.F11.down) {
			window.ToggleWindow(window.getWidth(), window.getHeight());
		}

		// if (Camera::getCurrent().locked) r.renderStatic(Point2D<int>(30,30),900,506, window);
		// else r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);

		r.render(Point2D<int>(30,30),900,506, inputEvent, window, manager);
		// Print camera position and angle
		// std::cout<<Camera::getCurrent().getCameraPosition()<< " "<<Camera::getCurrent().angleX<<" "<<Camera::getCurrent().angleY<<std::endl;

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		r.renderOrientation(Point2D<int>(50, 470), 50, window);

		sceneTitle.render(window.getRenderer(), 0, 0);
		
		if (HomeDesign::interactSpace == 1)  {
			hm.renderInsertion1(inputEvent, window);
		}

		current_cam.render(window.getRenderer(), 0, 0);

		window.RenderScreen();
		window.FillScreen(hd_beigeBackground);
	}

	return 0;
}
