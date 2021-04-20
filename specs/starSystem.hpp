#include <iostream>
#include "Planet.hpp"
#include "Star.hpp"
#include <vector>

#define PLANETMAX 15 // Nos systèmes stellaires comprendront au maximum 15 planètes
#define DISTANCEMAX 6 * pow(10,9)

/**
 * @class Cette classe représente un système stellaire, avec une étoile autour de laquelle tournent des planètes
 */
class starSystem
{
private:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Liste de planètes présentées dans le système stellaire
    std::vector<Planet*> planets;
    // Représente l'étoile du système
    Star *sun = nullptr;
    // Détermine la vitesse de simulation du système (c'est un facteur)
    float simulationSpeed = 1;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    // Constructeur par défaut
    starSystem();

    /**
     * @brief Constructeur avec liste de planètes
     * 
     * @param planets Liste des planètes qui seront présentes dans le système stellaire
     */
    starSystem(const std::vector<Planet*> &planets);

    /**
     * @brief Constructeur avec étoile
     *
     * @param star représente l'étoile autour de laquelle les planètes tournent
     */
    starSystem(Star *star);

    /**
     * @brief Constructeur avec planètes et étoile
     * 
     * @param planets Listes des planètes du système stellaire
     * @param star l'étoile autour de laquelle tournent les planètes
     */
    starSystem(const std::vector<Planet*> &planets, Star *star);

    // Destucteur de la classe
    ~starSystem();

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Ajoute une planète dans le système stellaire
     * 
     * @param planet Planète à ajouter
     */
    void addPlanet(Planet *planet);

    /**
     * @brief Ajoute une étoile seulement si le système n'en possède pas
     * 
     * @param star L'étoile que l'on souhaite ajouter au système 
     */
    void addStar(Star *star);

    /**
     * @brief Détermine la vitesse de simulation
     * 
     * @param speed Facteur de vitesse de la simulation
     */
    void setSimulationSpeed(float speed);

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la liste des planètes présentent dans le système stellaire courant
     * 
     * @return Liste des planètes du système stellaire 
     */
    std::vector<Planet*> getPlanets();

    /**
     * @brief Récupère l'étoile du système stellaire courant
     * 
     * @return L'étoile du système stellaire 
     */
    Star *getStar();

    /**
     * @brief Récupère la vitesse de simulation du système stellaire courant
     * 
     * @return Le facteur de vitesse
     */
    float getSimulationSpeed();

    /* ----- METHODES ----- */

    // Vérifie que les planètes sont dans les limites imposées (pas trop loin de l'étoile)
    void checkPlanets();

    /**
     * @brief Retire une planète du système stellaire
     * 
     * @param name Nom de la planète
     */
    void deletePlanet(const std::string &name);

    // Retire l'étoile du système stellaire
    void deleteStar();

    // Retire tous les astres du système stellaire (appelle en boucle 'deletePlanet' et appelle 'deleteStar')
    void reset();

    // Vérifie qu'aucune collision n'a lieu dans le système stellaire
    void checkCollision();

    // Appelle les fonctions dans cinématique sur les planètes du système stellaires. Calcule la nouvelle position de toutes les planètes du système stellaire.
    void simulation();

    /**
     * @brief Calcule une vitesse initiale qui favorisera la stabilité du système, c'est-à-dire qui favorise l'orbite d'une planète autour de l'étoile
     * 
     * @param initialPosition Point représentant la position initiale de la planète
     * @return Vecteur vitesse initiale calculée
     */
    Point2D<double> calculateInitialSpeed(const Point2D &initialPosition);
};