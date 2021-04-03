namespace maths{
    /*@brief Renvoie le barycentre d'un triangle
     *@param t un triangle
    */
    Sommet getBarycentre(Triangle t);

    /* @brief Renvoie le vecteur normale au plan du triangle
     * @param t un triangle
    */
    Sommet getNormalVector(Triangle t);

    /*@brief Renvoie la distance entre les deux sommets
     *@param s1 premier sommet
     *@param s2 deuxième sommet
    */
    float getDistancePoint(Sommet s1,Sommet s2);



}