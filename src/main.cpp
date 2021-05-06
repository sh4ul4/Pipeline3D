#include "headers.hpp"

int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1(Vertex(0, 0, 0), 60, 0, 0);
	Camera cam2(Vertex(0, 0, 0), 60, 0, 0);

	TextBox::initLibrary();
	Render r(window, 1280, 720);

	cam1.setSubject(Vertex(10, 0, 0));

	cam2.setCurrent();
	
	manager.addCube("main_cube", { 40,0,0 }, 15);

	manager.addSphere("boule_noire", { 10,0,0 }, 10, black);
	manager.addSphere("la_lune", {0,-10,-10}, 3, blue);
	
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

		r.render(Point2D<int>(0, 0), 1280, 720, inputEvent, window, manager);
		window.RenderScreen();
		window.FillScreen(teal);
	}
	return 0;
}
