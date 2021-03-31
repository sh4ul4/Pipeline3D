#pragma once

//#include "library.h"
#include "Matrix.h"

#include "Input.h"
//#include "Window.h"
#include <array>
//#include "Physics.h"

/**
 * @class Camera
 * @brief Gestion et mise à jour de la caméra 
 */

class Camera
{
private:
	/**
	 * Une variable privée.
	 * Pointeur vers la camera utilisée.
	 */
	static Camera* current; 
	/**
	 * Une variable privée.
	 * Position de la camera.
	 */
	Vertex pos;
	/**
	 * Une variable privée.
	 * Si il y a un sujet, la camera tourne et ce concentre sur celui ci.
	 */	
	Vertex subject; 
	/**
	 * Une variable privée.
	 * Vecteur distance entre [pos] et [sujet].
	 */			
	float distanceToSubject = 50;
	/**
	 * Une variable privée.
	 * Angle d'observation horizontal.
	 */			
	float LastAngleX = 0; 
	/**
	 * Une variable privée.
	 * Angle d'observation vertical.
	 */		
	float LastAngleY = 0; // looking-direction angle vertical
	/**
	 * Une variable privée.
	 * Vecteur de la direction de la lumière du soleil.
	 */			
	Vertex sun; 
public:
	/**
	 * Une variable publique.
	 * Si la camera a un sujet ou non.
	 */	
	bool hasSubject = false;
	/**
	 * Une variable publique.
	 * L'angle de vue.
	 */		
	float angleView = 10;
	/**
	 * Une variable publique.
	 * L'angle de vue minimum.
	 */		
	const float minAngleView = 0;
	/**
	 * Une variable publique.
	 * L'angle de vue maximum.
	 */		
	const float maxAngleView = 180;
	/**
	 * Une variable publique.
	 * Défini la sensibilité de la camera,plus c'est haut, plus elle tourne vite.
	 */		
	float sensitivity = 0.003; 
	/**
	 * Une variable publique.
	 * Limite du clipping de proximité.
	 */			
	float near = 1;
	/**
	 * Une variable publique.
	 * Limite du clipping de distance.
	 */		
	float far = 5000;
	/**
	 * Une variable publique.
	 * Angle d'observation horizontal.
	 */			
	float angleX = 0; 
	/**
	 * Une variable publique.
	 * Angle d'observation vertical.
	 */			
	float angleY = 0; 	
	/**
	 * Une variable publique.
	 * Cette variable indique si il peut être altéré seulement intérieurement.
	 */		
	bool locked = false;
	/**
	 * Une variable publique.
	 * Vecteur du chemin qui peut être utilisé pour bouger la camera.
	 */		
	std::vector<Vertex> path; 
	/**
	 * Une variable publique.
	 * Vitesse de mouvement de la camera pendant qu'elle suit le chemin.
	 */			
	int pathMoveSpeed = 1; 
public:
	/**
	 * Un constructeur.
	 * @param pos Position actuelle de la camera, angleOfView Angle actuel de la camera, nearClipping Distance de clipping proche, farClipping Distance de clipping loin, angleX Angle d'observation horizontal, angleY Angle d'observation vertical
	 * Construit la camera et si aucune n'est utilisée, la fait devenir celle utilisée.
	 */		
	Camera(const std::array<float, 3>& pos, const float& angleOfView, const float& nearClipping, const float& farClipping, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleOfView), near(nearClipping), far(farClipping), angleX(angleX), angleY(angleY) {
		if (current == nullptr)setCurrent();
	}
	/**
	 * Un constructeur.
	 * @param pos Position actuelle de la camera, angleOfView Angle actuel de la camera, angleX Angle d'observation horizontal, angleY Angle d'observation vertical
	 * Construit la camera et si aucune n'est utilisée, la fait devenir celle utilisée.
	 */			
	Camera(const std::array<float, 3>& pos, const float& angleView, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleView), angleX(angleX), angleY(angleY) {
		if (current == nullptr)setCurrent();
	}
	/**
	 * Un constructeur.
	 * @param aucun
	 * Supprime la camera.
	 */		
	Camera() = delete;

	/**
	 * Un setter.
	 * @param Vecteur de la direction de la lumière du soleil
	 * @brief Configure la direction de la lumière du soleil.
	 */	
	void setSun(const Vertex& sun) {
		this->sun = sun;
	}

	/**
	 * Un getter.
	 * @return Retourne la direction de la lumière du soleil en constante.
	 */	
	inline Vertex getSun() const {
		return sun;
	}

	/**
	 * Un getter.
	 * @return Retourne la direction de la lumière du soleil.
	 */	
	inline Vertex getSunLight() {
		//Vertex sun(-2, 10, 3);
		return sun;
	}

	/**
	 * Un setter.
	 * @param s Sujet de la camera
	 * @brief Défini s comme sujet de la camera, et donc hassubject devient true.
	 */	
	void setSubject(const Vertex& s) {
		subject = s;
		hasSubject = true;
		//rotSmooth *= 2;
	}
	
	/**
	 * Un getter.
	 * @return Retourne la position actuelle de la camera
	 */	
	Vertex& getCameraPosition() {
		return pos;
	}

	/**
	 * Un getter.
	 * @return Retourne la position actuelle du sujet de la camera ( si la camera en a un), sinon il retourne la position de la camera
	 */	
	Vertex& getSubjectPosition() {
		if (hasSubject)return subject;
		else return pos;
	}

	/**
	 * @brief Ajout une position au chemin de la camera
	 * @param p Est une position
	 */	
	void addToPath(const std::array<float, 3>& p) {
		path.push_back(p);
	}

	/**
	 * @brief La camera tourne de tous les angles possibles et ça refresh la visualisation 2D
	 */	
	void refresh2D() {
		Matrix::V = Matrix::makeFPviewMatrix({ pos.x,pos.y,pos.z }, angleY, angleX); // rotation is done around camera
		Matrix::P = Matrix::makeProjectionMatrix(angleView, far, near, -0.003);
		Matrix::makeWorldToCameraMatrix();
	}

	/*// move camera depending on world axes
	void moveCameraWorldCoord(const float& front, const float& side, const float& up) {
		const Matrix::m4d tmp = { {-side,0,-front,0} };
		pos[0] += tmp[0][0];
		pos[1] += up;
		pos[2] += tmp[0][2];
	}*/

	// v[0] : front  v[1] : side  v[2] : up
	// view-angle not incorporated
	
	/**
	 * @brief Permet de fixé la camera aux coordonnées du monde
	 * @param v Vecteur 3D des coordonnées du monde
	 */		
	void FPfixedCoordToWorldCoord(Matrix::vec3d& v) {
		Matrix::m4d tmp = { {-v[1],0,-v[0],0} };
		Matrix::mult(tmp, Matrix::V, tmp);
		v[0] = tmp[0][0] + pos[0];
		v[1] += pos[1];
		v[2] = tmp[0][2] + pos[2];
	}

	// v[0] : front  v[1] : side  v[2] : up
	// view-angle incorporated
	
	/**
	 * @brief Permet de fixé la camera aux coordonnées du monde
	 * @param v Vecteur 3D des coordonnées du monde
	 */		
	void FPrelativeCoordToWorldCoord(Matrix::vec3d& v) {
		Matrix::m4d tmp = { -v[1],-v[2],-v[0], 0 };
		Matrix::mult(tmp, Matrix::Vi, tmp);
		v[0] = tmp[0][0] + pos[0];
		v[1] = tmp[0][1] + pos[1];
		v[2] = tmp[0][2] + pos[2];
	}

	/*// move camera depending on first-person looking direction
	// ignoring up-looking direction
	void moveCameraFPCoord(const float& front, const float& side, const float& up, const float& speed) {
		Matrix::vec3d v;
		v = { front,side,up };
		v = Matrix::normalizeOnLength(v, speed);
		Matrix::m4d tmp = { {-v[1],0,-v[0],0} };
		Matrix::mult(tmp, Matrix::V, tmp);
		if (hasSubject) {
			subject.x += tmp[0][0];
			subject.y += up;
			subject.z += tmp[0][2];
		}
		else {
			pos[0] += tmp[0][0];
			pos[1] += up;
			pos[2] += tmp[0][2];
		}
	}*/
	/**
	 * @brief 
	 * @param front , side, up, speed Vitesse de la camera
	 * @return
	 */	
	static Vertex getMovementVector(const float& front, const float& side, const float& up, const float& speed) {
		Vertex v(front, side, up);
		v.normalizeOnLength(speed);
		Matrix::m4d tmp = { {-v[1],-v[2],-v[0],0} };
		Matrix::mult(tmp, Matrix::Vi, tmp);
		return Vertex(tmp[0][0], tmp[0][1], tmp[0][2]);
	}

	/**
	 * @brief 
	 * @param v Vecteur de sujet de la camera
	 * @return
	 */	
	void moveCameraPreCalculated(const Vertex& v) {
		if (hasSubject) {
			subject += v;
		}
		else {
			pos[0] += v.x;
			pos[1] += v.y;
			pos[2] += v.z;
		}
	}

	/*// move camera depending on first-person looking direction
	void moveCameraFPCoordUpIncl(const float& front, const float& side, const float& up, const float& speed) {
		Vertex v(front, side, up);
		v.normalizeOnLength(speed);
		Matrix::m4d tmp = { {-v[1],-v[2],-v[0],0} };
		Matrix::mult(tmp, Matrix::Vi, tmp);
		if (hasSubject) {
			subject.x += tmp[0][0];
			subject.y += tmp[0][1];
			subject.z += tmp[0][2];
		}
		else {
			pos[0] += tmp[0][0];
			pos[1] += tmp[0][1];
			pos[2] += tmp[0][2];
		}
	}*/
	/**
	 * @brief 
	 * @param vec Vecteur où se deplace la camera
	 * @return
	 */
	void moveTo(const Vertex& vec) {
		pos = Matrix::toVec3d(vec);
	}

	/**
	 * @brief 
	 * @param vec
	 * @return
	 */
	float relationToClipPlane(const Vertex& vec) {
		const Vertex pos = this->pos + getMovementVector(1, 0, 0, near);
		const Vertex n = getMovementVector(1, 0, 0, 10); // should be negative if going backwards !
		return n.x * (vec.x - pos.x) + n.y * (vec.y - pos.y) + n.z * (vec.z - pos.z);
	}

	/**
	 * @brief 
	 * @param m , clip , windows
	 * @return 
	 */
	Vertex get2D(Matrix::m4d m, bool& clip, const Window& window) {
		//world space
		clip = false;
		if (m.size() == 0) {
			return {};
		}
		m[0][3] = 1;
		Matrix::optmult(m, Matrix::V, m);
		// camera space
		if (Matrix::lengthV(m) > far) {
			clip = true;
			return { (int)m[0][0],(int)m[0][1], 0 };
		}
		Matrix::optmult(m, Matrix::P, m);
		//homogeneous clip space
		int x = (int)(m[0][0] / m[0][3]);
		int y = (int)(m[0][1] / m[0][3]);
		//NDC space[-1,1]
		x += window.getWidthCenter();
		y += window.getHeightCenter();
		//raster space
		return { x, y, m[0][3] };
	}

	/**
	 * @brief 
	 * @param m, clip, window 
	 * @return
	 */
	Point2D get2DwithoutPerspective(Matrix::m4d m, bool* clip, const Window& window) {
		*clip = false;
		if (m.size() == 0) {
			return {};
		}
		m[0][3] = 1;
		Matrix::mult(m, Matrix::V, m);
		if (Matrix::lengthV(m) > far) {
			*clip = true;
			return { (int)m[0][0],(int)m[0][1] };
		}
		if (m[0][3] < near) {
			*clip = true;
			return { (int)m[0][0],(int)m[0][1] };
		}
		const int x = (int)(m[0][0] / m[0][3]) + window.getWidthCenter();
		const int y = (int)(m[0][1] / m[0][3]) + window.getHeightCenter();
		return { x,y };
	}

	/**
	 * @brief 
	 * @param m, clip^, window
	 * @return
	 */
	Point2D get2DonlyPerspective(Matrix::m4d m, bool* clip, const Window& window) {
		*clip = false;
		if (m.size() == 0) {
			return {};
		}
		m[0][3] = 1;
		//Matrix::mult(m, Matrix::V, &m);
		if (Matrix::lengthV(m) > far) {
			*clip = true;
			return { (int)m[0][0],(int)m[0][1] };
		}
		if (m[0][3] < near) {
			*clip = true;
			/*normalize(&m);
			m[0][0] *= -1;
			m[0][1] *= -1;
			m[0][0] += winCenterX;
			m[0][1] += winCenterY;*/
			return { (int)m[0][0],(int)m[0][1] };
		}
		const int x = (int)(m[0][0] / m[0][3]) + window.getWidthCenter();
		const int y = (int)(m[0][1] / m[0][3]) + window.getHeightCenter();
		return { x,y };
	}

	/**
	 * Setter
	 * @param void
	 * @brief Configure la camera actuelle en celle ci
	 */
	inline void setCurrent(void) { current = this; }
	
	/**
	 * Getter
	 * @param void
	 * @return Retourne la camera actuelle
	 */	 
	static Camera& getCurrent(void) { return *current; }
	
	/**
	 * @param void
	 * @return Retourne vrai si une camera actuelle existe
	 */	
	static bool currentExists(void) { return current != nullptr; }
	
	/**
	 * @param void
	 * @brief Bloque la camera comme ça elle ne peut être modifiée qu'intérieurement
	 */		
	inline void lock(void) { locked = true; }
	
	/**
	 * @param void
	 * @brief Débloque la camera comme ça elle ne peut être modifée de partout
	 */		
	inline void unlock(void) { locked = false; }
private:
	/**
	 * Fonction privée
	 * @param angle Angle de la camera
	 * @return
	 */		
	float clampAngleY(const float& angle) {
		return angle > 4.53786f ? 4.53786f : (angle < 1.74533f ? 1.74533f : angle);
	}
	
	/**
	 * Fonction privée
	 * @param angle Angle de la camera
	 * @return
	 */		
	float clampAngleX(const float& angle) {
		float tmp = (angle + 180.0f) / 360.0f;
		return angle -((int)tmp - (tmp < 0.0f ? 1 : 0)) * 360.0f;
	}
public:

	/**
	 * Fonction publique
	 * @param inputEvent, window
	 * @brief
	 */	
	void update(InputEvent& inputEvent, const Window& window) {
		if (inputEvent.keyboard.z && angleView > minAngleView) {
			inputEvent.keyboard.z = false;
			angleView -= 1;
		}
		if (inputEvent.keyboard.e && angleView < maxAngleView) {
			inputEvent.keyboard.e = false;
			angleView += 1;
		}
		if (!locked) {
			// set angles
			angleX += sensitivity * (float)(inputEvent.mouse.x - window.getWidthCenter());
			angleX = clampAngleX(angleX);
			angleY += sensitivity * (float)(inputEvent.mouse.y - window.getHeightCenter());
			angleY = clampAngleY(angleY);
		}
		if (path.size() > 0 && current == this) {
			Vertex direction = { path[0][0] - subject[0],path[0][1] - subject[1],path[0][2] - subject[2] }; // goal - start
			if (direction[0] + direction[1] + direction[2] < 5 && direction[0] + direction[1] + direction[2] > -5) { // hit goal
				for (size_t i = 0; i < path.size() - 1; i++) {
					path[i] = path[i + 1];
				}path.pop_back();
			}
			else {
				direction.normalizeOnLength(pathMoveSpeed);
				// move
				subject += direction;
			}
		}
		SDL_WarpMouseInWindow(window.getWindow(), window.getWidthCenter(), window.getHeightCenter());
		refresh2D();
		if (hasSubject) {
			const Vertex look = this->getMovementVector(1, 0, 0, distanceToSubject);
			pos = subject - look;
		}
		refresh2D();
	}
};

	/**
	 * Destructeur
	 */	
Camera* Camera::current = nullptr;