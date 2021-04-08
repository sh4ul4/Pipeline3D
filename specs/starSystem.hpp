#include <iostream>
#include "Planet.hpp"
#include "Star.hpp"
#include <vector>

/**
 * @class Cette classe représente un système stellaire, avec une étoile autour de laquelle tournent des planètes
 */
class starSystem
{
private:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Liste de planètes présentes dans le système stellaire
    std::vector<Planet> planets;
    // Représente l'étoile du système
    Star sun;
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
    starSystem(const std::vector<Planet> &planets);

    /**
     * @brief Constructeur avec étoile
     *
     * @param star représente l'étoile autour de laquelle les planètes tournent
     */
    starSystem(Star star);

    /**
     * @brief Constructeur avec planètes et étoile
     * 
     * @param planets Listes des planètes du système stellaire
     * @param star l'étoile autour de laquelle tournent les planètes
     */
    starSystem(std::vector<Planet> planets, Star star);

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Ajoute une planète dans le système stellaire
     * 
     * @param planet Planète à ajouter
     */
    void addPlanet(Planet planet);

    /**
     * @brief Ajoute une étoile seulement si le système n'en possède pas
     * 
     * @param star L'étoile que l'on souhaite ajouter au système 
     */
    void addStar(Planet star);

    /**
     * @brief Détermine la vitesse de simulation
     * 
     * @param speed Facteur de vitesse de la simulation
     */
    void setSimulationSpeed(float speed);
};
