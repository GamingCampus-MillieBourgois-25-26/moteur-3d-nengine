#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <numbers>

namespace MathsLib
{
    /**
     * @brief Vecteur 3D générique.
     *
     * Fournit les opérations mathématiques usuelles
     * pour la manipulation de vecteurs 3D.
     *
     * @tparam T Type des composantes du vecteur
     */
    template <typename T>
    class Vector3
    {
    public:
        T x, y, z;

        /** @brief Construit un vecteur nul. */
        Vector3();

        /**
         * @brief Construit un vecteur 3D.
         */
        Vector3(T x, T y, T z);

        /** @brief Vecteur pointant vers l'arrière. */
        static Vector3 back;

        /** @brief Vecteur pointant vers le bas. */
        static Vector3 down;

        /** @brief Vecteur pointant vers l'avant. */
        static Vector3 forward;

        /** @brief Vecteur pointant vers la gauche. */
        static Vector3 left;

        /** @brief Vecteur pointant vers la droite. */
        static Vector3 right;

        /** @brief Vecteur pointant vers le haut. */
        static Vector3 up;

        /** @brief Vecteur nul. */
        static Vector3 zero;

        /** @brief Vecteur unitaire. */
        static Vector3 one;

        /** @brief Vecteur représentant l'infini négatif. */
        static Vector3 negativeInfinity;

        /** @brief Vecteur représentant l'infini positif. */
        static Vector3 positiveInfinity;

        /**
         * @brief Calcule la magnitude du vecteur.
         * @return Longueur du vecteur
         */
        T magnitude() const;

        /**
         * @brief Retourne un vecteur normalisé.
         * @return Vecteur normalisé
         */
        Vector3 normalized() const;

        /**
         * @brief Calcule la magnitude au carré du vecteur.
         * @return Magnitude au carré
         */
        T sqrMagnitude() const;

        /**
         * @brief Compare deux vecteurs.
         * @param other Vecteur à comparer
         * @return true si les vecteurs sont égaux
         */
        bool Equals(const Vector3& other) const;

        /** @brief Normalise le vecteur. */
        void Normalize();

        /**
         * @brief Définit les composantes du vecteur.
         */
        void Set(T X, T Y, T Z);

        /**
         * @brief Convertit le vecteur en chaîne de caractères.
         */
        std::string toString();

        /** @brief Addition de deux vecteurs. */
        Vector3 operator+(const Vector3&) const;

        /** @brief Soustraction de deux vecteurs. */
        Vector3 operator-(const Vector3&) const;

        /** @brief Multiplication composante par composante. */
        Vector3 operator*(Vector3&) const;

        /** @brief Multiplication par un scalaire. */
        Vector3 operator*(T) const;

        /** @brief Division par un scalaire. */
        Vector3 operator/(T) const;

        /**
         * @brief Accède à une composante du vecteur.
         */
        const T& operator[](int) const;

        /** @brief Compare deux vecteurs. */
        bool operator/=(const Vector3&) const;

        /** @brief Compare deux vecteurs. */
        bool operator==(const Vector3&) const;

        /**
         * @brief Calcule le produit vectoriel de deux vecteurs.
         */
        static Vector3 Cross(const Vector3&, const Vector3&);

        /**
         * @brief Orthonormalise deux vecteurs.
         */
        static Vector3 OrthoNormalize(const Vector3&, const Vector3&);

        /**
         * @brief Projette un vecteur sur un autre.
         */
        static Vector3 Project(const Vector3&, const Vector3&);

        /**
         * @brief Projette un vecteur sur un plan.
         */
        static Vector3 ProjectOnPlane(const Vector3&, const Vector3);

        /**
         * @brief Fait tourner un vecteur vers un autre avec une limite.
         */
        static Vector3 RotateTowards(const Vector3&, const Vector3&, T);

        /**
         * @brief Interpolation sphérique entre deux vecteurs.
         */
        static Vector3 Slerp(const Vector3&, const Vector3&, T);

        /**
         * @brief Interpolation sphérique non bornée entre deux vecteurs.
         */
        static Vector3 SlerpUnclamped(const Vector3& from, const Vector3& to, T t);

        /**
         * @brief Calcule l'angle entre deux vecteurs.
         */
        static T Angle(const Vector3&, const Vector3&);

        /**
         * @brief Limite la magnitude d'un vecteur.
         */
        static Vector3 ClampMagnitude(const Vector3&, T);

        /**
         * @brief Calcule la distance entre deux vecteurs.
         */
        static T Distance(const Vector3&, const Vector3&);

        /**
         * @brief Calcule le produit scalaire de deux vecteurs.
         */
        static T Dot(const Vector3&, const Vector3&);

        /**
         * @brief Interpole linéairement entre deux vecteurs.
         */
        static Vector3 Lerp(const Vector3&, const Vector3&, T);

        /**
         * @brief Interpolation linéaire non bornée entre deux vecteurs.
         */
        static Vector3 LerpUnclamped(const Vector3&, const Vector3&, T);

        /**
         * @brief Retourne le maximum composante par composante.
         */
        static Vector3 Max(const Vector3&, const Vector3&);

        /**
         * @brief Retourne le minimum composante par composante.
         */
        static Vector3 Min(const Vector3&, const Vector3&);

        /**
         * @brief Déplace un vecteur vers un autre avec une limite.
         */
        static Vector3 MoveTowards(const Vector3&, const Vector3&, T);

        /**
         * @brief Réfléchit un vecteur par rapport à une normale.
         */
        static Vector3 Reflect(const Vector3&, const Vector3&);

        /**
         * @brief Multiplie deux vecteurs composante par composante.
         */
        static Vector3 Scale(const Vector3&, const Vector3&);

        /**
         * @brief Calcule l'angle signé entre deux vecteurs.
         */
        static T SignedAngle(const Vector3&, const Vector3&, const Vector3&);
    };
}

#include "Vector3.inl"
