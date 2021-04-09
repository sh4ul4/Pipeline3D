#pragma once

template <size_t T1,size_t T2>
class Matrix {
public:
    //tableau à 2 dimensions représentant la matrice
    std::array<std::array<float, T1>, T2> m;

    const size_t rows;
    const size_t columns;

    float get(const int& row, const int& column) {
        return m[row][column];
    }

    void set(const int& row, const int& column, const float& value) {
        m[row][column] = value;
    }

    //Initialisation d'un matrice de taille T1*T2
    Matrix() :rows(T1), columns(T2) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = 0;
    }

    Matrix(std::array<std::array<float, T1>, T2> src) :rows(T1), columns(T2) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = src[i][j];
    }

    Matrix& operator=(const Matrix<T1,T2>& other) {
        if (other.rows != rows || other.columns != columns) {
            std::cout << "error" << std::endl;
            exit(1);
        }
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = other.m[i][j];
        return *this;
    }

    Matrix& operator=(const std::array<std::array<float,T1>,T2>& other) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = other[i][j];
        return *this;
    }

    /*
     * @brief renvoie l'addition de 2 matrices
     *
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    //Matrix& operator+=(const Matrix& m2);

    /*
     * @brief renvoie la soustraction de 2 matrices
     *
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice additionée
    */
    //Matrix& operator-=(const Matrix& m2);

    /*
     *@brief renvoie la multiplication de 2 matrices
     *
     * @param this la première matrice additionée
     * @param m2 la deuxième matrice multiplié
    */
    //Matrix& operator*=(const Matrix& m2);

    /*
     * @brief renvoie la matrice inverse
     */
    //Matrix inverse() const;
    /*
     * @brief renvoie la transposée de la matrice
    */
    //Matrix transpose(const Matrix& m);
    /*
     * @brief  multiplie la matrice par un scalaire
     *
     * @param f le scalaire
     */

    //void scalarMult(const float f);
    /*
     * @brief renvoie le déterminant de la matrice
    */
    //float determinantM4d();

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
};

template <size_t T1, size_t T2>
inline const Matrix<T1,T2> operator+(const Matrix<T1, T2>& v1, const Matrix<T1, T2>& v2) {
    return Matrix();
}

template <size_t T1, size_t T2>
inline const Matrix<T1, T2> operator-(const Matrix<T1, T2>& v1, const Matrix<T1, T2>& v2) {
    return Matrix();
}

template <size_t T1, size_t T2>
inline const Matrix<T1, T2> operator*(const Matrix<T1, T2>& v1, const Matrix<T1, T2>& v2) {
    return Matrix();
}

inline void optimizedProduct(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2, Matrix<4, 4>& dest) {
    dest.m[0] =
    {
        m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m2.m[3][0],
        m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m2.m[3][1],
        m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m2.m[3][2],
        m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m2.m[3][3]
    };
}

// length of relevant 3D coordinates in 4x4 matrix
inline float optimizedLength(const Matrix<4, 4>& m) {
    return sqrtf(m.m[0][0] * m.m[0][0] + m.m[0][1] * m.m[0][1] + m.m[0][2] * m.m[0][2]);
}