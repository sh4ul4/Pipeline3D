#include "headers.hpp"
#include <regex>

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

struct Starstruct {
	starSystem* Ssys;
	std::string name;
	std::string mass;
	std::string radius;
	std::string x;
	std::string y;
	std::string speed_x;
	std::string speed_y;
	std::string simuspeed;
};

void main_addStar(Starstruct* S) {
	if((std::regex_match(S->mass, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->radius, std::regex("([0-9]*[.])[0-9]+")))
		&& (std::regex_match(S->x, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->y, std::regex("([0-9]*[.])[0-9]+")))){
		Star* star = new Star(std::stof(S->radius), std::stod(S->mass), Point2D<double>(std::stod(S->x), std::stod(S->y)), S->name);
		S->Ssys->addStar(star);
		std::cout << S->Ssys->getStar()->getName() << std::endl;
		return;
	}
	else {
		std::cout << "Valeurs non initialisees !" << std::endl;
		return;
	}
}

void main_addPlanet(Starstruct* S) {
	if ((std::regex_match(S->mass, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->radius, std::regex("([0-9]*[.])[0-9]+")))
		&& (std::regex_match(S->x, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->y, std::regex("([0-9]*[.])[0-9]+")))
		&& (std::regex_match(S->speed_x, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->speed_y, std::regex("([0-9]*[.])[0-9]+")))) {
		Point2D<double> initialPos(std::stod(S->x), std::stod(S->y));
		Point2D<double> initialSpeed(std::stod(S->speed_x), std::stod(S->speed_y));
		Planet* p = new Planet(std::stof(S->radius), std::stod(S->mass), initialPos, initialSpeed, S->name);
		S->Ssys->addPlanet(p);
		return;
	}
	else {
		std::cout << "Valeurs non initialisees !" << std::endl;
		return;
	}
}

void main_setSimulation(Starstruct* S) {
	if (std::regex_match(S->simuspeed, std::regex("([0-9]*[.])[0-9]+"))) {
		S->Ssys->setSimulationSpeed(std::stof(S->simuspeed));
		std::cout << "Vitesse de simulation correctement modifiee" << std::endl;
		return;
	}
	else {
		std::cout << "Entrer une valeur valide !" << std::endl;
		return;
	}
}

void main_startSimu(Starstruct* S) {
	S->Ssys->simulation();
}

void main_reset(Starstruct* S) {
	S->Ssys->reset();
}

void test_print(Starstruct* S) {
	if(S->Ssys->getStar())
		std::cout << S->Ssys->getStar()->getName() << std::endl;
	else
		std::cout << "Pas d'étoile" << std::endl;
	return;
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
	TextBox t_textinfo("> Currently no star system.", PATH + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(40, 520), 650, 150, window.getRenderer());
	bm.addRectButton<TextInput*>("b_Info", nullptr, white, black, &t_textinfo, Point2D<int>(30, 500), 680, 170);

	// 3. Set un astre

	int i_TLx = 750, i_TLy = 20;

	TextBox t_input("Input", PATH + std::string("fonts/calibri.ttf"), 28, black, Point2D<int>(i_TLx, i_TLy), 80, 50, window.getRenderer());

	i_TLx += 20 ; i_TLy += 40;

	TextBox t_mass("Mass", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	TextInput i_mass(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy), 100, 20, window.getRenderer());

	TextBox t_radius("Radius", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy + 35), 60, 20, window.getRenderer());
	TextInput i_radius(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy + 35), 100, 20, window.getRenderer());

	TextBox t_name("Name", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx, i_TLy + 70), 60, 20, window.getRenderer());
	TextInput i_name(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 60, i_TLy + 70), 100, 20, window.getRenderer());

	TextBox t_posx("Position X", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy), 100, 20, window.getRenderer());
	TextInput i_posx(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy), 100, 20, window.getRenderer());

	TextBox t_posy("Position Y", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy + 35), 100, 20, window.getRenderer());
	TextInput i_posy(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy + 35), 100, 20, window.getRenderer());

	TextBox t_speed_x("SpeedX", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy + 70), 80, 20, window.getRenderer());
	TextInput i_speed_x(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy + 70), 100, 20, window.getRenderer());

	TextBox t_speed_y("SpeedY", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 220, i_TLy + 105), 80, 20, window.getRenderer());
	TextInput i_speed_y(" ", PATH + std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(i_TLx + 310, i_TLy + 105), 100, 20, window.getRenderer());

	i_TLx = 750;
	i_TLy = 200;
	TextBox t_planet("Planet", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 60, 20, window.getRenderer());
	bm.addCheckBox<void*>("b_switch", black, gray, Point2D<int>(818, 200), 20);
	TextBox t_star("Star", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 110, i_TLy), 40, 20, window.getRenderer());

	int b_width = 60, b_height = 25;
	int b_topleftx = 1120, b_tly = 197;
	TextBox b_add("Add", PATH + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Starstruct*>("b_add", nullptr, green, black, &b_add, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	Starstruct S = { &Ssys, i_name.getText(), i_mass.getText(), i_radius.getText(), i_posx.getText(), i_posy.getText(), i_speed_x.getText(), i_speed_y.getText() };

	i_TLy = 450;
	TextBox t_simuspeed("Simulation Speed", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 140, 25, window.getRenderer());
	TextInput i_simuspeed(" x1", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 130, i_TLy - 3), 195, 25, window.getRenderer());

	b_width = 80, b_height = 30;
	b_topleftx = 1100, b_tly = 445;

	TextBox b_change("Change", PATH + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Starstruct*>("b_change", nullptr, green, black, &b_change, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Starstruct*>("b_change").setAction(main_setSimulation, &S);

	// 4. Global Settings
	TextBox t_globset("Global Settings", PATH + std::string("fonts/calibri.ttf"), 28, black, Point2D<int>(i_TLx, i_TLy + 50), 220, 50, window.getRenderer());

	b_width = 130, b_height = 30;
	b_topleftx = 750, b_tly = 550;
	TextBox b_startsimu("Start simulation", PATH + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Starstruct*>("b_start", nullptr, green, black, &b_startsimu, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Starstruct*>("b_start").setAction(main_startSimu, &S);
	b_topleftx += 147;

	TextBox b_stopsimu("Stop simulation", PATH + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Starstruct*>("b_stop", nullptr, red, black, &b_stopsimu, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Starstruct*>("b_stop").setAction(test_print, &S);

	b_topleftx += 147;

	TextBox b_reset("Reset", PATH + std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<Starstruct*>("b_reset", nullptr, red, black, &b_reset, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
	bm.getButton<Starstruct*>("b_reset").setAction(main_reset, &S);

	i_TLy = 620;
	TextBox t_loadsave("Load / Save", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 90, 25, window.getRenderer());
	TextInput i_loadsave(" Filename", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 105, i_TLy - 3), 130, 25, window.getRenderer());

	b_width = 80, b_height = 25;
	b_topleftx = 1003, b_tly = 617;
	TextBox b_load("Load", PATH + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_load", nullptr, green, black, &b_load, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	b_topleftx = 1100;
	TextBox b_save("Save", PATH + std::string("fonts/calibri.ttf"), 17, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
	bm.addRectButton<void*>("b_save", nullptr, green, black, &b_save, Point2D<int>(b_topleftx, b_tly), b_width, b_height);

	//5. Camera
	Camera faceCam({ manager.getShape("fond").center.x, manager.getShape("fond").center.y, manager.getShape("fond").center.z - 100 }, 60, 0, 3.14519);
	faceCam.lock();
	
	b_topleftx = 720;

	//6. ScrollZone
	ScrollZone zone(inputEvent, window, Point2D<int>(750, 230), 430, 200, 520, 410);
	zone.startDrawInside(window.getRenderer());
		Draw::DrawTable(Point2D<int>(5, 5), 450, 400, 17, 6, white, black, window.getRenderer());
	zone.stopDrawInside(window.getRenderer());

	TextBox zone_name("NAME", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(20, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_name);

	TextBox zone_mass("MASS", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(95, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_mass);

	TextBox zone_radius("RADIUS", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(165, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_radius);

	TextBox zone_position("POSITION", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(232, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_position);

	TextBox zone_speed_x("SPEEDX", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(315, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_speed_x);

	TextBox zone_speed_y("SPEEDY", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(390, 10), 80, 30, window.getRenderer());
	zone.linkTextBox(zone_speed_y);

	//********************************************************************************************************
	//TEST AFFICHE STAR/PLANETE LORSQUE CREATION
	/*Point2D<int>pos(10, 30);
	TextBox star_1_name(testStar.getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_1_mass(std::to_string(testStar.getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_1_radius(std::to_string(testStar.getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_1_pos_x(std::to_string((int)testStar.getPosition().x) + ", ", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 45;
	TextBox star_1_pos_y(std::to_string((int)testStar.getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 45;
	TextBox star_1_speed("1", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox b_delete_1("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos, 30, 30, window.getRenderer());
	zone.buttonManager.addRectButton<void*>("delete_1", nullptr, red, black, &b_delete_1, pos, 20, 20);
	zone.linkTextBox(star_1_name);
	zone.linkTextBox(star_1_mass);
	zone.linkTextBox(star_1_radius);
	zone.linkTextBox(star_1_pos_x);
	zone.linkTextBox(star_1_pos_y);
	zone.linkTextBox(star_1_speed);
	pos.x = 10;
	pos.y += 25;

	TextBox star_2_name(testStar.getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_2_mass(std::to_string(testStar.getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_2_radius(std::to_string(testStar.getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox star_2_pos_x(std::to_string((int)testStar.getPosition().x) + ", ", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 45;
	TextBox star_2_pos_y(std::to_string((int)testStar.getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 45; 
	TextBox star_2_speed("1", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 80, 30, window.getRenderer());
	pos.x += 90;
	TextBox b_delete_2("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos, 30, 30, window.getRenderer());
	zone.buttonManager.addRectButton<void*>("delete_2", nullptr, red, black, &b_delete_1, pos, 20, 20);
	zone.linkTextBox(star_2_name);
	zone.linkTextBox(star_2_mass);
	zone.linkTextBox(star_2_radius);
	zone.linkTextBox(star_2_pos_x);
	zone.linkTextBox(star_2_pos_y);
	zone.linkTextBox(star_2_speed);
	pos.x = 10;
	pos.y += 25;*/
	//********************************************************************************************************
	

	r.updateTriangles(manager);
	while (!keyboard.escape.down) {
		i_mass.checkForInput(inputEvent, window.getRenderer());
		i_radius.checkForInput(inputEvent, window.getRenderer());
		i_name.checkForInput(inputEvent, window.getRenderer());
		i_posx.checkForInput(inputEvent, window.getRenderer());
		i_posy.checkForInput(inputEvent, window.getRenderer());
		i_speed_x.checkForInput(inputEvent, window.getRenderer());
		i_speed_y.checkForInput(inputEvent, window.getRenderer());
		i_simuspeed.checkForInput(inputEvent, window.getRenderer());
		i_loadsave.checkForInput(inputEvent, window.getRenderer());

		inputEvent.updateMouse(mouse);
		inputEvent.updateKeyBoard(keyboard);

		inputEvent.update();

		if (! bm.getButton<void*>("b_switch").isClicked()) {
			bm.getButton<Starstruct*>("b_add").setAction(main_addStar, &S);
		}
		else{
			bm.getButton<Starstruct*>("b_add").setAction(main_addPlanet, &S);
		}

		bm.checkButtons();
		bm.renderButtons(window.getRenderer());
		i_mass.render(window.getRenderer(), 0);
		i_radius.render(window.getRenderer(), 0);
		i_name.render(window.getRenderer(), 0);
		i_posx.render(window.getRenderer(), 0);
		i_posy.render(window.getRenderer(), 0);
		i_speed_x.render(window.getRenderer(), 0);
		i_speed_y.render(window.getRenderer(), 0);
		i_simuspeed.render(window.getRenderer(), 0);
		i_loadsave.render(window.getRenderer(), 0);

		t_input.render(window.getRenderer(), 0, 0);
		t_mass.render(window.getRenderer(), 0, 0);
		t_radius.render(window.getRenderer(), 0, 0);
		t_name.render(window.getRenderer(), 0, 0);
		t_posx.render(window.getRenderer(), 0, 0);
		t_posy.render(window.getRenderer(), 0, 0);
		t_speed_x.render(window.getRenderer(), 0, 0);
		t_speed_y.render(window.getRenderer(), 0, 0);
		t_planet.render(window.getRenderer(), 0, 0);
		t_star.render(window.getRenderer(), 0, 0);
		t_simuspeed.render(window.getRenderer(), 0, 0);
		t_globset.render(window.getRenderer(), 0, 0);
		t_loadsave.render(window.getRenderer(), 0, 0);
		
		r.updateTriangles(manager);

		//Mise a jour des valeurs
		S.mass = i_mass.getText();
		S.radius = i_radius.getText();
		S.x = i_posx.getText();
		S.y = i_posy.getText();
		S.name = i_name.getText();
		S.speed_x = i_speed_x.getText();
		S.speed_y = i_speed_y.getText();
		S.simuspeed = i_simuspeed.getText();

		zone.update();
		zone.render(window);

		r.render({ 30,30 }, 680, 430, inputEvent, window, manager);

		window.RenderScreen();
		window.FillScreen(white);
	}

	return 0;
}
