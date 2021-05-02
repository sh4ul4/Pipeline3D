#include <iostream>
#include "Planet.hpp"
#include "Star.hpp"
#include <vector>

#define PLANETMAX 15               // Nos systèmes stellaires comprendront au maximum 15 planètes
#define DISTANCEMAX 6 * pow(10, 9) // En m

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
    std::vector<Planet *> planets;
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
    starSystem(const std::vector<Planet *> &planets)
    {
        this->planets = planets;
    }

    /**
     * @brief Constructeur avec étoile
     *
     * @param star représente l'étoile autour de laquelle les planètes tournent
     */
    starSystem(Star *star)
    {
        this->sun = star;
    }

    /**
     * @brief Constructeur avec planètes et étoile
     * 
     * @param planets Listes des planètes du système stellaire
     * @param star l'étoile autour de laquelle tournent les planètes
     */
    starSystem(const std::vector<Planet *> &planets, Star *star)
    {
        this->planets = planets;
        this->sun = star;
    }

    // Destucteur de la classe
    ~starSystem()
    {
        if (sun)
        {
            delete sun;
            sun = nullptr;
        }

        for (Planet *p : planets)
            delete p;
        planets.clear();
    }

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Ajoute une planète dans le système stellaire
     * 
     * @param planet Planète à ajouter
     */
    void addPlanet(Planet *planet)
    {
        if (planet)
            planets.push_back(planet);
    }

    /**
     * @brief Ajoute une étoile seulement si le système n'en possède pas
     * 
     * @param star L'étoile que l'on souhaite ajouter au système 
     */
    void addStar(Star *star)
    {
        if (this->sun == nullptr && star != nullptr)
            this->sun = star;
    }

    /**
     * @brief Détermine la vitesse de simulation
     * 
     * @param speed Facteur de vitesse de la simulation
     */
    void setSimulationSpeed(float speed)
    {
        this->simulationSpeed = speed;
    }

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la liste des planètes présentent dans le système stellaire courant
     * 
     * @return Liste des planètes du système stellaire 
     */
    std::vector<Planet *> getPlanets()
    {
        return planets;
    }

    /**
     * @brief Récupère l'étoile du système stellaire courant
     * 
     * @return L'étoile du système stellaire 
     */
    Star *getStar()
    {
        return sun;
    }

    /**
     * @brief Récupère la vitesse de simulation du système stellaire courant
     * 
     * @return Le facteur de vitesse
     */
    float getSimulationSpeed()
    {
        return simulationSpeed;
    }

    /* ----- METHODES ----- */

    // Vérifie que les planètes sont dans les limites imposées (pas trop loin de l'étoile)
    void checkPlanets()
    {
        Point2D positionS = sun->getPosition();
        int distance;
        Point2D positionP;
        for (std::vector<Planet *>::iterator it = planets.begin(); it != planets.end();)
        {
            positionP = (*it)->getPosition();
            distance = positionP.distance(positionS);
            if (distance > DISTANCEMAX)
            {
                delete (*it);
                it = planets.erase(it);
            }
            else
                it++;
        }
    }

    /**
     * @brief Retire une planète du système stellaire
     * 
     * @param name Nom de la planète
     */
    void deletePlanet(const std::string &name)
    {
        for (std::vector<Planet *>::iterator it = planets.begin(); it != planets.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                delete (*it);
                planets.erase(it);
                return;
            }
        }
    }

    // Retire l'étoile du système stellaire
    void deleteStar()
    {
        if (sun)
        {
            delete sun;
            sun = nullptr;
        }
    }

    // Retire tous les astres du système stellaire
    void reset()
    {
        deleteStar();
        for (std::vector<Planet *>::iterator it = planets.begin(); it != planets.end();)
        {
            delete (*it);
            it = planets.erase(it);
        }
    }

    // Vérifie qu'aucune collision n'a lieu dans le système stellaire
    void checkCollision()
    {
        int distance;
        Point2D posA, posB;
        for (std::vector<Planet *>::iterator i = planets.begin(); i != planets.end() - 1; i++)
        {
            posA = (*i)->getPosition();
            // Check si il y a une collision entre la planète et toutes les autres
            for (std::vector<Planet *>::iterator j = planets.begin() + 1; j != planets.end(); j++)
            {
                posB = (*j)->getPosition();
                distance = posA.distance(posB);
                if (distance <= (*i)->getRadius() + (*j)->getRadius())
                    std::cout << "STOP THE SIMULATION !" << std::endl;
            }

            // Check si il y a une collision entre la planète et l'étoile
            posB = sun->getPosition();
            distance = posA.distance(posB);
            if (distance <= (*i)->getRadius() + sun->getRadius())
                std::cout << "STOP THE SIMULATION !" << std::endl;
        }
    }

    // Appelle les fonctions dans cinématique sur les planètes du système stellaires. Calcule la nouvelle position de toutes les planètes du système stellaire.
    void simulation()
    {
    }

    /**
     * @brief Calcule une vitesse initiale qui favorisera la stabilité du système, c'est-à-dire qui favorise l'orbite d'une planète autour de l'étoile
     * 
     * @param p Planète dont on veut calculer le vecteur vitesse initiale. L'attribut 'initalSpeed' est modifié par cette méthode
     */
    void calculateInitialSpeed(Planet &p)
    {
    }
};
