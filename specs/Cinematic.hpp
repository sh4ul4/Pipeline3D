namespace Cinematic
{
    /* ----- FORMULES PHYSIQUES ----- */

    /**
     * @brief Calcul la force d'attraction entre deux corps
     * 
     * @param l1 Premier corp
     * @param l2 Deuxième corp
     * @return  
     */
    float attractionForce(Luminary *l1, luminary *l2);

    /**
     * @brief Calcul l'accélération d'un astre en fonction des forces exercées sur celui-ci
     * 
     * @param mass Masse de la planète
     * @param force Forces exercées sur la planète (il faudrait un std::vector des forces)
     * @return std::pair<float, float> --> vecteur accélération
     */
    std::pair<float, float> Newton2nd(float mass, std::pair<float, float> force);
}
