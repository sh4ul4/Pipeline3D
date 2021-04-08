namespace maths{
    /*@brief Renvoie le barycentre d'un triangle
     *@param t un triangle
    */
    Vertex getBarycentre(Triangle t);

    /* @brief Renvoie le vecteur normale au plan du triangle
     * @param t un triangle
    */
    Vertex getNormalVector(Triangle t);

    /*@brief Renvoie la distance entre les deux sommets
     *@param s1 premier sommet
     *@param s2 deuxième sommet
    */
    float getDistancePoint(const Vertex& s1,const Vertex& s2);

    /*
     * @brief renvoie le sommet qui est l'intersection entre un segment et un plan
     * @param s1 le premier sommet du segment
     * @param s2 le deuxième sommet du segment 
     * @param p un point de plan
     * @param v1 un vecteur du plan
     * @param v2 un vexteur du plan
    */
    Vertex intersectionSegmentPlane(const Vertex& s1,const Vertex& s2,const Vertex& p,const Vector& v1,const Vector& v2);

    



}