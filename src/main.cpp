#include "headers.hpp"

int main(int argc, char* argv[]) {
	Window window(800, 600);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1(Vertex(0, 0, 0), 60, 0, 0);
	Camera cam2(Vertex(0, 0, 0), 60, 0, 0);
	cam2.setSubject(Vertex(0, 0, 0));
	cam2.lock();
	TextBox::initLibrary();
	Render r(window, 600, 400);
	ButtonManager bm(inputEvent, window);

	bm.addRectTextButton<Camera*>("b1", Point2D<int>(120, 480), 70, 25, "Camera 1");
	bm.getButton<Camera*>("b1").setAction([](Camera* cam) {cam->setCurrent(); }, &cam1);

	bm.addRectTextButton<Camera*>("b2", Point2D<int>(200, 480), 70, 25, "Camera 2");
	bm.getButton<Camera*>("b2").setAction([](Camera* cam) {cam->setCurrent(); }, &cam2);

	manager.imprtShapeObj("Earth", 0.001);
	
	r.updateTriangles(manager);
	Mouse mouse;
	Keyboard keyboard;
	while (!keyboard.escape.down) {
		inputEvent.update();
		r.updateTriangles(manager);
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);
		if (keyboard.l.down) {
			if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		if (Camera::getCurrent().locked) r.renderStatic(Point2D<int>(50, 50), 600, 400, window);
		else r.render(Point2D<int>(50, 50), 600, 400, inputEvent, window, manager);

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		r.renderOrientation(Point2D<int>(50, 470), 50, window);
		window.RenderScreen();
		window.FillScreen(teal);
	}
	return 0;
}