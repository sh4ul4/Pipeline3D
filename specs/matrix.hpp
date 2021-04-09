#pragma once

template <size_t T1,size_t T2>
class Matrix{
    public:
    //tableau à 2 dimensions représentant la matrice
    std::array<std::array<float,T1>,T2> m;

    const size_t rows;
    const size_t columns;

    //Initialisation d'un matrice de taille T1*T2
    Matrix() :rows(T1), columns(T2) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = 0;
    }
    /*
     * @brief Renvoie une copie de la matrice
    */
    Matrix& operator=(const Matrix& other) {
        if (other.rows != rows || other.columns != columns) {
            std::count << "error" << std::endl;
            return *this;
        }
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = other.m[i][j];
        return *this;
    }

    /*
     * @brief renvoie l'addition de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    Matrix operator+(const Matrix& m2);

    /*
     * @brief renvoie la soustraction de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    Matrix operator-(const Matrix& m2);

    /*
     *@brief renvoie la multiplication de 2 matrices
     * 
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice multiplié
    */
    Matrix operator*(const Matrix& m2);

    /*
     * @brief renvoie la matrice inverse
     */
    Matrix invert();
    /*
     * @brief renvoie la transposée de la matrice
    */
    Matrix transpose(m4d& m);
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
    void print() {
        for (int i = 0; i < m.size(); i++) {
            std::cout << "{ ";
            for (int j = 0; j < m[i].size(); j++) {
                std::cout << m[i][j] << " ";
            }
            std::cout << " }" << std::endl;
        }
    }
}
