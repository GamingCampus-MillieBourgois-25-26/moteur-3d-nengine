#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <numbers>

namespace MathsLib
{
    /**
     * @brief Vecteur 2D générique.
     *
     * Fournit les opérations mathématiques usuelles
     * sur des vecteurs 2D.
     *
     * @tparam T Type des composantes du vecteur
     */
    template <typename T>
    class Vector2
    {
    public:
        /** @brief Construit un vecteur nul. */
        Vector2<T>() = default;

        /**
         * @brief Construit un vecteur 2D.
         */
        Vector2<T>(T X = 0.0f, T Y = 0.0f);

#pragma region Static Properties

        /** @brief Vecteur pointant vers le bas. */
        static Vector2 down;

        /** @brief Vecteur pointant vers la gauche. */
        static Vector2 left;

        /** @brief Vecteur pointant vers la droite. */
        static Vector2 right;

        /** @brief Vecteur pointant vers le haut. */
        static Vector2 up;

        /** @brief Vecteur nul. */
        static Vector2 zero;

        /** @brief Vecteur unitaire. */
        static Vector2 one;

        /** @brief Vecteur représentant l'infini négatif. */
        static Vector2 negativeInfinity;

        /** @brief Vecteur représentant l'infini positif. */
        static Vector2 positiveInfinity;

#pragma endregion

#pragma region Properties

        T x, y;

        /**
         * @brief Calcule la magnitude du vecteur.
         * @return Longueur du vecteur
         */
        T magnitude() const;

        /**
         * @brief Retourne un vecteur normalisé.
         * @return Vecteur normalisé
         */
        Vector2<T> normalized() const;

        /**
         * @brief Calcule la magnitude au carré du vecteur.
         * @return Magnitude au carré
         */
        T sqrMagnitude() const;

#pragma endregion

#pragma region Public Methods

        /**
         * @brief Compare deux vecteurs.
         * @param other Vecteur à comparer
         * @return true si les vecteurs sont égaux
         */
        bool Equals(const Vector2& other) const;

        /** @brief Normalise le vecteur. */
        void Normalize();

        /**
         * @brief Définit les composantes du vecteur.
         */
        void Set(T X, T Y);

        /**
         * @brief Convertit le vecteur en chaîne de caractères.
         */
        std::string toString();

#pragma endregion

#pragma region Static Methods

        /**
         * @brief Calcule l'angle entre deux vecteurs.
         */
        static T Angle(const Vector2&, const Vector2&);

        /**
         * @brief Limite la magnitude d'un vecteur.
         */
        static Vector2 ClampMagnitude(const Vector2&, T);

        /**
         * @brief Calcule la distance entre deux vecteurs.
         */
        static T Distance(const Vector2&, const Vector2&);

        /**
         * @brief Calcule le produit scalaire de deux vecteurs.
         */
        static T Dot(const Vector2&, const Vector2&);

        /**
         * @brief Interpole linéairement entre deux vecteurs.
         */
        static Vector2 Lerp(const Vector2&, const Vector2&, T);

        /**
         * @brief Interpolation linéaire non bornée entre deux vecteurs.
         */
        static Vector2 LerpUnclamped(const Vector2&, const Vector2&, T);

        /**
         * @brief Retourne le maximum composante par composante.
         */
        static Vector2 Max(const Vector2&, const Vector2&);

        /**
         * @brief Retourne le minimum composante par composante.
         */
        static Vector2 Min(const Vector2&, const Vector2&);

        /**
         * @brief Déplace un vecteur vers un autre avec une limite.
         */
        static Vector2 MoveTowards(const Vector2&, const Vector2&, T);

        /**
         * @brief Retourne un vecteur perpendiculaire.
         */
        static Vector2 Perpandicular(const Vector2&);

        /**
         * @brief Réfléchit un vecteur par rapport à une normale.
         */
        static Vector2 Reflect(const Vector2&, const Vector2&);

        /**
         * @brief Multiplie deux vecteurs composante par composante.
         */
        static Vector2 Scale(const Vector2&, const Vector2&);

        /**
         * @brief Calcule l'angle signé entre deux vecteurs.
         */
        static T SignedAngle(const Vector2&, const Vector2&);

#pragma endregion 

#pragma region Operators

        /**
         * @brief Conversion implicite vers un autre type vectoriel.
         */
        template<typename TcT>
        operator TcT() { return TcT{ x, y }; }

        /** @brief Addition de deux vecteurs. */
        Vector2 operator+(const Vector2&) const;

        /** @brief Soustraction de deux vecteurs. */
        Vector2 operator-(const Vector2&) const;

        /** @brief Multiplication par un scalaire. */
        Vector2 operator*(T) const;

        /** @brief Division par un scalaire. */
        Vector2 operator/(T) const;

        /** @brief Compare deux vecteurs. */
        bool operator/=(const Vector2&) const;

        /** @brief Compare deux vecteurs. */
        bool operator==(const Vector2&) const;

#pragma endregion
    };
}

#include "Vector2.inl"
