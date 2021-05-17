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

	ScrollZone* zone;
	Window* w;
	TextBox** TN;
	TextBox** TM;
	TextBox** TR;
	TextBox** TPX;
	TextBox** TPY;
	TextBox** TSX;
	TextBox** TSY;
};

void main_addStar(Starstruct* S) {
	if((std::regex_match(S->mass, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->radius, std::regex("([0-9]*[.])[0-9]+")))
		&& (std::regex_match(S->x, std::regex("([0-9]*[.])[0-9]+"))) && (std::regex_match(S->y, std::regex("([0-9]*[.])[0-9]+")))){
		Star* star = new Star(std::stof(S->radius), std::stod(S->mass), Point2D<double>(std::stod(S->x), std::stod(S->y)), S->name);
		S->Ssys->addStar(star);
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

		S->TN[S->Ssys->getPlanets().size() - 1]->update(S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TM[S->Ssys->getPlanets().size() - 1]->update(std::to_string(S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TR[S->Ssys->getPlanets().size() - 1]->update(std::to_string(S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TPX[S->Ssys->getPlanets().size() - 1]->update(std::to_string((int)S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TPY[S->Ssys->getPlanets().size() - 1]->update(std::to_string((int)S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TSX[S->Ssys->getPlanets().size() - 1]->update(std::to_string((int)S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		S->TSY[S->Ssys->getPlanets().size() - 1]->update(std::to_string((int)S->Ssys->getPlanets().at(S->Ssys->getPlanets().size() - 1)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
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
	int i;
	for (i = 0; i < 15; i++) {
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->zone->render(*(S->w));
	}
}

void main_deleteStar(Starstruct* S) {
	S->Ssys->deleteStar();
	return;
}

void main_deletePlanet1(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 1) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(0)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet2(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 2) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(1)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());

	}
	return;
}

void main_deletePlanet3(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 3) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(2)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());

	}
	return;
}

void main_deletePlanet4(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 4) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(3)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet5(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 5) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(4)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet6(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 6) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(5)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet7(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 7) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(6)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet8(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 8) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(7)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet9(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 9) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(8)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet10(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 10) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(9)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet11(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 11) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(10)->getName()); int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet12(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 12) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(11)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet13(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 13) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(12)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet14(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 14) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(13)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));

			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}

void main_deletePlanet15(Starstruct* S) {
	if (S->Ssys->getPlanets().size() >= 15) {
		S->Ssys->deletePlanet(S->Ssys->getPlanets().at(14)->getName());
		int i;
		for (i = 0; i < S->Ssys->getPlanets().size(); i++) {
			S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
			S->zone->render(*(S->w));
			
			S->TN[i]->update(S->Ssys->getPlanets().at(i)->getName(), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TM[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getMass()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TR[i]->update(std::to_string(S->Ssys->getPlanets().at(i)->getRadius()), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TPY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getPosition().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSX[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().x), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());
			S->TSY[i]->update(std::to_string((int)S->Ssys->getPlanets().at(i)->getSpeed().y), PATH + std::string("fonts/calibri.ttf"), 18, black, S->w->getRenderer());

		}
		S->TN[i]->update(S->TN[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TM[i]->update(S->TM[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TR[i]->update(S->TR[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPX[i]->update(S->TPX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TPY[i]->update(S->TPY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSX[i]->update(S->TSX[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
		S->TSY[i]->update(S->TSY[i]->getText(), PATH + std::string("fonts/calibri.ttf"), 18, white, S->w->getRenderer());
	}
	return;
}


int main(int argc, char* argv[]) {
	Window window(1280, 720);
	InputEvent inputEvent;
	ShapeManager manager;
	TextBox::initLibrary();
	ButtonManager bm(inputEvent, window);
	Render r(window, 600, 400); // 16:9
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

	i_TLy = 450;
	TextBox t_simuspeed("Simulation Speed", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx, i_TLy), 140, 25, window.getRenderer());
	TextInput i_simuspeed(" x1", PATH + std::string("fonts/calibri.ttf"), 18, black, Point2D<int>(i_TLx + 130, i_TLy - 3), 195, 25, window.getRenderer());

	
	b_width = 80, b_height = 30;
	b_topleftx = 1100, b_tly = 445;

	//6. ScrollZone
	ScrollZone zone(inputEvent, window, Point2D<int>(750, 230), 430, 200, 520, 410);

	int i;
	//TEST AFFICHE STAR/PLANETE LORSQUE CREATION
	Point2D<int>pos(8, 30);

	std::string curName = "";
	std::string curMass = "";
	std::string curRadius = "";
	std::string curpos_x = "";
	std::string curpos_y = "";
	TextBox star_1_name(curName, PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 75, 30, window.getRenderer());
	pos.x += 74;
	TextBox star_1_mass(curMass, PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
	pos.x += 77;
	TextBox star_1_radius(curRadius, PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
	pos.x += 73;
	TextBox star_1_pos_x(curpos_x, PATH + std::string("fonts/calibri.ttf"), 17, black, pos, 40, 30, window.getRenderer());
	pos.x += 35;
	TextBox star_1_pos_y(curpos_y, PATH + std::string("fonts/calibri.ttf"), 17, black, pos, 40, 30, window.getRenderer());
	zone.linkTextBox(star_1_name);
	zone.linkTextBox(star_1_mass);
	zone.linkTextBox(star_1_radius);
	zone.linkTextBox(star_1_pos_x);
	zone.linkTextBox(star_1_pos_y);
	pos.x = 10;
	pos.y += 23;

	TextBox* planet_name[15];
	TextBox* planet_mass[15];
	TextBox* planet_radius[15];
	TextBox* planet_posx[15];
	TextBox* planet_posy[15];
	TextBox* planet_speedx[15];
	TextBox* planet_speedy[15];

	for (int i = 0; i < 15; i++) {
		planet_name[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 75, 30, window.getRenderer());
		pos.x += 74;
		planet_mass[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x += 76;
		planet_radius[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x += 75;
		planet_posx[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x += 35;
		planet_posy[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x += 38;
		planet_speedx[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x += 75;
		planet_speedy[i] = new TextBox("", PATH + std::string("fonts/calibri.ttf"), 18, black, pos, 70, 30, window.getRenderer());
		pos.x = 10;
		pos.y += 23;
		zone.linkTextBox(*planet_name[i]);
		zone.linkTextBox(*planet_mass[i]);
		zone.linkTextBox(*planet_radius[i]);
		zone.linkTextBox(*planet_posx[i]);
		zone.linkTextBox(*planet_posy[i]);
		zone.linkTextBox(*planet_speedx[i]);
		zone.linkTextBox(*planet_speedy[i]);
	}
	//INITIALISATION STRUCT
	Starstruct S = { &Ssys, i_name.getText(), i_mass.getText(), i_radius.getText(), i_posx.getText(), i_posy.getText(), i_speed_x.getText(), i_speed_y.getText(), i_simuspeed.getText(),  &zone, &window, planet_name, planet_mass, planet_radius, planet_posx, planet_posy, planet_speedx, planet_speedy };

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

	Point2D<int> pos_zoneb(465, 30);
	TextBox zone_b_delete("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del0", nullptr, red, black, &zone_b_delete, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del0").setAction(main_deleteStar, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_1("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del1", nullptr, red, black, &zone_b_delete_1, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del1").setAction(main_deletePlanet1, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_2("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del2", nullptr, red, black, &zone_b_delete_2, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del2").setAction(main_deletePlanet2, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_3("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del3", nullptr, red, black, &zone_b_delete_3, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del3").setAction(main_deletePlanet3, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_4("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del4", nullptr, red, black, &zone_b_delete_4, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del4").setAction(main_deletePlanet4, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_5("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del5", nullptr, red, black, &zone_b_delete_5, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del5").setAction(main_deletePlanet5, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_6("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del6", nullptr, red, black, &zone_b_delete_6, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del6").setAction(main_deletePlanet6, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_7("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del7", nullptr, red, black, &zone_b_delete_7, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del7").setAction(main_deletePlanet7, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_8("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del8", nullptr, red, black, &zone_b_delete_8, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del8").setAction(main_deletePlanet8, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_9("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del9", nullptr, red, black, &zone_b_delete_9, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del9").setAction(main_deletePlanet9, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_10("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del10", nullptr, red, black, &zone_b_delete_10, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del10").setAction(main_deletePlanet10, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_11("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del11", nullptr, red, black, &zone_b_delete_11, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del11").setAction(main_deletePlanet11, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_12("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del12", nullptr, red, black, &zone_b_delete_12, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del12").setAction(main_deletePlanet12, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_13("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del13", nullptr, red, black, &zone_b_delete_13, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del13").setAction(main_deletePlanet13, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_14("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del14", nullptr, red, black, &zone_b_delete_14, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del14").setAction(main_deletePlanet14, &S);
	pos_zoneb.y += 23;
	TextBox zone_b_delete_15("X", PATH + std::string("fonts/calibri.ttf"), 20, black, pos_zoneb, 20, 20, window.getRenderer());
	zone.buttonManager.addRectButton<Starstruct*>("b_del15", nullptr, red, black, &zone_b_delete_15, pos_zoneb, 20, 20);
	zone.buttonManager.getButton<Starstruct*>("b_del15").setAction(main_deletePlanet15, &S);

	//*********************************************************************************

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
	Camera faceCam(Vertex(0,0,0), 60, 0, 3.14519);
	faceCam.lock();
	
	b_topleftx = 720;
	
	//********************************************************************************************************
	
	Ssys.addStar(new Star(693340000,1.989*pow(10,30),Point2D<double>(0,0),"Soleil"));
	
	//afficher tous les objets du système stellaire
	manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", "sun", 0.001);
	Point2D<double>	p1(0,149600000000);
	Point2D<double> p2(800,0);
	Ssys.addPlanet(new Planet(6371000,5.972*pow(10,24),p1,p2,std::string("earth")));
	std::vector<Planet*> Planets = Ssys.getPlanets();

	Planet *test=Planets[0];
	Point2D<double> Position=test->getPosition();
	std::cout<<Position.x<<std::endl;
	std::cout<<Position.y<<std::endl;
	Position.x=Position.x/pow(10,10);
	Position.y=Position.y/pow(10,10);
	std::cout<<Position.x<<std::endl;
	std::cout<<Position.y<<std::endl;
	manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", test->getName(), 0.001);
	manager.getShape(test->getName()).setPos(Vertex(Position.x,Position.y,0));


	/*
	for(int i=0;Planets.size();i++){
		manager.imprtShapeObj(std::string("OBJ/Earth/"), "Earth.obj", "earth", 0.001);
		manager.getShape("earth").setPos(Vertex(100,100,0));
		}
	*/

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

		//GESTION DES ASTRES
		if (!bm.getButton<void*>("b_switch").isClicked()) {
			bm.getButton<Starstruct*>("b_add").setAction(main_addStar, &S);
		}
		else {
			bm.getButton<Starstruct*>("b_add").setAction(main_addPlanet, &S);
		}

		if (S.Ssys->getStar()) {
			curName = S.Ssys->getStar()->getName();
			curMass = std::to_string(S.Ssys->getStar()->getMass());
			curRadius = std::to_string(S.Ssys->getStar()->getRadius());
			curpos_x = std::to_string((int)S.Ssys->getStar()->getPosition().x);
			curpos_y = std::to_string((int)S.Ssys->getStar()->getPosition().y);
			star_1_name.update(curName, window.getRenderer());
			star_1_mass.update(curMass, window.getRenderer());
			star_1_radius.update(curRadius, window.getRenderer());
			star_1_pos_x.update(curpos_x, window.getRenderer());
			star_1_pos_y.update(curpos_y, window.getRenderer());
			
		}
		else {
			star_1_name.update(curName, PATH + std::string("fonts/calibri.ttf"), 18, white, window.getRenderer());
			star_1_mass.update(curMass, PATH + std::string("fonts/calibri.ttf"), 18, white, window.getRenderer());
			star_1_radius.update(curRadius, PATH + std::string("fonts/calibri.ttf"), 18, white, window.getRenderer());
			star_1_pos_x.update(curpos_x, PATH + std::string("fonts/calibri.ttf"), 18, white, window.getRenderer());
			star_1_pos_y.update(curpos_y, PATH + std::string("fonts/calibri.ttf"), 18, white, window.getRenderer());
		}

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
