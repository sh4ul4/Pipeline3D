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
    std::pair<float, float> speed;
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
     * @param rayon Rayon (en mètre) de la planète
     * @param mass Masse (en Kg) de la planète
     * @param position Position en x et y de la planète
     * @param speed Vitesse de la planète autour
     * @param visible Détermine la visibilité de la planète
     */
    Planet(int rayon, int mass, std::pair<float, float> &position, std::pair<float, float> &speed, bool visible = true) : Luminary(rayon, mass, position) {}

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Affecte une nouvelle vitesse sur les deux axes (x,y)
     * 
     * @param speed Nouvelle vitesse à affecter : speed.first --> axe x, speed.second --> axe y
     */
    void setSpeed(const std::pair<float, float> &speed);

    // Change la visiblité de la planète
    void changeVisibility();

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la distance qui sépare le centre de la planète courante du centre de l'étoile
     * 
     * @return Retourne la distance (int) 
     */
    int getDistance();

    /* Je ne pense qu'il soit intéressant de récupérer la vitesse initiale d'une planète */

    /**
     * @brief Récupère la vitesse sur l'axe x et la vitesse sur l'axe y
     * 
     * @return Retourne un std::pair : first --> speedX, second --> speedY 
     */
    std::pair<float, float> getSpeed();
};