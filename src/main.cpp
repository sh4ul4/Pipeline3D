#include "headers.hpp"

Bitmap* bmp;

void f(ShapeManager* manager) {
	manager->addCube("cubeX", { 6,6,6 }, 40, bmp);
	std::cout << "done" << std::endl;
}

int main(int argc, char* argv[]) {
	//Window window(1200,600, 400, 200);
	//Window window(1200,600, 800, 400);
	Window window(1200, 600);
	//Window window(1600, 1080);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1({ -100,250,150 }, 60, 0, 0);
	//cam1.setSun({ 10,10,0 });
	// Camera cam2({ 60,10,50 }, 60, 0, 0);
	Camera cam2({ 50,50,200 }, 60, 0, 0);
	//cam2.setSun(Vertex(-2, 10, 3));

	TextBox::initLibrary();

	TextBox tb("Hello world!", "../fonts/calibri.ttf", 30, black, Point2D(50, 50), window.getRenderer());

	ButtonManager bm(inputEvent);

	bm.addButton("buttonA", nullptr, dark_gray, black, &tb, Point2D(50, 200), 50, 18);
	bm.getButton<int>("buttonA").setAction([]() { std::cout << "ok" << std::endl; });

	Render r(window);
	Bitmap t0("../textures/space.gif");
	Bitmap t2("../textures/rgba2.png");
	//Texture t0("../textures/space.jpg", window.getRenderer());
	Bitmap t3("../textures/space2.jpg");
	Bitmap t5("../textures/32x32.gif");
	Bitmap t4("../textures/rgba2.png");
	Bitmap t1("../textures/80s_1.jpg");
	//Texture t2("../textures/queque.png", window.getRenderer());
	manager.addCube("cube1", { 0,0,0 }, 5, &t1);
	manager.setSubject("cube1");

	bmp = &t1;
	/*RectButton<ShapeManager*> rb("buttonB", nullptr, dark_gray, black, &tb, Point2D(50, 230), 50, 18);
	rb.setAction(f, &manager);
	rb.playAction();*/

	bm.addButton<ShapeManager*>("buttonC", nullptr, dark_gray, black, &tb, Point2D(50, 230), 50);
	bm.getButton<ShapeManager*>("buttonC").setAction(f, &manager);

	// La chambre d'Elyn
	cam2.setCurrent();
	Vertex hg = { 0, 100, 0 };
	Vertex bg = { 0, 0, 0 };
	Vertex hd = { 100, 100, 0 };
	Vertex bd = { 100, 0, 0 };

	// Mur de face
	manager.addRectangle("rect1", hg, bg, hd, bd, &t1); // original

	// cot� droit
	manager.addRectangle("rect2", hg, bg, { 0, 100, 100 }, { 0, 0, 100 });
	// tapis, shape 3
	manager.addRectangle("rect3", { 0, 80, 20 }, { 0, 20, 20 }, { 0, 80, 80 }, { 0, 20, 80 }, &t1);

	// parall�le (cot� gauche)
	manager.addRectangle("rect4", hd, bd, { 100, 100, 100 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle("rect5", { 100, 80, 20 }, { 100, 20, 20 }, { 100, 80, 80 }, { 100, 20, 80 }, &t1);

	// sol
	manager.addRectangle("rect6", { 0, 0, 0 }, { 0, 0, 100 }, { 100, 0, 0 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle("rect7", { 20, 0, 20 }, { 20, 0, 80 }, { 80, 0, 20 }, { 80, 0, 80 }, &t1);

	// Bureau noir
	manager.addCube("cube2", { 6,6,6 }, 12, &t1);
	manager.addCube("cube3", { 18,6,6 }, 12, &t1);
	manager.addCube("cube4", { 30,6,6 }, 12, &t1);
	manager.addCube("cube5", { 6,6,18 }, 12, &t1); // ANGLE

	// Frigo
	manager.addCube("cube6", { 54,6,6 }, 12);
	manager.addCube("cube7", { 54,18,6 }, 12);

	// petit meuble avec plante (lol)
	manager.addCube("cube8", { 74,6,6 }, 12, &t1);
	manager.addCube("cube9", { 74,12,3 }, 6, &t0);

	// Lit
	manager.addCube("cube10", { 94,6,6 }, 12, &t1);
	manager.addCube("cube11", { 94,9,6 }, 10, &t5); // COUSSIN
	manager.addCube("cube12", { 94,6,18 }, 12, &t1);
	manager.addCube("cube13", { 94,6,30 }, 12, &t1);

	// Plafond
	manager.addRectangle("rect8", { 0, 100, 0 }, { 0, 100, 100 }, { 100, 100, 0 }, { 100, 100, 100 });
	// tapis plafond
	manager.addRectangle("rect9", { 20, 100, 20 }, { 20, 100, 80 }, { 80, 100, 20 }, { 80, 100, 80 }, &t1);

	// porte & paillasson
	manager.addRectangle("rect10", { 100, 25, 80 }, { 100, 0, 80 }, { 100, 25, 90 }, { 100, 0, 90 }, &t1);
	manager.addRectangle("rect11", { 98, 0, 80 }, { 90, 0, 80 }, { 98, 0, 90 }, { 90, 0, 90 }, &t1);
	
	r.updateTriangles(manager);
	Mouse mouse;
	Keyboard keyboard;
	while (!keyboard.escape.down) {
		r.updateTriangles(manager);
		//inputEvent.update(mouse, keyboard);
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		// manager.shapes[3]->rotateY({  100,50,50 }, (float)0.019); // Tapis droit
		// manager.shapes[5]->rotateX({  0, 50, 50 }, (float)0.019); // Tapis gauche
		// manager.shapes[7]->rotateY({  50, 0, 50 }, (float)0.019); // Tapis sol
		manager.shapes[15]->rotateY({ 74, 12, 3 }, (float)0.019); // PLANTE (lol)
		if (keyboard.one.down) {
			cam1.setCurrent();
		}
		if (keyboard.two.down) {
			cam2.setCurrent();
		}
		if (keyboard.l.down) {
			if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		if (keyboard.F11.down) {
			window.ToggleWindow(window.getRenderWidth(), window.getRenderHeight());
		}
		r.render(inputEvent, window, manager /*, &t0*/);

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());

		window.RenderScreen();
		window.FillScreen(teal);
	}
	return 0;
}
