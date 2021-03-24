//#include "library.h"
//#include "Matrix.h"
#include "Render.h"
//#include "Input.h"
//#include "Window.h"
//#include "Vertex.h"


int main(int argc, char* argv[]) {
	//Window window(1200,600, 400, 200);
	//Window window(1200,600, 800, 400);
	Window window(1200, 600);
	InputEvent inputEvent;
	ShapeManager manager;
	Camera cam1({ -100,250,150 }, 60, 0, 0);
	// Camera cam2({ 60,10,50 }, 60, 0, 0);
	Camera cam2({ 50,50,200 }, 60, 0, 0);
	//cam2.setSun(Vertex(-2, 10, 3));

	 cam2.addToPath({ -50,50,50 });
	 cam2.addToPath({ 50,50,-20 });
	 cam2.addToPath({ -50,-30,50 });
	 cam2.addToPath({ 50,50,10 });

	Render r(window);
	Bitmap t0("../textures/space.gif");
	Bitmap t2("../textures/rgba2.png");
	//Texture t0("../textures/space.jpg", window.getRenderer());
	Bitmap t5("../textures/space2.jpg");
	Bitmap t1("../textures/32x32.gif");
	Bitmap t4("../textures/rgba2.png");
	Bitmap t3("../textures/80s_1.jpg");
	//Texture t2("../textures/queque.png", window.getRenderer());
	manager.addCube({ 0,0,0 }, 5, &t1);
	manager.setSubject(0);

	// La chambre d'Elyn
	cam2.setCurrent();
	Vertex hg = { 0, 100, 0 };
	Vertex bg = { 0, 0, 0 };
	Vertex hd = { 100, 100, 0 };
	Vertex bd = { 100, 0, 0 };
	// Mur de face
	manager.addRectangle(hg, bg, hd, bd, &t1); // original

	// coté droit
	manager.addRectangle(hg, bg, { 0, 100, 100 }, { 0, 0, 100 });
	// tapis, shape 3
	manager.addRectangle({ 0, 80, 20 }, { 0, 20, 20 }, { 0, 80, 80 }, { 0, 20, 80 }, &t1);

	// parallèle (coté gauche)
	manager.addRectangle(hd, bd, { 100, 100, 100 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle({ 100, 80, 20 }, { 100, 20, 20 }, { 100, 80, 80 }, { 100, 20, 80 }, &t1);

	// sol
	manager.addRectangle({ 0, 0, 0 }, { 0, 0, 100 }, { 100, 0, 0 }, { 100, 0, 100 });
	// tapis
	manager.addRectangle({ 20, 0, 20 }, { 20, 0, 80 }, { 80, 0, 20 }, { 80, 0, 80 }, &t1);

	// Bureau noir
	manager.addCube({ 6,6,6 }, 12, &t1);
	manager.addCube({ 18,6,6 }, 12, &t1);
	manager.addCube({ 30,6,6 }, 12, &t1);
	manager.addCube({ 6,6,18 }, 12, &t1); // ANGLE

	// Frigo
	manager.addCube({ 54,6,6 }, 12, &t5);
	manager.addCube({ 54,18,6 }, 12, &t5);

	// petit meuble avec plante (lol)
	manager.addCube({ 74,6,6 }, 12, &t1);
	manager.addCube({ 74,12,3 }, 6, &t0);

	// Lit
	manager.addCube({ 94,6,6 }, 12, &t1);
	manager.addCube({ 94,9,6 }, 10, &t5); // COUSSIN
	manager.addCube({ 94,6,18 }, 12, &t1);
	manager.addCube({ 94,6,30 }, 12, &t1);

	// Plafond
	manager.addRectangle({ 0, 100, 0 }, { 0, 100, 100 }, { 100, 100, 0 }, { 100, 100, 100 });
	// tapis plafond
	manager.addRectangle({ 20, 100, 20 }, { 20, 100, 80 }, { 80, 100, 20 }, { 80, 100, 80 }, &t1);

	// porte & paillasson
	manager.addRectangle({ 100, 25, 80 }, { 100, 0, 80 }, { 100, 25, 90 }, { 100, 0, 90 }, &t1);
	manager.addRectangle({ 98, 0, 80 }, { 90, 0, 80 }, { 98, 0, 90 }, { 90, 0, 90 }, &t1);

	r.updateTriangles(manager);
	while (!inputEvent.keyboard.escape) {
		// manager.shapes[3]->rotateY({  100,50,50 }, (float)0.019); // Tapis droit
		// manager.shapes[5]->rotateX({  0, 50, 50 }, (float)0.019); // Tapis gauche
		// manager.shapes[7]->rotateY({  50, 0, 50 }, (float)0.019); // Tapis sol
		manager.shapes[15]->rotateY({ 74, 12, 3 }, (float)0.019); // PLANTE (lol)
		if (inputEvent.keyboard._1) {
			cam1.setCurrent();
			inputEvent.keyboard._1 = false;
		}
		if (inputEvent.keyboard._2) {
			cam2.setCurrent();
			inputEvent.keyboard._2 = false;
		}
		if (inputEvent.keyboard.l) {
			if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
			inputEvent.keyboard.l = false;
		}
		if (inputEvent.keyboard.F11) {
			window.ToggleWindow(window.getRenderWidth(), window.getRenderHeight());
			inputEvent.keyboard.F11 = false;
		}
		r.render(inputEvent, window, manager /*, &t0*/);
	}
	return 0;
}
