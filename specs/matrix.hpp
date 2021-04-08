template <size_t T1,size_t T2>
class matrix{
    public:
    //tableau à 2 dimensions représentant la matrice
    std::array<std::array<float,T1>,T2> m;

    //Initialisation d'un matrice de taille T1*T2
    matrix();
    /*
     * @brief Renvoie une copie de la matrice
    */
    matrix copy();

    /*
     * @brief renvoie l'addition de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    matrix operator+(const matrix& m2);

    /*
     * @brief renvoie la soustraction de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    matrix operator-(const matrix& m2);

    /*
     *@brief renvoie la multiplication de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice multiplié
    */
    matrix operator*(const matrix& m2);

    /*
     * @brief renvoie la matrice inverse
     */
    matrix invert();
    /*
     * @brief renvoie la transposée de la matrice
    */
    matrix transpose(m4d& m);
    /*
     * @brief  multiplie la matrice par un scalaire
     * 
     * @param f le scalaire
     */
  
    void scalarMult(const float f);
    /*
     * @brief renvoie le déterminant de la matrice
    */
    float determinantM4d();

    /*
     * @brief affiche la matrice
    */
    void print();
   
}
