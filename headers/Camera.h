#pragma once

//#include "library.h"
#include "Matrix.h"

#include "Input.h"
//#include "Window.h"
#include <array>
//#include "Physics.h"

class Camera
{
private:
	static Camera* current; // pointer to camera being used
	Vertex pos; // camera position
	Vertex subject; // if there is a subject, camera can rotate to focus on the subject
	float distanceToSubject = 50; // vector length from [pos] to [subject]
	float LastAngleX = 0; // looking-direction angle horizontal
	float LastAngleY = 0; // looking-direction angle vertical
	Vertex sun; // vector for sunlight direction
public:
	bool hasSubject = false;
	float angleView = 10; // angle of view
	const float minAngleView = 0;
	const float maxAngleView = 180;
	float sensitivity = 0.003; // defines how smooth the camera turns ( < : more smooth)
	float near = 1; // near-clipping limit
	float far = 5000;// 350; // far-clipping limit
	float angleX = 0; // looking-direction angle horizontal
	float angleY = 0; // looking-direction angle vertical
	bool locked = false; // if true, can only be altered internally
	std::vector<Vertex> path; // path that can be set and used to move camera
	int pathMoveSpeed = 1; // movement speed of camera while following path
public:
	Camera(const std::array<float, 3>& pos, const float& angleOfView, const float& nearClipping, const float& farClipping, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleOfView), near(nearClipping), far(farClipping), angleX(angleX), angleY(angleY) {
		if (current == nullptr)setCurrent();
	}
	Camera(const std::array<float, 3>& pos, const float& angleView, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleView), angleX(angleX), angleY(angleY) {
		if (current == nullptr)setCurrent();
	}
	Camera() = delete;

	void setSun(const Vertex& sun) {
		this->sun = sun;
	}

	inline Vertex getSun() const {
		return sun;
	}

	inline Vertex getSunLight() {
		//Vertex sun(-2, 10, 3);
		return sun;
	}

	void setSubject(const Vertex& s) {
		subject = s;
		hasSubject = true;
		//rotSmooth *= 2;
	}

	Vertex& getCameraPosition() {
		return pos;
	}

	Vertex& getSubjectPosition() {
		if (hasSubject)return subject;
		else return pos;
	}

	void addToPath(const std::array<float, 3>& p) {
		path.push_back(p);
	}

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
	void FPfixedCoordToWorldCoord(Matrix::vec3d& v) {
		Matrix::m4d tmp = { {-v[1],0,-v[0],0} };
		Matrix::mult(tmp, Matrix::V, tmp);
		v[0] = tmp[0][0] + pos[0];
		v[1] += pos[1];
		v[2] = tmp[0][2] + pos[2];
	}

	// v[0] : front  v[1] : side  v[2] : up
	// view-angle incorporated
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

	static Vertex getMovementVector(const float& front, const float& side, const float& up, const float& speed) {
		Vertex v(front, side, up);
		v.normalizeOnLength(speed);
		Matrix::m4d tmp = { {-v[1],-v[2],-v[0],0} };
		Matrix::mult(tmp, Matrix::Vi, tmp);
		return Vertex(tmp[0][0], tmp[0][1], tmp[0][2]);
	}

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

	void moveTo(const Vertex& vec) {
		pos = Matrix::toVec3d(vec);
	}

	float relationToClipPlane(const Vertex& vec) {
		const Vertex pos = this->pos + getMovementVector(1, 0, 0, near);
		const Vertex n = getMovementVector(1, 0, 0, 10); // should be negative if going backwards !
		return n.x * (vec.x - pos.x) + n.y * (vec.y - pos.y) + n.z * (vec.z - pos.z);
	}

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

	Point2 get2DwithoutPerspective(Matrix::m4d m, bool* clip, const Window& window) {
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

	Point2 get2DonlyPerspective(Matrix::m4d m, bool* clip, const Window& window) {
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

	inline void setCurrent(void) { current = this; }
	static Camera& getCurrent(void) { return *current; }
	static bool currentExists(void) { return current != nullptr; }
	inline void lock(void) { locked = true; }
	inline void unlock(void) { locked = false; }
private:
	float clampAngleY(const float& angle) {
		return angle > 4.53786f ? 4.53786f : (angle < 1.74533f ? 1.74533f : angle);
	}
	float clampAngleX(const float& angle) {
		float tmp = (angle + 180.0f) / 360.0f;
		return angle -((int)tmp - (tmp < 0.0f ? 1 : 0)) * 360.0f;
	}
public:
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

Camera* Camera::current = nullptr;