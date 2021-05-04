#pragma once

/**
 * @class Cette classe repr�sente une �toile autour de laquelle tournent les plan�tes du syt�me stellaire
 * Cette �toile se trouve au centre du syst�me stellaire.
 * La classe 'Star' h�rite de la classe 'Luminary'
 */
class Star : public Luminary
{
public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    /**
     * @brief Construit une �toile
     *
     * @param radius Taille de l'�toile (en m)
     * @param mass Masse de l'�toile (en Kg)
     * @param centre Centre de l'�toile. La position est initialis�e � (0,0) car nous consid�rons ce point comme �tant le centre du syst�me stellaire
     * @param name Nom de l'�toile
     */
    Star(float radius, double mass, Point2D<int> centre, std::string name) : Luminary(radius, mass, centre, name) {}

    Star() = delete;
};