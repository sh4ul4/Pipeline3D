#pragma once

/**
 * @class Cette classe représente une étoile autour de laquelle tournent les planètes du sytème stellaire
 * Cette étoile se trouve au centre du système stellaire.
 * La classe 'Star' hérite de la classe 'Luminary'
 */
class Star : public Luminary
{
public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    /**
     * @brief Construit une étoile
     *
     * @param radius Taille de l'étoile (en m)
     * @param mass Masse de l'étoile (en Kg)
     * @param centre Centre de l'étoile. La position est initialisée à (0,0) car nous considérons ce point comme étant le centre du système stellaire
     * @param name Nom de l'étoile
     */
    Star(float radius, double mass, Point2D<int> centre, std::string name) : Luminary(radius, mass, centre, name) {}

    Star() = delete;
};