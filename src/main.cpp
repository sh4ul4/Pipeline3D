#include "headers.hpp"

// __unix __WIN32
#if defined(_WIN32)
	std::string pth = "../";
#else
	std::string pth = "./";
#endif

int main(int argc, char* argv[]) {
	Window window(1200, 600);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1({ -100,250,150 }, 60, 0, 0);
	cam1.setSubject({ 0,0,0 });
	Camera cam2({ 50,50,200 }, 60, 0, 0);
	TextBox::initLibrary();
	ButtonManager bm(inputEvent);
	Render r(window);

	//manager.imprt("file");

	TextInput ti("Hello world!", pth+std::string("fonts/calibri.ttf"), 30, black, Point2D(450, 50), 200, 50, window.getRenderer());
	ti.start(inputEvent);

	TextBox tb("Hello world!", pth+std::string("fonts/calibri.ttf"), 30, black, Point2D(50, 50), window.getRenderer());

	bm.addButton<void*>("buttonA", nullptr, dark_gray, black, &tb, Point2D(50, 200), 50, 18);
	bm.getButton<void*>("buttonA").setAction([](void* a = nullptr) { std::cout << "ok" << std::endl; });

	Bitmap::newBitmap("space", pth + std::string("textures/space.gif"));//t0
	Bitmap::newBitmap("32x32", pth + std::string("textures/32x32.gif"));//t5
	Bitmap::newBitmap("80s_1", pth + std::string("textures/80s_1.jpg"));//t1

	manager.addCube("cube1", { 0,0,0 }, 5, Bitmap::getBitmap("80s_1"));

	struct Function {
		Bitmap* bmp;
		ShapeManager* sm;
		void f(Function* f) {
			f->sm->addCube("cubeX", { 6,6,6 }, 40, bmp);
		}
	};
	Function func;
	func.bmp = Bitmap::getBitmap("80s_1");
	func.sm = &manager;

	bm.addButton<Function*>("buttonC", nullptr, dark_gray, black, &tb, Point2D(50, 230), 50);
	bm.getButton<Function*>("buttonC").setAction([](Function* f) { f->sm->addCube("cubeX", { 6,6,6 }, 40, f->bmp); }, &func);

	bm.addButton<ShapeManager*>("buttonD", nullptr, dark_gray, black, &tb, Point2D(50, 350), 20);
	bm.getButton<ShapeManager*>("buttonD").setAction([](ShapeManager* s) { s->removeShape("cube1"); }, &manager);

	// La chambre d'Elyn
	cam2.setCurrent();
	Vertex hg = { 0, 100, 0 };
	Vertex bg = { 0, 0, 0 };
	Vertex hd = { 100, 100, 0 };
	Vertex bd = { 100, 0, 0 };

	// Mur de face
	manager.addRectangle("rect1", hg, bg, hd, bd, Bitmap::getBitmap("80s_1")); // original

	// cot� droit
	manager.addRectangle("rect2", hg, bg, { 0, 100, 100 }, { 0, 0, 100 });
	// tapis, shape 3
	manager.addRectangle("rect3", { 0, 80, 20 }, { 0, 20, 20 }, { 0, 80, 80 }, { 0, 20, 80 }, Bitmap::getBitmap("80s_1"));

	// parall�le (cot� gauche)
	manager.addRectangle("rect4", hd, bd, { 100, 100, 100 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle("rect5", { 100, 80, 20 }, { 100, 20, 20 }, { 100, 80, 80 }, { 100, 20, 80 }, Bitmap::getBitmap("80s_1"));

	// sol
	manager.addRectangle("rect6", { 0, 0, 0 }, { 0, 0, 100 }, { 100, 0, 0 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle("rect7", { 20, 0, 20 }, { 20, 0, 80 }, { 80, 0, 20 }, { 80, 0, 80 }, Bitmap::getBitmap("80s_1"));

	// Bureau noir
	manager.addCube("cube2", { 6,6,6 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube3", { 18,6,6 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube4", { 30,6,6 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube5", { 6,6,18 }, 12, Bitmap::getBitmap("80s_1")); // ANGLE

	// Frigo
	manager.addCube("cube6", { 54,6,6 }, 12);
	manager.addCube("cube7", { 54,18,6 }, 12);

	// petit meuble avec plante (lol)
	manager.addCube("cube8", { 74,6,6 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube9", { 74,12,3 }, 6, Bitmap::getBitmap("space"));

	// Lit
	manager.addCube("cube10", { 94,6,6 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube11", { 94,9,6 }, 10, Bitmap::getBitmap("32x32")); // COUSSIN
	manager.addCube("cube12", { 94,6,18 }, 12, Bitmap::getBitmap("80s_1"));
	manager.addCube("cube13", { 94,6,30 }, 12, Bitmap::getBitmap("80s_1"));

	// Plafond
	manager.addRectangle("rect8", { 0, 100, 0 }, { 0, 100, 100 }, { 100, 100, 0 }, { 100, 100, 100 });
	// tapis plafond
	manager.addRectangle("rect9", { 20, 100, 20 }, { 20, 100, 80 }, { 80, 100, 20 }, { 80, 100, 80 }, Bitmap::getBitmap("80s_1"));

	// porte & paillasson
	manager.addRectangle("rect10", { 100, 25, 80 }, { 100, 0, 80 }, { 100, 25, 90 }, { 100, 0, 90 }, Bitmap::getBitmap("80s_1"));
	manager.addRectangle("rect11", { 98, 0, 80 }, { 90, 0, 80 }, { 98, 0, 90 }, { 90, 0, 90 }, Bitmap::getBitmap("80s_1"));
	
	r.updateTriangles(manager);
	Mouse mouse;
	Keyboard keyboard;
	while (!keyboard.escape.down) {
		inputEvent.update();
		ti.checkForInput(inputEvent, window.getRenderer());
		r.updateTriangles(manager);
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
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

		Draw::DrawFillRoundedRectContoured({ 200,200 }, 100, 50, 6, gray, dark_gray, window.getRenderer());
		ti.render(window.getRenderer(), 0, 0);
		window.RenderScreen();
		window.FillScreen(teal);
	}
	ti.stop(inputEvent);
	//manager.exprt("file");
	return 0;
}