#pragma once
#include "Luminary.hpp"

/**
 * @class Cette classe représente les planètes qui serviront à simuler un système stellaire.
 * La classe 'Planet' hérite de la classe 'Luminary'
 */
class Planet : public Luminary
{
private:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Vitesse sur l'axe x et y
    Point2D<double> speed;
    // Vitesse initial de la planète
    Point2D<double> initialSpeed;
    // Position initial de la planète
    Point2D<double> initialPosition;
    // Acceleration de la planète
    Point2D<double> acceleration;
    // Détermine si la planète et visible ou non : 1 --> visible ; 0 --> pas visible
    bool visible;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    // Avec un constructeur par défaut, plusieurs planètes pourraient se trouver sur le même axe de rotation --> collision assuré
    Planet() = delete;

    /**
     * @brief Construit une planète
     * 
     * @param radius Rayon (en mètre) de la planète
     * @param mass Masse (en Kg) de la planète
     * @param initialPosition Position initiale de la planète
     * @param initialSpeed Vitesse initiale de la planète
     * @param name Nom de la planète
     * @param visible Détermine la visibilité de la planète
     */
    Planet(float radius, double mass, Point2D<double> &initialPosition, Point2D<double> &initialSpeed, std::string name, bool visible = true) : Luminary(radius, mass, initialPosition, name) {}

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Affecte une nouvelle vitesse sur les deux axes (x,y)
     * 
     * @param speed Nouvelle vitesse à affecter. 
     *      speed.first --> axe x, speed.second --> axe y
     */
    void setSpeed(const Point2D<double> &speed)
    {
        this->speed = speed;
    }

    /**
     * @brief Affecte une nouvelle vitesse sur les deux axes (x,y)
     * 
     * @param initialSpeed Nouvelle vitesse initiale à affecter.
     *      initialSpeed.first --> axe x, initialSpeed.second --> axe y
     */
    void setInitialSpeed(const Point2D<double> &initialSpeed)
    {
        this->initialSpeed = initialSpeed;
    }

    /**
     * @brief Affecte une nouvelle position initiale à la planète
     * 
     * @param initialPosition Nouvelle position initiale
     */
    void setInitialPosition(const Point2D<double> &initialPosition)
    {
        this->initialPosition = initialPosition;
    }

    /**
     * @brief Affecte une nouvelle acceleration à la planète
     * 
     * @param accelaration Nouvelle acceleration
     */
    void setAcceleration(const Point2D<double> &acceleration)
    {
        this->acceleration = acceleration;
    }

    // Change la visiblité de la planète
    void changeVisibility()
    {
        visible = ~visible;
    }

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la vitesse sur l'axe x et  l'axe y
     * 
     * @return Vecteur vitesse 
     */
    Point2D<double> getSpeed()
    {
        return speed;
    }

    /**
     * @brief Récupère la vitesse initiale sur l'axe x et  l'axe y
     * 
     * @return Vecteur vitesse initiale 
     */
    Point2D<double> getInitialSpeed()
    {
        return initialSpeed;
    }

    /**
     * @brief Récupère la position initiale
     * 
     * @return Point2D représentant la position initiale de la planète
     */
    Point2D<double> getInitialPosition()
    {
        return initialPosition;
    }

    /**
     * @brief Récupère l'acceleration de la planète
     * 
     * @return Vecteur acceleration
     */
    Point2D<double> getAcceleration()
    {
        return acceleration;
    }

    /**
     * @brief Récupère la visibilité de la planète
     * 
     * @return 'true' si la planète est visible et considéré, 'false' sinon 
     */
    bool getVisibility()
    {
        return visible;
    }
};
