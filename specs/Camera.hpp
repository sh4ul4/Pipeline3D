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
 * 
 * @private `current`			Pointeur vers la camera utilisée.
 * @private `pos`			Position de la camera.
 * @private `subject`			Si il y a un sujet, la camera tourne et ce concentre sur celui ci.
 * @private `distanceToSubject`		Vecteur distance entre [pos] et [sujet].
 * @private `LastAngleX`		Angle d'observation horizontal.
 * @private `LastAngleY`		Angle d'observation vertical.
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
	/**
	 * Une variable privée.
	 * Angle d'observation horizontal.
	 */			
	float LastAngleX = 0; 
	/**
	 * Une variable privée.
	 * Angle d'observation vertical.
	 */		
	float LastAngleY = 0;

	Mouse mouse;

	Keyboard keyboard;
public:
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
		:pos(pos), angleView(angleOfView), near(nearClipping), far(farClipping), angleX(angleX), angleY(angleY), angleZ(angleZ) {}
	
	/**
	 * Un constructeur.
	 * @param pos Position actuelle de la camera, angleOfView Angle actuel de la camera, angleX Angle d'observation horizontal, angleY Angle d'observation vertical
	 * Construit la camera et si aucune n'est utilisée, la fait devenir celle utilisée.
	 */			
	Camera(const std::array<float, 3>& pos, const float& angleView, const float& angleX, const float& angleY)
		:pos(pos), angleView(angleView), angleX(angleX), angleY(angleY), angleZ(angleZ) {}
	
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
	void setSubject(const Vertex& s);
	
	/**
	 * Un getter.
	 * @return Retourne la position actuelle de la camera
	 */	
	Vertex& getCameraPosition();

	/**
	 * Un getter.
	 * @return Retourne la position actuelle du sujet de la camera ( si la camera en a un), sinon il retourne la position de la camera
	 */	
	Vertex& getSubjectPosition();

	/**
	 * @brief Ajout une position au chemin de la camera
	 * @param p Est une position
	 */	
	void addToPath(const std::array<float, 3>& p);

	/**
	 * @brief Création des nouvelle matrices de vue et projection de la frame actuelle.
	 */	
	void refresh2D();
	
	/**
	 * @brief Permet de fixé la camera aux coordonnées du monde.
	 * @param v Vecteur 3D des coordonnées du monde
	 */		
	void FPfixedCoordToWorldCoord(Matrix::vec3d& v);
	
	/**
	 * @brief Permet de fixé la camera aux coordonnées du monde.
	 * @param v Vecteur 3D des coordonnées du monde
	 */		
	void FPrelativeCoordToWorldCoord(Matrix::vec3d& v);

	/**
	 * @brief Precalcule le point de la position de la caméra concidérant sa direction et sa vitesse.
	 * @param front coordonnée X de la trajectoire de la caméra, side coordonnée Z de la trajectoire de la caméra, up coordonnée Y de la trajectoire de la caméra, speed Vitesse de la camera
	 * @return
	 */	
	static Vertex getMovementVector(const float& front, const float& side, const float& up, const float& speed);

	/**
	 * @brief Applique à la caméra une nouvelle position en fonction d'un vecteur de déplacement.
	 * @param v Vecteur de déplacement
	 * @return
	 */	
	void moveCameraPreCalculated(const Vertex& v);

	/**
	 * @brief Déplace la caméra directement à un point.
	 * @param point Point où se deplace la caméra
	 * @return
	 */
	void moveTo(const Vertex& point);

	/**
	 * @brief Retourner la distance par rapport au point. Négatif si le point est derrière la caméra.
	 * @param point Point dont la distance est mesurée
	 * @return
	 */
	float relationToClipPlane(const Vertex& point);

	/**
	 * @brief Renvois la position d'un point par rapport au plan de la caméra, avec application de la perspective
	 * @param m matrice 4x4 contenant les coordonnées du point, clip valeur de clipping du point, windows fenetre dans laquelle calculer la position.
	 * @return 
	 */
	Vertex get2D(Matrix::m4d m, bool& clip, const Window& window);

	/**
	 * Setter
	 * @param void
	 * @brief Configure la camera actuelle en celle ci
	 */
	inline void setCurrent(void);
	
	/**
	 * Getter
	 * @param void
	 * @return Retourne la camera actuelle
	 */	 
	static Camera& getCurrent(void);
	
	/**
	 * @param void
	 * @return Retourne vrai si une camera actuelle existe
	 */	
	static bool currentExists(void);
	
	/**
	 * @param void
	 * @brief Bloque la camera comme ça elle ne peut être modifiée qu'intérieurement
	 */		
	inline void lock(void);
	
	/**
	 * @param void
	 * @brief Débloque la camera comme ça elle ne peut être modifée de partout
	 */		
	inline void unlock(void);
	
private:
	/**
	 * Fonction privée
	 * @param angle Angle de la camera
	 * @return
	 */		
	float clampAngleY(const float& angle);
	
	/**
	 * Fonction privée
	 * @param angle Angle de la camera
	 * @return
	 */		
	float clampAngleX(const float& angle);
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
	void update(InputEvent& inputEvent, const Window& window);

	/**
	 * Destructeur
	 */	
Camera* Camera::current = nullptr;
