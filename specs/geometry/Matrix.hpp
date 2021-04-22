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

    Matrix(const std::array<std::array<float, T1>, T2>& src) {
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