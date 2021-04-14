#pragma once

template <size_t T1,size_t T2>
class Matrix {
public:
    //tableau à 2 dimensions représentant la matrice
    std::array<std::array<float, T1>, T2> m;

    float get(const int& row, const int& column) {
        return m[row][column];
    }

    void set(const int& row, const int& column, const float& value) {
        m[row][column] = value;
    }

    //Initialisation d'un matrice de taille T1*T2
    Matrix() {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = 0;
    }

    Matrix(const std::array<std::array<float, T1>, T2>& src) :rows(T1), columns(T2) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = src[i][j];
    }

    Matrix(const Matrix& other) {
        for (int i = 0; i < T1; i++)
            for (int j = 0; j < T2; j++)
                m[i][j] = other.m[i][j];
    }

    Matrix& operator=(const Matrix& other) {
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

inline Matrix<4, 4> optimizedProduct(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2) {
    Matrix<4, 4> res;
    res.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m2.m[3][0];
    res.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m2.m[3][1];
    res.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m2.m[3][2];
    res.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m2.m[3][3];
    return res;
}

// length of relevant 3D coordinates in 4x4 matrix
inline float optimizedLength(const Matrix<4, 4>& m) {
    return sqrtf(m.m[0][0] * m.m[0][0] + m.m[0][1] * m.m[0][1] + m.m[0][2] * m.m[0][2]);
}

Matrix<4,4> inverseMatrix(Matrix<4, 4> in) {
    float inv[16];
    const float m[16] = { in.m[0][0],in.m[1][0],in.m[2][0],in.m[3][0],
                            in.m[0][1],in.m[1][1],in.m[2][1],in.m[3][1],
                            in.m[0][2],in.m[1][2],in.m[2][2],in.m[3][2],
                            in.m[0][3],in.m[1][3],in.m[2][3],in.m[3][3] };

    const float m10x15 = m[10] * m[15];
    const float m11x14 = m[11] * m[14];
    const float m6x15 = m[6] * m[15];
    const float m7x14 = m[7] * m[14];
    const float m6x11 = m[6] * m[11];
    const float m7x10 = m[7] * m[10];
    const float m4x9 = m[4] * m[9];
    const float m11x13 = m[11] * m[13];
    const float m8x5 = m[8] * m[5];
    const float m7x13 = m[7] * m[13];
    const float m12x5 = m[12] * m[5];
    const float m7x9 = m[7] * m[9];
    const float m10x13 = m[10] * m[13];
    const float m2x15 = m[2] * m[15];
    const float m3x14 = m[3] * m[14];
    const float m12x2 = m[12] * m[2];
    const float m12x3 = m[12] * m[3];
    const float m6x13 = m[6] * m[13];
    const float m2x11 = m[2] * m[11];
    const float m3x10 = m[3] * m[10];
    const float m0x9 = m[0] * m[9];
    const float m8x1 = m[8] * m[1];
    const float m8x3 = m[8] * m[3];
    const float m12x1 = m[12] * m[1];
    const float m8x2 = m[8] * m[2];
    const float m2x7 = m[2] * m[7];
    const float m3x6 = m[3] * m[6];
    const float m0x5 = m[0] * m[5];
    const float m4x1 = m[4] * m[1];
    const float m4x3 = m[4] * m[3];
    const float m4x2 = m[4] * m[2];

    inv[0] = m[5] * m10x15 + m[9] * m7x14 + m[13] * m6x11 - (m[5] * m11x14 + m[9] * m6x15 + m[13] * m7x10);
    inv[4] = m[12] * m7x10 + m[4] * m11x14 + m[8] * m6x15 - (m[8] * m7x14 + m[12] * m6x11 + m[4] * m10x15);
    inv[8] = m4x9 * m[15] + m[8] * m7x13 + m12x5 * m[11] - (m[12] * m7x9 + m[4] * m11x13 + m8x5 * m[15]);
    inv[12] = m[4] * m10x13 + m8x5 * m[14] + m[12] * m[6] * m[9] - (m4x9 * m[14] + m[8] * m6x13 + m12x5 * m[10]);
    inv[1] = m[1] * m11x14 + m[9] * m2x15 + m[13] * m3x10 - (m[1] * m10x15 + m[9] * m3x14 + m[13] * m2x11);
    inv[5] = m[0] * m10x15 + m[8] * m3x14 + m12x2 * m[11] - (m12x3 * m[10] + m[0] * m11x14 + m[8] * m2x15);
    inv[9] = m[0] * m11x13 + m8x1 * m[15] + m12x3 * m[9] - (m8x3 * m[13] + m12x1 * m[11] + m0x9 * m[15]);
    inv[13] = m0x9 * m[14] + m8x2 * m[13] + m12x1 * m[10] - (m12x2 * m[9] + m[0] * m10x13 + m8x1 * m[14]);
    inv[2] = m[1] * m6x15 + m[5] * m3x14 + m[13] * m2x7 - (m[13] * m3x6 + m[1] * m7x14 + m[5] * m2x15);
    inv[6] = m12x3 * m[6] + m[0] * m7x14 + m[4] * m2x15 - (m[4] * m3x14 + m12x2 * m[7] + m[0] * m6x15);
    inv[10] = m0x5 * m[15] + m4x3 * m[13] + m12x1 * m[7] - (m12x3 * m[5] + m[0] * m7x13 + m4x1 * m[15]);
    inv[14] = m12x2 * m[5] + m[0] * m6x13 + m4x1 * m[14] - (m4x2 * m[13] + m12x1 * m[6] + m0x5 * m[14]);
    inv[3] = m[9] * m3x6 + m[1] * m7x10 + m[5] * m2x11 - (m[5] * m3x10 + m[9] * m2x7 + m[1] * m6x11);
    inv[7] = m[0] * m6x11 + m[4] * m3x10 + m8x2 * m[7] - (m8x3 * m[6] + m[0] * m7x10 + m[4] * m2x11);
    inv[11] = m[0] * m7x9 + m4x1 * m[11] + m8x3 * m[5] - (m8x1 * m[7] + m0x5 * m[11] + m4x3 * m[9]);
    inv[15] = m0x5 * m[10] + m4x2 * m[9] + m8x1 * m[6] - (m8x2 * m[5] + m[0] * m[6] * m[9] + m4x1 * m[10]);

    float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return Matrix<4, 4>();

    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        inv[i] *= det;

    for (int i = 0; i < 16; i++) {
        in.m[(int)(i / 4)][(int)(i % 4)] = m[i]; // transposition
    }
    return in;
}