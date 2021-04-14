#include "Luminary.hpp"

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
     * @param rayon Taille de l'étoile (en m)
     * @param mass Masse de l'étoile (en Kg)
     * La position est initialisé à (0,0) car nous considérons ce point comme étant le centre du système stellaire
     */
    Star(int rayon, float mass) : Luminary(rayon, mass, {0,0}) {}

    Star() = delete;
};
