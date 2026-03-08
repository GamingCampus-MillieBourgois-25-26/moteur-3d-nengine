#pragma once
/**
 * @file Matrice3.h
 * @brief 3x3 matrix template class.
 * @ingroup MathsLib
 */

namespace MathsLib
{
    /**
     * @brief A column-major 3×3 matrix.
     *
     * @tparam T Scalar type (e.g. float, double).
     *
     * Provides basic arithmetic operators and an equality test.
     * Used for 2D linear transformations and 3D rotation matrices.
     */
    template<typename T>
    class Matrice3
    {
    public:
        T m[3][3]; ///< Raw 3×3 storage, accessed as m[row][col].

        /// @brief Default constructor – leaves elements uninitialised.
        Matrice3<T>() = default;

        /**
         * @brief Constructs a matrix with explicit element values (row-major order).
         * @param a  m[0][0]  @param b  m[0][1]  @param c  m[0][2]
         * @param d  m[1][0]  @param e  m[1][1]  @param f  m[1][2]
         * @param g  m[2][0]  @param h  m[2][1]  @param i  m[2][2]
         */
        Matrice3<T>(
            T a = 0.0f, T b = 0.0f, T c = 0.0f,
            T d = 0.0f, T e = 0.0f, T f = 0.0f,
            T g = 0.0f, T h = 0.0f, T i = 0.0f
        );

        /// @brief Matrix multiplication.
        Matrice3<T> operator*(Matrice3<T>& rhs);

        /// @brief Component-wise matrix addition.
        Matrice3<T> operator+(Matrice3<T>& rhs);

        /// @brief Component-wise matrix subtraction.
        Matrice3<T> operator-(Matrice3<T>& rhs);

        /// @brief Adds a scalar to every element.
        Matrice3<T> operator+(T scalar);

        /// @brief Subtracts a scalar from every element.
        Matrice3<T> operator-(T scalar);

        /// @brief Returns true if all elements are equal.
        bool operator==(const Matrice3<T>& rhs);
    };
} // namespace MathsLib

#include "Matrice3.inl"
