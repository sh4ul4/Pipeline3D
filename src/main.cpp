#include "headers.hpp"


const Color elynbeige(248, 237, 227);
const Color elynmarron(216, 172, 156);
const Color elynvfonce(121, 135, 119);

void changeCam(Camera *cam) {
	cam->setCurrent();
	// if (shp) shp.visible = false;
}

void goFreeView(Camera *cam) {
	cam->setCurrent();
	cam->unlock();
}

void furnitureInsertion(int *type) {
	std::cout << "Insertion de meuble de type " << *type << std::endl;
}

int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	
	
	TextBox::initLibrary();
	ButtonManager bm(inputEvent, window);
	Render r(window, 900, 506); // 16:9

	// hg 		     	  hd              
	// { 0, 0, 0 },	 { 0, 0, 100 }, 
	
	// bg  			 bd
	// { 100, 0, 0 },   { 100, 0, 100 }

	// sol
	Bitmap t0(std::string("defense"), std::string("../textures/img.png"));
	manager.addRectangle("floor", { 0, 0, 0 }, { 0, 0, 100 }, { 100, 0, 0 }, { 100, 0, 100 }, &t0);
	
	// Mur de face
	Bitmap t1(std::string("80s"), std::string("../textures/face.jpg"));
	Vertex hg = { 0, 100, 0 };
	Vertex bg = { 0, 0, 0 };
	Vertex hd = { 100, 100, 0 };
	Vertex bd = { 100, 0, 0 };
	manager.addRectangle("face", hg, bg, hd, bd, &t1); // original

	Bitmap t2(std::string("wall"), std::string("../textures/wall.jpg"));
	// coté droit
	manager.addRectangle("droit", hg, bg, { 0, 100, 100 }, { 0, 0, 100 }, &t2);
	// coté gauche
	manager.addRectangle("gauche", hd, bd, { 100, 100, 100 }, { 100, 0, 100 }, &t2);

	std::cout<<(manager.getShape("floor")).center<<std::endl;
	Camera topCam({manager.getShape("floor").center.x, 200, manager.getShape("floor").center.z}, 60, -1.5708, 4.71239);
	// {50,200,50}
	Camera freeCam({ 120,300,65 }, 60, 0, 4);
	std::cout<<"[42.6126|43.0887|-7.44944] 0.0389948, 3.41587"<<std::endl;
	std::cout<<manager.getShape("face").center<<std::endl;
	Camera faceCam({manager.getShape("face").center.x, manager.getShape("face").center.y, manager.getShape("face").center.z - 100}, 60, 0, 3.1416);
	topCam.lock();
	freeCam.lock();
	faceCam.lock();
	Camera droitCam({manager.getShape("droit").center.x, manager.getShape("droit").center.y, manager.getShape("droit").center.z - 100}, 60, 0, 3.1416);
	droitCam.lock();
	
	// manager.imprtShapeObj("Earth",0.01);
	

	// TextInput ti("Hello world!", pth+std::string("fonts/calibri.ttf"), 30, black, Point2D<int>(450, 50), 200, 25, window.getRenderer());
	// TextBox tb1("OK!", pth + std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 100), 100, 25, window.getRenderer());
	// TextBox tb2("ajouter cube", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(1300, 150), 100, 25, window.getRenderer());

	// bm.addRectButton<TextInput*>("buttonA", nullptr, dark_gray, black, &tb1, Point2D<int>(1300, 100), 100, 25);
	// bm.getButton<TextInput*>("buttonA").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);

	// 3. Boutons interaction globale
	int b_width = 270, b_height = 38;
	int b_topleftx = 970, b_tly = 546;
	TextBox tb3("Editer la surface de la scene", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_editSurface", nullptr, elynmarron, black, &tb3, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	// bm.getButton<TextInput*>("b_editSurface").setAction([](TextInput* t) { std::cout << t->getText() << std::endl; }, &ti);
	b_tly += 58;
	TextBox tb4("Enregistrer la scene", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_saveScene", nullptr, elynmarron, black, &tb4, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	b_tly += 58;
	TextBox tb5("Exporter la vue actuelle", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<TextInput*>("b_exportView", nullptr, elynmarron, black, &tb5, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	
	TextBox tb6("MaChambreCROUS.flanf", pth+std::string("fonts/calibri.ttf"), 24, black, Point2D<int>(30, 5), 930, 30, window.getRenderer());
	
	// PP    40 970-240 40
 	// 0-930|--|buttons|--

	// 536 
	// --
	// 546
	// buttons
	// 576
	// --
	// 596
	// 
	// 700
	// 720

	TextBox tb1(" ", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(970, 30), 270, 506, window.getRenderer());
	bm.addRectButton<TextInput*>("b_espInter", nullptr, white, black, &tb1, Point2D<int>(970, 30), 270, 506);
	

	
	// 2. Boutons interaction Frame : vues
	b_width = 120, b_height = 30;
	b_topleftx = 30, b_tly = 546;
	TextBox tb7("Vue Haut", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Camera*>("b_mainView", nullptr, dark_gray, black, &tb7, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Camera*>("b_mainView").setAction(changeCam, &topCam);
	b_topleftx += 140;

	TextBox tb8("Vue Face", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Camera*>("b_View1", nullptr, dark_gray, black, &tb8, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Camera*>("b_View1").setAction(changeCam, &faceCam);
	b_topleftx += 140;

	TextBox tb9("Vue 2", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Camera*>("b_View2", nullptr, dark_gray, black, &tb9, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Camera*>("b_View2").setAction(changeCam, &droitCam);
	b_topleftx += 140;

	TextBox tb10("Vue 3", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_View3", nullptr, dark_gray, black, &tb10, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	b_topleftx += 140;

	TextBox tb11("Vue 4", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_View4", nullptr, dark_gray, black, &tb11, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	b_topleftx += 140;

	TextBox tb12("Deplacement libre", pth+std::string("fonts/calibri.ttf"), 16, light_gray, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Camera*>("b_freeMotionView", nullptr, elynvfonce, black, &tb12, Point2D<int>(b_topleftx, b_tly), b_width+80, b_height);
	bm.getButton<Camera*>("b_freeMotionView").setAction(goFreeView, &freeCam);


	// 2. Boutons interaction Frame : insertion
	int un = 1, deux = 2, trois = 3;
	b_width = 286, b_height = 104;
	b_topleftx = 30, b_tly = 596;
	TextBox tb13("Inserer meuble type 1", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<int*>("b_insertDefault", nullptr, elynmarron, black, &tb13, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<int*>("b_insertDefault").setAction(furnitureInsertion, &un);
	b_topleftx += 306; 

	TextBox tb14("Inserer meuble type 2", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<int*>("b_insertDefault2", nullptr, elynmarron, black, &tb14, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<int*>("b_insertDefault2").setAction(furnitureInsertion, &deux);
	b_topleftx += 306; 

	TextBox tb15("Inserer meuble type 3", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<int*>("b_insertDefault3", nullptr, elynmarron, black, &tb15, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<int*>("b_insertDefault3").setAction(furnitureInsertion, &trois);
	

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
	Mouse mouse;
	Keyboard keyboard;
	// Camera::getCurrent().lock();
	while (!keyboard.escape.down) {
		inputEvent.update();
		// ti.checkForInput(inputEvent, window.getRenderer());
		r.updateTriangles(manager);
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		
		// if (keyboard.one.down) {
		// 	cam1.setCurrent();
		// }
		// if (keyboard.two.down) {
		// 	//cam2.setCurrent();
		// }
		if (keyboard.l.down) {
			if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
			// if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			// else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		if (keyboard.F11.down) {
			window.ToggleWindow(window.getWidth(), window.getHeight());
		}

		r.render({30,30},900,506, inputEvent, window, manager /*, &t0*/);
		// Print camera position and angle
		// std::cout<<Camera::getCurrent().getCameraPosition()<< " "<<Camera::getCurrent().angleX<<" "<<Camera::getCurrent().angleY<<std::endl;

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());

		tb6.render(window.getRenderer(), 0, 0);
		// ti.render(window.getRenderer(), 0, 0);
		window.RenderScreen();
		window.FillScreen(elynbeige);
	}

	return 0;
}
