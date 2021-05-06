#include "headers.hpp"

int main(int argc, char* argv[]) {
	Window window(800, 600);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1(Vertex(0, 0, 0), 60, 0, 0);
	Camera cam2(Vertex(0, 0, 0), 60, 0, 0);
	cam2.setSubject(Vertex(0, 0, 0));

	TextBox::initLibrary();
	Render r(window, 800, 600);

	manager.addCube(std::string("cube1"),{ 0,0,0 }, 5);
	Camera::getCurrent().setSubject(Vertex(0, 0, 0));

	// La chambre d'Elyn
	cam2.setCurrent();
	Vertex hg = { 0, 100, 0 };
	Vertex bg = { 0, 0, 0 };
	Vertex hd = { 100, 100, 0 };
	Vertex bd = { 100, 0, 0 };
	// Mur de face
	manager.addRectangle(std::string("rect1"), hg, bg, hd, bd); // original

	// sol
	manager.addRectangle(std::string("rect6"), { 0, 0, 0 }, { 0, 0, 100 }, { 100, 0, 0 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle(std::string("rect7"), { 20, 0, 20 }, { 20, 0, 80 }, { 80, 0, 20 }, { 80, 0, 80 });


	manager.addCube("main_cube", { 100,0,0 }, 100);
	
	manager.addSphere("boule_noire", { 0,0,0 }, 50, black);

	manager.addSphere("boule_noire2", { 5,6,6 }, 10, black);


	// Frigo
	manager.addCube(std::string("cube6"), { 54,6,6 }, 12);
	manager.addCube(std::string("cube7"), { 54,18,6 }, 12);

	// petit meuble avec plante (lol)
	manager.addCube(std::string("cube8"), { 74,6,6 }, 12);
	manager.addCube(std::string("cube9"), { 74,12,3 }, 6);

	// Lit
	manager.addCube(std::string("cube10"), { 94,6,6 }, 12);
	manager.addCube(std::string("cube11"), { 94,9,6 }, 10); // COUSSIN
	manager.addCube(std::string("cube12"), { 94,6,18 }, 12);
	manager.addCube(std::string("cube13"), { 94,6,30 }, 12);

	
	
	r.updateTriangles(manager);
	Mouse mouse;
	Keyboard keyboard;
	while (!keyboard.escape.down) {
		inputEvent.update();
		r.updateTriangles(manager);
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		if (keyboard.one.down) {
			cam1.setCurrent();
		}
		if (keyboard.two.down) {
			cam2.setCurrent();
		}

		r.render(Point2D<int>(0, 0), 800, 600, inputEvent, window, manager);
		window.RenderScreen();
		window.FillScreen(teal);
	}
	return 0;
}