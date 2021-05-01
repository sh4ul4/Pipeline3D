#pragma once
/**
 * @class Camera
 * @brief Gestion et mise à jour de la caméra
 * 
 * @private `current`			Pointeur vers la camera utilisée.
 * @private `pos`			Position de la camera.
 * @private `subject`			Si il y a un sujet, la camera tourne et ce concentre sur celui ci.
 * @private `distanceToSubject`		Vecteur distance entre [pos] et [sujet].
 *
 * @public `hasSubject`		Si la camera a un sujet ou non.
 * @public `angleView`		L'angle de vue, exprimé en degrées.
 * @public `minAngleView`	L'angle de vue minimum.
 * @public `maxAngleView`	L'angle de vue maximum.
 * @public `sensitivity`	Défini la sensibilité de rotation de la camera.
 * @public `near`		Limite du clipping de proximité.
 * @public `far`		Limite du clipping de distance.
 * @public `angleX`		Angle d'observation horizontal.
 * @public `angleY`		Angle d'observation vertical.
 * @public `locked`		Cette variable indique si il peut être altéré seulement intérieurement.
 * @public `path`		Vecteur du chemin qui peut être utilisé pour bouger la camera.
 * @public `pathMoveSpeed`	Vitesse de mouvement de la camera pendant qu'elle suit le chemin.
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

	Mouse mouse;

	Keyboard keyboard;
public:

	Matrix<4, 4> viewMatrix;
	Matrix<4, 4> inverseViewMatrix;
	Matrix<4, 4> projectionMatrix;

	/**
	 * Une variable publique.
	 * Si la camera a un sujet ou non.
	 */
	bool hasSubject = false;
	/**
	 * Une variable publique.
	 * L'angle de vue, exprimé en degrée.
	 */
	float angleView = 80;
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
	float far = 100000;
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

	Vector look{ 0,0,0 };
public:
	/**
	 * Un constructeur.
	 * @param pos Position actuelle de la camera, angleOfView Angle actuel de la camera, nearClipping Distance de clipping proche, farClipping Distance de clipping loin, angleX Angle d'observation horizontal, angleY Angle d'observation vertical
	 * Construit la camera et si aucune n'est utilisée, la fait devenir celle utilisée.
	 */
	Camera(const Vertex& pos, const float& angleOfView, const float& nearClipping, const float& farClipping, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleOfView), near(nearClipping), far(farClipping), angleX(angleX), angleY(angleY) {
		if (current == nullptr) setCurrent();
	}

	/**
	 * Un constructeur.
	 * @param pos Position actuelle de la camera, angleOfView Angle actuel de la camera, angleX Angle d'observation horizontal, angleY Angle d'observation vertical
	 * Construit la camera et si aucune n'est utilisée, la fait devenir celle utilisée.
	 */
	Camera(const Vertex& pos, const float& angleView, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleView), angleX(angleX), angleY(angleY) {
		if (current == nullptr) setCurrent();
	}

	/**
	 * Un destructeur.
	 * @param aucun
	 * Supprime la camera.
	 */
	Camera() = delete;

	/**
	 * Un setter.
	 * @param s Sujet de la camera
	 * @brief Défini s comme sujet de la camera, et donc hassubject devient true.
	 */
	void setSubject(const Vertex& s) {
		subject = s;
		hasSubject = true;
	}

	void setDistanceToSubject(const float& distance) {
		distanceToSubject = distance;
	}

	void removeSubject() { hasSubject = false; }

	/**
	 * Un getter.
	 * @return Retourne la position actuelle de la camera
	 */
	Vertex& getCameraPosition() { return pos; }

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
	void addToPath(const Vertex& v) {
		path.push_back(v);
	}

	static Matrix<4, 4> makeFPviewMatrix(const Vertex& eye, const float& pitch, const float& yaw) {
		const float cosPitch = cos(pitch);
		const float sinPitch = sin(pitch);
		const float cosYaw = cos(yaw);
		const float sinYaw = sin(yaw);

		const Vector xaxis = { cosYaw, 0, -sinYaw };
		const Vector yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
		const Vector zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
		const Vector eyeVec = eye.getOriginVector();

		Matrix<4, 4> viewMatrix;
		viewMatrix.m[0] = { xaxis.x, yaxis.x, zaxis.x, 0 };
		viewMatrix.m[1] = { xaxis.y, yaxis.y, zaxis.y, 0 };
		viewMatrix.m[2] = { xaxis.z, yaxis.z, zaxis.z, 0 };
		viewMatrix.m[3] = { -xaxis.dot(eyeVec), -yaxis.dot(eyeVec), -zaxis.dot(eyeVec), 1 };
		return viewMatrix;
	}


	static Matrix<4, 4> makeProjectionMatrix(const float& fieldOfView, const float& far, const float& near, const float& depthOfField) {
		const float scale = (float)1.0 / (float)tan(fieldOfView / 2.0 * M_PI / 180);
		const float f1 = -far / (far - near);
		const float f2 = f1 * near;
		Matrix<4, 4> projectionMatrix;
		projectionMatrix.m[0] = { scale,0,0,0 };
		projectionMatrix.m[1] = { 0,scale,0,0 };
		projectionMatrix.m[2] = { 0,0,f1,depthOfField };
		projectionMatrix.m[3] = { 0,0,f2,0 };
		return projectionMatrix;
	}

	void makeWorldToCameraMatrix(Matrix<4, 4>& m) {
		m = inverseMatrix(viewMatrix);
	}

	/**
	 * @brief Création des nouvelle matrices de vue et projection de la frame actuelle.
	 */
	void refresh2D() {
		viewMatrix = makeFPviewMatrix(pos, angleY, angleX); // rotation is done around camera
		projectionMatrix = makeProjectionMatrix(angleView, far, near, -0.003);
		makeWorldToCameraMatrix(inverseViewMatrix);
	}

	/**
	 * @brief Permet de fixer la camera aux coordonnées du monde.
	 * @param v Vecteur 3D des coordonnées du monde
	 */
	void FPfixedCoordToWorldCoord(Vertex& v) const {
		Matrix<4, 4> tmp;
		tmp.m[0] = { -v[1],0,-v[0],0 };
		tmp.m[1] = { 0, 0, 0, 0 };
		tmp.m[2] = { 0, 0, 0, 0 };
		tmp.m[3] = { 0, 0, 0, 0 };
		tmp = optimizedProduct(tmp, viewMatrix);
		v.x = tmp.m[0][0] + pos.x;
		v.y += pos.y;
		v.z = tmp.m[0][2] + pos.z;
	}

	/**
	 * @brief Permet de fixer la camera aux coordonnées du monde.
	 * @param v Vecteur 3D des coordonnées du monde
	 */
	void FPrelativeCoordToWorldCoord(Vertex& v) const {
		Matrix<4, 4> tmp;
		tmp.m[0] = { -v[1],-v[2],-v[0],0 };
		tmp.m[1] = { 0, 0, 0, 0 };
		tmp.m[2] = { 0, 0, 0, 0 };
		tmp.m[3] = { 0, 0, 0, 0 };
		tmp = optimizedProduct(tmp, inverseViewMatrix);
		v.x = tmp.m[0][0] + pos.x;
		v.y = tmp.m[0][1] + pos.y;
		v.z = tmp.m[0][2] + pos.z;
	}

	/**
	 * @brief Precalcule le point de la position de la caméra concidérant sa direction et sa vitesse.
	 * @param front coordonnée X de la trajectoire de la caméra, side coordonnée Z de la trajectoire de la caméra, up coordonnée Y de la trajectoire de la caméra, speed Vitesse de la camera
	 * @return
	 */
	Vector getMovementVector(const float& front, const float& side, const float& up, const float& speed) const {
		Vector v(front, side, up);
		v.normalizeOnLength(speed);
		const Vector res(
			-v.y * inverseViewMatrix.m[0][0] - v.z * inverseViewMatrix.m[1][0] - v.x * inverseViewMatrix.m[2][0] + inverseViewMatrix.m[3][0],
			-v.y * inverseViewMatrix.m[0][1] - v.z * inverseViewMatrix.m[1][1] - v.x * inverseViewMatrix.m[2][1] + inverseViewMatrix.m[3][1],
			-v.y * inverseViewMatrix.m[0][2] - v.z * inverseViewMatrix.m[1][2] - v.x * inverseViewMatrix.m[2][2] + inverseViewMatrix.m[3][2]
		);
		return res;
	}

	/**
	 * @brief Applique à la caméra une nouvelle position en fonction d'un vecteur de déplacement.
	 * @param v Vecteur de déplacement
	 * @return
	 */
	void moveCameraPreCalculated(const Vector& v) {
		if (locked)return;
		if (hasSubject) subject += v;
		else pos += v;
	}

	/**
	 * @brief Déplace la caméra directement à un point.
	 * @param point Point où se deplace la caméra
	 * @return
	 */
	void moveTo(const Vertex& point) { if (!locked) pos = point; }

	/**
	 * @brief Retourner la distance par rapport au point. Négatif si le point est derrière la caméra.
	 * @param point Point dont la distance est mesurée
	 * @return
	 */
	float relationToClipPlane(const Vertex& point) const {
		const Vertex position = pos + look * near;
		const Vector n = look * 10; // should be negative if going backwards !
		return n.x * (point.x - position.x) + n.y * (point.y - position.y) + n.z * (point.z - position.z);
	}

	/**
	 * @brief Renvois la position d'un point par rapport au plan de la caméra, avec application de la perspective
	 * @param m matrice 4x4 contenant les coordonnées du point, clip valeur de clipping du point, windows fenetre dans laquelle calculer la position.
	 * @return
	 */
	Vertex get2D(Matrix<4, 4> m, bool& clip, const Point2D<int>& center) {
		//world space
		clip = false;
		m.m[0][3] = 1;
		m = optimizedProduct(m, viewMatrix);
		// camera space
		if (optimizedLength(m) > far) {
			clip = true;
			return { (int)m.m[0][0],(int)m.m[0][1], 0 };
		}
		m = optimizedProduct(m, projectionMatrix);
		//homogeneous clip space
		int x = (int)(m.m[0][0] / m.m[0][3]);
		int y = (int)(m.m[0][1] / m.m[0][3]);
		//NDC space[-1,1]
		x += center.x;
		y += center.y;
		//raster space
		return { x, y, m.m[0][3] };
	}

	Vertex get2DWithoutPerspective(Matrix<4, 4> m, bool& clip, const Point2D<int>& center, Matrix<4, 4> viewMatrix) {
		//world space
		clip = false;
		m.m[0][3] = 1;
		m = optimizedProduct(m, viewMatrix);
		// camera space
		if (optimizedLength(m) > far) {
			clip = true;
			return { (int)m.m[0][0],(int)m.m[0][1], 0 };
		}
		//homogeneous clip space
		int x = (int)(m.m[0][0] / m.m[0][3]);
		int y = (int)(m.m[0][1] / m.m[0][3]);
		//NDC space[-1,1]
		x += center.x;
		y += center.y;
		//raster space
		return { x, y, m.m[0][3] };
	}

	/**
	 * Setter
	 * @param void
	 * @brief Configure la camera actuelle en celle ci
	 */
	void setCurrent() { current = this; }

	/**
	 * Getter
	 * @param void
	 * @return Retourne la camera actuelle
	 */
	static Camera& getCurrent() { return *current; }

	/**
	 * @param void
	 * @return Retourne vrai si une camera actuelle existe
	 */
	static bool currentExists() { return current != nullptr; }

	/**
	 * @param void
	 * @brief Bloque la camera comme ça elle ne peut être modifiée qu'intérieurement
	 */
	void lock() { locked = true; }

	/**
	 * @param void
	 * @brief Débloque la camera comme ça elle ne peut être modifée de partout
	 */
	void unlock() { locked = false; }

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
		return angle - ((int)tmp - (tmp < 0.0f ? 1 : 0)) * 360.0f;
	}
public:

	/**
		 * Fonction publique
		 * @param inputEvent, window
		 * @brief Mettre à jour tous les paramètre changeants de la caméra à chaques frame.
		 * - angle de vue
		 * - direction du regard (1ère personne ou 3ème personne)
		 * - mouvements 3D (caméra et sujet de la caméra)
		 * - matrice de vue et matrice de perspective
		 */
	void update(InputEvent& inputEvent, const Window& window) {
		if (!locked) {
			SDL_ShowCursor(false);
			inputEvent.updateMouse(mouse);
			inputEvent.updateKeyBoard(keyboard);
			SDL_WarpMouseInWindow(window.getWindow(), window.getWidthCenter(), window.getHeightCenter());
			if (keyboard.z.pressed && angleView > minAngleView) {
				angleView -= 1;
			}
			if (keyboard.e.pressed && angleView < maxAngleView) {
				angleView += 1;
			}

			// set angles
			angleX += sensitivity * (float)(mouse.x - window.getWidthCenter());
			angleX = clampAngleX(angleX);
			angleY += sensitivity * (float)(mouse.y - window.getHeightCenter());
			angleY = clampAngleY(angleY);

			if (path.size() > 0 && current == this) {
				Vector direction(path[0].x - subject.x, path[0].y - subject.y, path[0].z - subject.z); // goal - start
				if (direction.x + direction.y + direction.z < 5 && direction.x + direction.y + direction.z > -5) { // hit goal
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

			refresh2D();
			if (hasSubject) {
				const Vector look = getMovementVector(1, 0, 0, distanceToSubject);
				pos = subject - look;
			}
			look = getMovementVector(1, 0, 0, 1);
		}
		else SDL_ShowCursor(true);
		refresh2D();
	}
};

Camera* Camera::current = nullptr;