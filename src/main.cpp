#include "headers.hpp"

template <typename T>
std::string stdstrprec(const T a_value, const int n = 2)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

int main(int argc, char* argv[]) {
	Window window(1100, 550);
	InputEvent inputEvent;
	ShapeManager manager;

	Camera cam1("cam1",Vertex(0, 0, -30), 60, 0, 3.15);
	cam1.lock();
	cam1.lightColor = Color(244, 128, 0, 255);
	cam1.lightIntensity = 0.6;
	cam1.setControlDragAndDrop();

	TextBox::initLibrary();
	Render r(window, 758, 464);
	ButtonManager bm(inputEvent, window);

	/**		Camera		**/
	TextBox aovtb("angle of view : 60", "../fonts/calibri.ttf", 12, black, Point2D<int>(950, 30), window.getRenderer());
	DragBar aov(Point2D<int>(788, 30), 160, 11, 20, false);
	aov.padPos = 81;

	TextBox camcontroltb("camera-control : drag&drop / continuous", "../fonts/calibri.ttf", 12, black, Point2D<int>(800, 50), window.getRenderer());
	bm.addCheckBox<Camera*>("dragndrop", dark_gray, gray, Point2D<int>(788, 50), 10);
	bm.getButton<Camera*>("dragndrop").setAction([](Camera* cam) {cam->swapControl(); }, &cam1);

	TextBox lightxtb("light-source x : 0", "../fonts/calibri.ttf", 12, black, Point2D<int>(950, 70), window.getRenderer());
	DragBar lightX(Point2D<int>(788, 70), 160, 11, 20, false);
	TextBox lightytb("light-source y : 0", "../fonts/calibri.ttf", 12, black, Point2D<int>(950, 85), window.getRenderer());
	DragBar lightY(Point2D<int>(788, 85), 160, 11, 20, false);
	TextBox lightztb("light-source z : 0", "../fonts/calibri.ttf", 12, black, Point2D<int>(950, 100), window.getRenderer());
	DragBar lightZ(Point2D<int>(788, 100), 160, 11, 20, false);

	/**		Filter		**/
	TextBox filtertb("black&white filter", "../fonts/calibri.ttf", 12, black, Point2D<int>(800, 150), window.getRenderer());
	int filter = 0;
	bm.addCheckBox<int*>("bnw", dark_gray, gray, Point2D<int>(788, 150), 10);
	bm.getButton<int*>("bnw").setAction([](int* filter) {if (*filter == BNWfilter)*filter = 0; else *filter = BNWfilter; }, &filter);

	TextBox depthtb("z-buffer rendering", "../fonts/calibri.ttf", 12, black, Point2D<int>(800, 170), window.getRenderer());
	bool renderDepth = false;
	bm.addCheckBox<bool*>("renderD", dark_gray, gray, Point2D<int>(788, 170), 10);
	bm.getButton<bool*>("renderD").setAction([](bool* renderD) {*renderD = !*renderD; }, &renderDepth);

	TextBox showMeshtb("show mesh", "../fonts/calibri.ttf", 12, black, Point2D<int>(800, 190), window.getRenderer());
	bool showMesh = false;
	bm.addCheckBox<bool*>("showmesh", dark_gray, gray, Point2D<int>(788, 190), 10);
	bm.getButton<bool*>("showmesh").setAction([](bool* showMesh) {*showMesh = !*showMesh; }, &showMesh);

	/**		Movement	**/
	bm.addRectTextButton<Camera*>("left", Point2D<int>(788, 265), 20, 20, "<");
	bm.getButton<Camera*>("left").setAction([](Camera* cam) {cam->moveLockedCam(Vector(-1, 0, 0)); }, &cam1);
	bm.addRectTextButton<Camera*>("right", Point2D<int>(838, 265), 20, 20, ">");
	bm.getButton<Camera*>("right").setAction([](Camera* cam) {cam->moveLockedCam(Vector(1, 0, 0)); }, &cam1);
	bm.addRectTextButton<Camera*>("front", Point2D<int>(813, 240), 20, 20, "^");
	bm.getButton<Camera*>("front").setAction([](Camera* cam) {cam->moveLockedCam(Vector(0, 0, 1)); }, &cam1);
	bm.addRectTextButton<Camera*>("back", Point2D<int>(813, 265), 20, 20, "v");
	bm.getButton<Camera*>("back").setAction([](Camera* cam) {cam->moveLockedCam(Vector(0, 0, -1)); }, &cam1);
	bm.addRectTextButton<Camera*>("up", Point2D<int>(873, 240), 20, 20, "^");
	bm.getButton<Camera*>("up").setAction([](Camera* cam) {cam->moveLockedCam(Vector(0, 1, 0)); }, &cam1);
	bm.addRectTextButton<Camera*>("down", Point2D<int>(873, 265), 20, 20, "v");
	bm.getButton<Camera*>("down").setAction([](Camera* cam) {cam->moveLockedCam(Vector(0, -1, 0)); }, &cam1);

	TextBox anglextb("x-angle : ", "../fonts/calibri.ttf", 12, black, Point2D<int>(903, 235), window.getRenderer());
	DragBar anglexbar(Point2D<int>(903, 245), 160, 11, 20, false);
	anglexbar.padPos = 105;
	TextBox angleytb("y-angle : ", "../fonts/calibri.ttf", 12, black, Point2D<int>(903, 265), window.getRenderer());
	DragBar angleybar(Point2D<int>(903, 275), 160, 11, 20, false);
	angleybar.padPos = 70;

	/**		Matrix		**/
	TextBox viewmatrixtb("view-matrix", "../fonts/calibri.ttf", 12, black, Point2D<int>(788, 324), window.getRenderer());
	TextBox viewmatrixtbr1("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(788, 339), window.getRenderer());
	TextBox viewmatrixtbr2("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(788, 349), window.getRenderer());
	TextBox viewmatrixtbr3("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(788, 359), window.getRenderer());
	TextBox viewmatrixtbr4("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(788, 369), window.getRenderer());

	TextBox projmatrixtb("projection-matrix", "../fonts/calibri.ttf", 12, black, Point2D<int>(938, 324), window.getRenderer());
	TextBox projmatrixtbr1("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(938, 339), window.getRenderer());
	TextBox projmatrixtbr2("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(938, 349), window.getRenderer());
	TextBox projmatrixtbr3("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(938, 359), window.getRenderer());
	TextBox projmatrixtbr4("test", "../fonts/calibri.ttf", 12, black, Point2D<int>(938, 369), window.getRenderer());

	/**		Shapes		**/
	manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", "earth1", 0.001);
	manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", "earth2", 0.001);
	manager.getShape("earth2").move(Vector(10, 0, 25));


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

		/*Camera*/
		Draw::DrawFillContouredRect(Point2D<int>(788, 30) - 4, 300, 90, 2, Color(70, 70, 70), gray, window.getRenderer());

		aov.check(inputEvent, Point2D<int>(0, 0));
		cam1.angleView = 10 + aov.normalizedPadPos * 120;
		aov.render(window.getRenderer());
		aovtb.update("angle of view : " + std::to_string((int)cam1.angleView), window.getRenderer());
		aovtb.render(window.getRenderer());

		camcontroltb.render(window.getRenderer());

		lightX.check(inputEvent, Point2D<int>(0, 0));
		cam1.lightSource.x = -1 + lightX.normalizedPadPos * 2;
		lightX.render(window.getRenderer());
		lightxtb.update("light-source x : " + stdstrprec(cam1.lightSource.x), window.getRenderer());
		lightxtb.render(window.getRenderer());
		lightY.check(inputEvent, Point2D<int>(0, 0));
		cam1.lightSource.y = -1 + lightY.normalizedPadPos * 2;
		lightY.render(window.getRenderer());
		lightytb.update("light-source y : " + stdstrprec(cam1.lightSource.y), window.getRenderer());
		lightytb.render(window.getRenderer());
		lightZ.check(inputEvent, Point2D<int>(0, 0));
		cam1.lightSource.z = -1 + lightZ.normalizedPadPos * 2;
		lightZ.render(window.getRenderer());
		lightztb.update("light-source z : " + stdstrprec(cam1.lightSource.z), window.getRenderer());
		lightztb.render(window.getRenderer());
		cam1.lightSource.normalize();

		/*Filter*/
		Draw::DrawFillContouredRect(Point2D<int>(788, 150) - 4, 300, 60, 2, Color(70, 70, 70), gray, window.getRenderer());

		filtertb.render(window.getRenderer());
		depthtb.render(window.getRenderer());
		showMeshtb.render(window.getRenderer());

		/*Movement*/
		Draw::DrawFillContouredRect(Point2D<int>(788, 237) - 4, 300, 60, 2, Color(70, 70, 70), gray, window.getRenderer());

		cam1.angleX = -M_PI + anglexbar.normalizedPadPos * 4*M_PI;
		anglextb.update("x-angle : " + std::to_string(cam1.angleX), window.getRenderer());
		anglextb.render(window.getRenderer());
		anglexbar.check(inputEvent, Point2D<int>(0, 0));
		anglexbar.render(window.getRenderer());

		cam1.angleY = -M_PI + angleybar.normalizedPadPos * 4*M_PI;
		angleytb.update("y-angle : " + std::to_string(cam1.angleY), window.getRenderer());
		angleytb.render(window.getRenderer());
		angleybar.check(inputEvent, Point2D<int>(0, 0));
		angleybar.render(window.getRenderer());

		/*Matrix*/
		Draw::DrawFillContouredRect(Point2D<int>(788, 324) - 4, 300, 65, 2, Color(70, 70, 70), gray, window.getRenderer());
		viewmatrixtb.render(window.getRenderer());
		viewmatrixtbr1.update(
			stdstrprec(cam1.viewMatrix.get(0, 0)) + "  " + stdstrprec(cam1.viewMatrix.get(0, 1)) + "  " + stdstrprec(cam1.viewMatrix.get(0, 2)) + "  " + stdstrprec(cam1.viewMatrix.get(0, 3))
			, window.getRenderer());
		viewmatrixtbr2.update(
			stdstrprec(cam1.viewMatrix.get(1, 0)) + "  " + stdstrprec(cam1.viewMatrix.get(1, 1)) + "  " + stdstrprec(cam1.viewMatrix.get(1, 2)) + "  " + stdstrprec(cam1.viewMatrix.get(1, 3))
			, window.getRenderer());
		viewmatrixtbr3.update(
			stdstrprec(cam1.viewMatrix.get(2, 0)) + "  " + stdstrprec(cam1.viewMatrix.get(2, 1)) + "  " + stdstrprec(cam1.viewMatrix.get(2, 2)) + "  " + stdstrprec(cam1.viewMatrix.get(2, 3))
			, window.getRenderer());
		viewmatrixtbr4.update(
			stdstrprec(cam1.viewMatrix.get(3, 0)) + "  " + stdstrprec(cam1.viewMatrix.get(3, 1)) + "  " + stdstrprec(cam1.viewMatrix.get(3, 2)) + "  " + stdstrprec(cam1.viewMatrix.get(3, 3))
			, window.getRenderer());
		viewmatrixtbr1.render(window.getRenderer());
		viewmatrixtbr2.render(window.getRenderer());
		viewmatrixtbr3.render(window.getRenderer());
		viewmatrixtbr4.render(window.getRenderer());
		projmatrixtb.render(window.getRenderer());
		projmatrixtbr1.update(
			stdstrprec(cam1.projectionMatrix.get(0, 0)) + "  " + stdstrprec(cam1.projectionMatrix.get(0, 1)) + "  " + stdstrprec(cam1.projectionMatrix.get(0, 2)) + "  " + stdstrprec(cam1.projectionMatrix.get(0, 3))
			, window.getRenderer());
		projmatrixtbr2.update(
			stdstrprec(cam1.projectionMatrix.get(1, 0)) + "  " + stdstrprec(cam1.projectionMatrix.get(1, 1)) + "  " + stdstrprec(cam1.projectionMatrix.get(1, 2)) + "  " + stdstrprec(cam1.projectionMatrix.get(1, 3))
			, window.getRenderer());
		projmatrixtbr3.update(
			stdstrprec(cam1.projectionMatrix.get(2, 0)) + "  " + stdstrprec(cam1.projectionMatrix.get(2, 1)) + "  " + stdstrprec(cam1.projectionMatrix.get(2, 2)) + "  " + stdstrprec(cam1.projectionMatrix.get(2, 3))
			, window.getRenderer());
		projmatrixtbr4.update(
			stdstrprec(cam1.projectionMatrix.get(3, 0)) + "  " + stdstrprec(cam1.projectionMatrix.get(3, 1)) + "  " + stdstrprec(cam1.projectionMatrix.get(3, 2)) + "  " + stdstrprec(cam1.projectionMatrix.get(3, 3))
			, window.getRenderer());
		projmatrixtbr1.render(window.getRenderer());
		projmatrixtbr2.render(window.getRenderer());
		projmatrixtbr3.render(window.getRenderer());
		projmatrixtbr4.render(window.getRenderer());


		/*final rendering steps*/
		Draw::DrawFillContouredRect(Point2D<int>(20, 20) - 2, 762, 468, 2, Color(70, 70, 70), gray, window.getRenderer());
		r.render(Point2D<int>(20, 20), 758, 464, inputEvent, window, manager, filter, renderDepth, showMesh);

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());

		r.renderOrientation(Point2D<int>(728, 436), 40, window);

		window.RenderScreen();
		window.FillScreen(dark_gray);
	}
	return 0;
}