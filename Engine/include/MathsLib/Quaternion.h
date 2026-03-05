#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <numbers>
#include "Vector3.h"

namespace MathsLib
{
    /**
     * @brief Quaternion générique.
     *
     * Représente une rotation dans l'espace 3D et fournit
     * les opérations mathématiques associées.
     *
     * @tparam T Type des composantes du quaternion
     */
    template<typename T>
    class Quaternion {
    public:
        T w, x, y, z;

        /** @brief Construit un quaternion par défaut. */
        Quaternion();

        /**
         * @brief Construit un quaternion à partir de ses composantes.
         */
        Quaternion(T w, T x, T y, T z);

        /**
         * @brief Retourne un quaternion normalisé.
         * @return Quaternion normalisé
         */
        Quaternion normalized() const;

        /**
         * @brief Calcule la norme du quaternion.
         * @return Magnitude du quaternion
         */
        T magnitude() const;

        /** @brief Normalise le quaternion. */
        void Normalize();

        /** @brief Quaternion identité. */
        static Quaternion identity;

        /**
         * @brief Compare deux quaternions.
         * @param other Quaternion à comparer
         * @return true si les quaternions sont égaux
         */
        bool Equals(const Quaternion& other) const;

        /**
         * @brief Définit les composantes du quaternion.
         */
        void Set(T w, T x, T y, T z);

        /**
         * @brief Définit une rotation entre deux directions.
         */
        void SetFromToRotation(const Vector3<T>& from, const Vector3<T>& to);

        /**
         * @brief Définit une rotation à partir d'une direction et d'un vecteur haut.
         */
        void SetLookRotation(const Vector3<T>& forward, const Vector3<T>& up = Vector3<T>::up);

        /**
         * @brief Convertit le quaternion en angle et axe.
         */
        void ToAngleAxis(T& angle, Vector3<T>& axis) const;

        /**
         * @brief Crée un quaternion à partir d'un angle et d'un axe.
         */
        Quaternion<T> AngleAxis(float degrees, Vector3<T>& axis);

        /**
         * @brief Convertit le quaternion en chaîne de caractères.
         */
        std::string toString() const;

        /**
         * @brief Calcule l'angle entre deux quaternions.
         */
        static T Angle(const Quaternion<T>& a, const Quaternion<T>& b);

        /**
         * @brief Crée un quaternion à partir d'un angle et d'un axe.
         */
        static Quaternion<T> AngleAxis(T angle, const Vector3<T>& axis);

        /**
         * @brief Calcule le produit scalaire de deux quaternions.
         */
        static T Dot(const Quaternion<T>& a, const Quaternion<T>& b);

        /**
         * @brief Crée un quaternion à partir d'angles d'Euler.
         */
        static Quaternion<T> Euler(T x, T y, T z);

        /**
         * @brief Crée une rotation entre deux directions.
         */
        static Quaternion<T> FromToRotation(const Vector3<T>& from, const Vector3<T>& to);

        /**
         * @brief Calcule l'inverse d'un quaternion.
         */
        static Quaternion<T> Inverse(const Quaternion<T>& q);

        /**
         * @brief Interpole linéairement entre deux quaternions.
         */
        static Quaternion<T> Lerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /**
         * @brief Interpolation linéaire non bornée entre deux quaternions.
         */
        static Quaternion<T> LerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /**
         * @brief Crée un quaternion orienté vers une direction.
         */
        static Quaternion<T> LookRotation(const Vector3<T>& forward, const Vector3<T>& upwards = Vector3<T>::up());

        /**
         * @brief Fait tourner un quaternion vers un autre avec une limite angulaire.
         */
        static Quaternion<T> RotateTowards(const Quaternion<T>& from, const Quaternion<T>& to, T maxDegreesDelta);

        /**
         * @brief Interpolation sphérique entre deux quaternions.
         */
        static Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /**
         * @brief Interpolation sphérique non bornée entre deux quaternions.
         */
        static Quaternion<T> SlerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /**
         * @brief Multiplie deux quaternions.
         */
        Quaternion operator*(const Quaternion& other) const;

        /**
         * @brief Multiplie le quaternion par un scalaire.
         */
        Quaternion operator*(T value) const;

        /**
         * @brief Additionne deux quaternions.
         */
        Quaternion operator+(const Quaternion& other) const;

        /**
         * @brief Soustrait deux quaternions.
         */
        Quaternion operator-(const Quaternion& other) const;

        /**
         * @brief Compare deux quaternions.
         */
        bool operator==(const Quaternion& other) const;

        /**
         * @brief Accède à une composante du quaternion.
         */
        const T& operator[](int index) const;
    };
}

#include "Quaternion.inl"
