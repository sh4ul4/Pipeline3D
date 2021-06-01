#include "../headers/headers.hpp"

int main(int argc, char* argv[]) {

	// initialisation des valeurs initiales
	Window window(1100, 550);
	ShapeManager manager;
	Render ren(window, 758, 464);

	// ajout d'au moins une caméra
	Camera cam1("cam1",Vertex(0, 0, -50), 60, 0, 3.15);

	// initialisation des entrées utilisateur
	InputEvent inputEvent;
	Mouse mouse;
	Keyboard keyboard;

	/* zone d'ajout des objets initiaux
	 * ...
	 */

	while (!keyboard.escape.down) {

		// mise à jour du vecteur de triangles
		ren.updateTriangles(manager);

		// mise à jour des entrées utilisateur
		inputEvent.update();
		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		/* affichage qui nécessite un rafraichissement
		 * ...
		 */


		// étapes finales d'affichage
		Draw::DrawFillContouredRect(Point2D<int>(20, 20) - 2, 762, 468, 2, Color(70, 70, 70), gray, window.getRenderer());
		ren.render(Point2D<int>(20, 20), 758, 464, inputEvent, window, manager, filter, renderDepth, showMesh);

		// afficher l'orientation de la caméra dans une zonne apart
		ren.renderOrientation(Point2D<int>(728, 436), 40, window);

		// affichage SDL
		window.RenderScreen();
		window.FillScreen(dark_gray);
	}
	return 0;
}