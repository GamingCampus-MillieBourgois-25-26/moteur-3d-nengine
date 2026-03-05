#pragma once
#include <stdexcept>
#include <string>
#include "Quaternion.h"
#include "Vector3.h"

namespace MathsLib
{
    /**
     * @brief Matrice 4x4 générique.
     *
     * Fournit les opérations mathématiques usuelles
     * pour les transformations 3D (translation, rotation,
     * mise à l'échelle, projection).
     *
     * @tparam T Type des éléments de la matrice
     */
    template <typename T>
    class Matrice4
    {
    public:
        T m4[4][4];

        /**
         * @brief Construit une matrice 4x4 à partir de ses coefficients.
         */
        Matrice4<T>(
            T a = 0, T b = 0, T c = 0, T d = 0,
            T e = 0, T f = 0, T g = 0, T h = 0,
            T i = 0, T j = 0, T k = 0, T l = 0,
            T m = 0, T n = 0, T o = 0, T p = 0
        );

#pragma region Static properties

        /** @brief Matrice identité. */
        static Matrice4 identity;

        /** @brief Matrice nulle. */
        static Matrice4 zero;

#pragma endregion

#pragma region Properties

        /**
         * @brief Calcule le déterminant de la matrice.
         * @return Déterminant de la matrice
         */
        T determinant() const;

        /**
         * @brief Calcule l'inverse de la matrice.
         * @return Matrice inverse
         */
        Matrice4 inverse() const;

        /**
         * @brief Indique si la matrice est une identité.
         * @param epsilon Tolérance de comparaison
         * @return true si la matrice est une identité
         */
        bool isIdentity(T epsilon = static_cast<T>(1e-6)) const;

        /**
         * @brief Extrait la rotation de la matrice.
         * @return Quaternion représentant la rotation
         */
        Quaternion<T> rotation() const;

        /**
         * @brief Transpose la matrice.
         * @return Matrice transposée
         */
        Matrice4 transpose() const;

#pragma endregion

#pragma region StaticMethods

        /**
         * @brief Crée une matrice de projection orthographique.
         */
        Matrice4 ortho(T, T, T, T, T, T);

        /**
         * @brief Crée une matrice de projection perspective.
         */
        Matrice4 perspective(T, T, T, T);

        /**
         * @brief Crée une matrice de rotation à partir d'un quaternion.
         */
        Matrice4 rotate(Quaternion<T>& q);

        /**
         * @brief Crée une matrice de mise à l'échelle.
         */
        Matrice4 scale(Vector3<T>& vector);

        /**
         * @brief Crée une matrice de translation.
         */
        Matrice4 translate(Vector3<T>& vector);

        /**
         * @brief Crée une matrice TRS (Translation, Rotation, Scale).
         */
        Matrice4 trs(Vector3<T>& pos, Quaternion<T>& q, Vector3<T>& s);

#pragma endregion

#pragma region Operators

        /**
         * @brief Multiplie deux matrices 4x4.
         * @param m4 Matrice à multiplier
         * @return Résultat de la multiplication
         */
        Matrice4 operator*(const Matrice4& m4);

        /**
         * @brief Accède à une ligne de la matrice.
         * @param row Index de la ligne
         * @return Pointeur vers la ligne
         */
        T* operator[](int row) { return m4[row]; }

        /**
         * @brief Accède à une ligne de la matrice (const).
         * @param row Index de la ligne
         * @return Pointeur constant vers la ligne
         */
        const T* operator[](int row) const { return m4[row]; }

#pragma endregion

#pragma region Public Methods

        /**
         * @brief Récupère une colonne de la matrice.
         */
        T GetColumn(int index);

        /**
         * @brief Récupère la position contenue dans la matrice.
         */
        Vector3<T> GetPosition();

        /**
         * @brief Récupère une ligne de la matrice.
         */
        T GetRow(int index);

        /**
         * @brief Multiplie un point par la matrice.
         */
        Vector3<T> MultiplyPoint(Vector3<T> position);

        /**
         * @brief Multiplie un point par une matrice 3x4.
         */
        Vector3<T> MultiplyPoint3x4(Vector3<T> position);

        /**
         * @brief Multiplie un vecteur direction par la matrice.
         */
        Vector3<T> MultiplyPointVector(Vector3<T> direction);

        /**
         * @brief Définit une colonne de la matrice.
         */
        void SetColumn(int index, T a, T b, T c, T d);

        /**
         * @brief Définit une ligne de la matrice.
         */
        void SetRow(int index, T a, T b, T c, T d);

        /**
         * @brief Convertit la matrice en chaîne de caractères.
         */
        std::string ToString();

#pragma endregion
    };
}
#include "Matrice4.inl"
