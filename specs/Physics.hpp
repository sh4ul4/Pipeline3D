
namespace Physics
{
	// Valeurs correspondant aux lois physiques utilisées dans l'application Systeme Stellaire
	bool applyPhysics = false;
	Vertex playerHB = { 0,0,0 };
	float interactionDistance = 0.001;// 2;
	float speed = 1;
	float slippiness = 0.1f;
	bool intentionalMoving = false;
	
	// Valeurs des différentes constantes gravitationnelles 
	namespace Gravity {
		float gravity = 9.8f;
		bool applyGravity = false;
		int timeFalling = 0;
		float getGravityForce() {
			if (applyGravity == false) return 0;
			//timeFalling++;
			//return gravity / 50;
			return timeFalling * gravity / 1000;
		}
	}
	
	// 
	void resolvePhysics(const ShapeManager& manager, const Vertex& pos, const Vertex& step, const float& speed, const bool& falling, const int& depth);
	// 
	void move(InputEvent& inputEvent, const ShapeManager& manager);
}
