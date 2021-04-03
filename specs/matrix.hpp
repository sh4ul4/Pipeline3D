namespace matrtix{

    //défintiond'un type dune matrice de taille 4*4 avec un tableau en 2 dimensions
    typedef std::array<std::array<float,4>,4> m4d;
    typedef std::array<std::array<float,3>,3> m3d;
    typedef std::array<std::array<float,2>,2> m2d;
    
    //copié une matrice de taille 4*4
    m4d copyM4d(const m4d& m);
    //addition entre 2 matrice de taille 4*4
    void addM4d(const m4d& m1,const m4d& m2,m4d& res);
    //soustraction entre 2 matrice de taille 4*4
    void subM4d(const m4d& m1,const m4d& m2,m4d& res);
    //multiplication d'un matrice 4*4 avec un scalaire
    void scalarMultM4d(m4d& m,const float f);
    //multiplication entre 2 matrice de taille 4*4
    void multM4d(const m4d& m1, const m4d& m2,m4d& res);
    //inversion de matrice de taille 4*4
    void invertM4d(m4d& m);
    //transposée d'un matrice de taille 4*4
    void transposeM4d(m4d& m);
    //renvoie le déterminant de la matrice 4*4
    int determinantM4d(const m4d m);
    

    //tab 
    //Constructeur
    //vecteur
    //

}
}