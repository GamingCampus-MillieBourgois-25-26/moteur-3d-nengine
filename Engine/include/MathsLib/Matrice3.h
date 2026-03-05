#pragma once
namespace MathsLib {

    /**
     * @brief Matrice 3x3 générique.
     *
     * Fournit les opérations mathématiques de base
     * sur une matrice 3x3.
     *
     * @tparam T Type des éléments de la matrice
     */
    template<typename T>
    class Matrice3
    {
    public:
        T m[3][3];

        /** @brief Construit une matrice 3x3 par défaut. */
        Matrice3<T>() = default;

        /**
         * @brief Construit une matrice 3x3 à partir de ses coefficients.
         */
        Matrice3<T>(
            T a = 0.0f, T b = 0.0f, T c = 0.0f,
            T d = 0.0f, T e = 0.0f, T f = 0.0f,
            T g = 0.0f, T h = 0.0f, T i = 0.0f
        );

        /**
         * @brief Multiplie deux matrices 3x3.
         * @param other Matrice à multiplier
         * @return Résultat de la multiplication
         */
        Matrice3<T> operator*(Matrice3<T>& other);

        /**
         * @brief Additionne deux matrices 3x3.
         * @param other Matrice à additionner
         * @return Résultat de l'addition
         */
        Matrice3<T> operator+(Matrice3<T>& other);

        /**
         * @brief Soustrait deux matrices 3x3.
         * @param other Matrice à soustraire
         * @return Résultat de la soustraction
         */
        Matrice3<T> operator-(Matrice3<T>& other);

        /**
         * @brief Additionne un scalaire à la matrice.
         * @param value Valeur scalaire
         * @return Résultat de l'opération
         */
        Matrice3<T> operator+(T value);

        /**
         * @brief Soustrait un scalaire à la matrice.
         * @param value Valeur scalaire
         * @return Résultat de l'opération
         */
        Matrice3<T> operator-(T value);

        /**
         * @brief Compare deux matrices 3x3.
         * @param other Matrice à comparer
         * @return true si les matrices sont égales
         */
        bool operator==(const Matrice3<T>& other);
    };

}
#include "Matrice3.inl"
