#pragma once

/**
 * @class Cette classe permet la factorisation de code. Les classes 'Planet' et 'Star' hériteront de 'Luminary'
 */
class Luminary
{
protected:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Rayon de la planète (Détermine sa taille)
    float radius;
    // Masse de la planète (en Kg --> important pour les formules physiques)
    double mass;
    // Position en x et y du corps celeste
    Point2D<double> position;
    // Donne un nom à l'astre
    std::string name;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    /**
     * @brief Construit un corps celeste
     *
     * @param radius Rayon (en mètre) du corps celeste
     * @param mass Masse (en Kg) du corps celeste
     * @param position Paire contenant la coordonnée x et y du centre du corps celeste
     * @param name Nom de l'astre
     */
    Luminary(float radius, double mass, Point2D<double>& position, std::string name)
    {
        this->radius = radius;
        this->mass = mass;
        this->position = position;
        this->name = name;
    }

    Luminary() = delete;

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Affecte un nouveau rayon à la planète courante
     *
     * @param rayon Nouveau rayon à affecter
     */
    void setRadius(float radius)
    {
        this->radius = radius;
    }

    /**
     * @brief Affecte une nouvelle masse à la planète courante
     *
     * @param masse Nouvelle masse à affectée
     */
    void setMass(double mass)
    {
        this->mass = mass;
    }

    /**
     * @brief Affecte une nouvelle position au corps celeste
     *
     * @param position Un point 2D qui sera les nouvelles coordonnées de l'astre courant
     */
    void setPosition(Point2D<double>& position)
    {
        this->position = position;
    }

    /**
     * @brief Change le nom de l'astre
     *
     * @param name Nouveau nom donné à l'astre
     */
    void setName(std::string name)
    {
        this->name = name;
    }

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère le rayon de la planète courante
     *
     * @return Retourne le rayon de la planète (float)
     */
    float getRadius()
    {
        return radius;
    }

    /**
     * @brief Récupère la masse de la planète courante
     *
     * @return Retourn la masse de la planète (double)
     */
    double getMass()
    {
        return mass;
    }

    /**
     * @brief Récupère les coordonnées du centre du corps celeste
     *
     * @return Objet du type Point2D
     */
    Point2D<double> getPosition()
    {
        return position;
    }

    /**
     * @brief Récupère lenom de l'astre
     *
     * @return Nom de l'astre
     */
    std::string getName()
    {
        return name;
    }
};