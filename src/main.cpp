#include "headers.hpp"


void startFunc(int *s) {
	*s = 0; 
}

void changeCam(Camera *cam) {
	cam->setCurrent();
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
	
	starSystem Ssys(bm, manager, window);

	// 1. Contour
	bm.addRectButton<TextInput*>("b_contour", nullptr, white, black, nullptr, Point2D<int>(10, 10), 1260, 690);

	// 2. Chat
	TextBox t_textinfo("> Currently no star system.", pth + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(40, 520), 650, 150, window.getRenderer());
	bm.addRectButton<TextInput*>("b_Info", nullptr, white, black, &t_textinfo, Point2D<int>(30, 500), 680, 170);

	// 3. Set un astre

	int i_TLx = 750, i_TLy = 20;

	TextBox t_input("Input", pth + std::string("fonts/calibri.ttf"), 28, black, Point2D<int>(i_TLx, i_TLy), 80, 50, window.getRenderer());

	i_TLx += 20 ; i_TLy += 40;

	TextBox t_mass("Mass", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mass(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy), 100, 20, window.getRenderer());

	TextBox t_radius("Radius", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy + 35), 60, 20, window.getRenderer());
	TextInput i_radius(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy + 35), 100, 20, window.getRenderer());

	TextBox t_name("Name", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy + 70), 60, 20, window.getRenderer());
	TextInput i_name(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy + 70), 100, 20, window.getRenderer());

	TextBox t_posx("Position X", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy), 100, 20, window.getRenderer());
	TextInput i_posx(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy), 100, 20, window.getRenderer());

	TextBox t_posy("Position Y", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy + 35), 100, 20, window.getRenderer());
	TextInput i_posy(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy + 35), 100, 20, window.getRenderer());

	TextBox t_speed("Speed", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy + 70), 60, 20, window.getRenderer());
	TextInput i_speed(" ", pth + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy + 70), 100, 20, window.getRenderer());

	i_TLx = 750;
	i_TLy = 190;
	TextBox t_planet("Planet", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	bm.addCheckBox<void*>("switchstar", black, gray, Point2D<int>(818, 190), 20);
	TextBox t_star("Star", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 110, i_TLy), 40, 20, window.getRenderer());

	int b_width = 60, b_height = 25;
	int b_topleftx = 1120, b_tly = 187;
	TextBox b_add("Add", pth + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_add", nullptr, green, black, &b_add, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	//TEST
	Star testStar(5000, 1000000, { 0,0 }, "testStar");
	//bm.getButton<void*>("b_add").setAction(Ssys.addStar, &testStar);



	i_TLy = 450;
	TextBox t_simuspeed("Simulation Speed", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 140, 25, window.getRenderer());
	TextInput i_simuspeed(" x1", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 130, i_TLy - 3), 195, 25, window.getRenderer());

	b_width = 80, b_height = 30;
	b_topleftx = 1100, b_tly = 445;
	TextBox b_change("Change", pth + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_change", nullptr, green, black, &b_change, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	// 4. Global Settings
	TextBox t_globset("Global Settings", pth + std::string("fonts/calibri.ttf"), 28, black, Point2D<int>(i_TLx, i_TLy + 50), 220, 50, window.getRenderer());

	b_width = 130, b_height = 30;
	b_topleftx = 750, b_tly = 550;
	TextBox b_startsimu("Start simulation", pth + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_start", nullptr, green, black, &b_startsimu, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	b_topleftx += 147;

	TextBox b_stopsimu("Stop simulation", pth + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_stop", nullptr, red, black, &b_stopsimu, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	b_topleftx += 147;

	TextBox b_reset("Reset", pth + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_reset", nullptr, red, black, &b_reset, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	//bm.getButton<void*>("b_reset").setAction(Ssys.reset);

	i_TLy = 620;
	TextBox t_loadsave("Load / Save", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 90, 25, window.getRenderer());
	TextInput i_loadsave(" Filename", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 105, i_TLy - 3), 130, 25, window.getRenderer());

	b_width = 80, b_height = 25;
	b_topleftx = 1003, b_tly = 617;
	TextBox b_load("Load", pth + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_load", nullptr, green, black, &b_load, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	b_topleftx = 1100;
	TextBox b_save("Save", pth + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_save", nullptr, green, black, &b_save, Point2D<int>(b_topleftx, b_tly), b_width, b_height);



	//5. Camera
	Camera faceCam({ manager.getShape("fond").center.x, manager.getShape("fond").center.y, manager.getShape("fond").center.z - 100 }, 60, 0, 3.14519);
	faceCam.lock();
	
	b_topleftx = 720;

	//6. ScrollZone
	ScrollZone zone(inputEvent, window, Point2D<int>(750, 230), 430, 200, 520, 410);
	zone.startDrawInside(window.getRenderer());
		Draw::DrawTable(Point2D<int>(5, 5), 450, 400, 17, 5, white, black, window.getRenderer());
	zone.stopDrawInside(window.getRenderer());

	//zone.buttonManager.addRectTextButton<Camera*>("b1", Point2D<int>(5, 105), 70, 25, "Camera 1");
	TextBox zone_name("NAME", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(30, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_name);

	TextBox zone_mass("MASS", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(120, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_mass);

	TextBox zone_radius("RADIUS", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(200, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_radius);

	TextBox zone_position("POSITION", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(285, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_position);

	TextBox zone_speed("SPEED", pth + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(390, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_speed);
	
	manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", "earth", 0.001);
	r.updateTriangles(manager);
	while (!keyboard.escape.down) {
		i_mass.checkForInput(inputEvent, window.getRenderer());
		i_radius.checkForInput(inputEvent, window.getRenderer());
		i_name.checkForInput(inputEvent, window.getRenderer());
		i_posx.checkForInput(inputEvent, window.getRenderer());
		i_posy.checkForInput(inputEvent, window.getRenderer());
		i_speed.checkForInput(inputEvent, window.getRenderer());
		i_simuspeed.checkForInput(inputEvent, window.getRenderer());
		i_loadsave.checkForInput(inputEvent, window.getRenderer());

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		inputEvent.update();

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		i_mass.render(window.getRenderer(), 0);
		i_radius.render(window.getRenderer(), 0);
		i_name.render(window.getRenderer(), 0);
		i_posx.render(window.getRenderer(), 0);
		i_posy.render(window.getRenderer(), 0);
		i_speed.render(window.getRenderer(), 0);
		i_simuspeed.render(window.getRenderer(), 0);
		i_loadsave.render(window.getRenderer(), 0);

		t_input.render(window.getRenderer(), 0, 0);
		t_mass.render(window.getRenderer(), 0, 0);
		t_radius.render(window.getRenderer(), 0, 0);
		t_name.render(window.getRenderer(), 0, 0);
		t_posx.render(window.getRenderer(), 0, 0);
		t_posy.render(window.getRenderer(), 0, 0);
		t_speed.render(window.getRenderer(), 0, 0);
		t_planet.render(window.getRenderer(), 0, 0);
		t_star.render(window.getRenderer(), 0, 0);
		t_simuspeed.render(window.getRenderer(), 0, 0);
		t_globset.render(window.getRenderer(), 0, 0);
		t_loadsave.render(window.getRenderer(), 0, 0);
		r.updateTriangles(manager);

		zone.update();
		zone.render(window);

		//r.render({ 30,30 }, 680, 430, inputEvent, window, manager);
		if (keyboard.l.down) {
			if (Camera::getCurrent().locked)Camera::getCurrent().unlock();
			else if (!Camera::getCurrent().locked)Camera::getCurrent().lock();
		}
		if (Camera::getCurrent().locked) r.renderStatic(Point2D<int>(50, 50), 600, 400, window);
		else r.render(Point2D<int>(50, 50), 600, 400, inputEvent, window, manager);

		window.RenderScreen();
		window.FillScreen(white);
	}

	return 0;
}
