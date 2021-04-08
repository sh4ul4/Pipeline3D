#include <iostream>

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
    int rayon;
    // Masse de la planète (en Kg --> important pour les formules physiques)
    int mass;
    // Position en x et y du corps celeste
    std::pair<float, float> position;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    /**
     * @brief Construit un corps celeste
     * 
     * @param rayon Rayon (en mètre) du corps celeste
     * @param mass Masse (en Kg) du corps celeste
     * @param position Paire contenant la coordonnée x et y du centre du corps celeste
     */
    Luminary(int rayon, int mass, std::pair<float, float> &position);

    Luminary() = delete;

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Affecte un nouveau rayon à la planète courante
     * 
     * @param rayon Nouveau rayn à affecté
     */
    void setRayon(int rayon);

    /**
     * @brief Affecte une nouvelle masse à la planète courante
     * 
     * @param masse Nouvelle masse à affectée
     */
    void setMass(int mass);

    /**
     * @brief Affecte une nouvelle position au corps celeste
     * 
     * @param position 
     */
    void setPosition(std::pair<float, float> &position);

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère le rayon de la planète courante
     * 
     * @return Retourne le rayon de la planète (int)
     */
    int getRayon();

    /**
     * @brief Récupère la masse de la planète courante
     * 
     * @return Retourn la masse de la planète (int) 
     */
    int getMass();

    /**
     * @brief Récupère les coordonnées du centre du corps celeste
     * 
     * @return std::pair<float, float> 
     */
    std::pair<float, float> getPosition();
};