#pragma once
/**
 * @file Quaternion.h
 * @brief Quaternion template class for 3D rotations.
 * @ingroup MathsLib
 *
 * Quaternion convention: q = w + xi + yj + zk
 * with the following multiplication rules:
 *   j*i = -k,  j*k = i,  i*k = -j,  i*i = j*j = k*k = -1
 */

#include <cmath>
#include <algorithm>
#include <string>
#include <numbers>
#include "Vector3.h"

namespace MathsLib
{
    /**
     * @brief Represents a rotation in 3D space as a unit quaternion.
     *
     * @tparam T Scalar type (typically float or double).
     *
     * Provides construction, normalisation, SLERP/LERP interpolation,
     * Euler-angle conversion, and static factory methods identical in spirit
     * to Unity's Quaternion API.
     */
    template<typename T>
    class Quaternion
    {
    public:
        T w; ///< Scalar (real) part.
        T x; ///< i imaginary component.
        T y; ///< j imaginary component.
        T z; ///< k imaginary component.

        // ── Constructors ────────────────────────────────────────────────────

        /// @brief Default constructor – identity quaternion (w=1, x=y=z=0).
        Quaternion();

        /**
         * @brief Constructs a quaternion with explicit components.
         * @param w  Scalar part.
         * @param x  i component.
         * @param y  j component.
         * @param z  k component.
         */
        Quaternion(T w, T x, T y, T z);

        // ── Static constants ─────────────────────────────────────────────────

        static Quaternion identity; ///< Identity rotation (no rotation).

        // ── Properties ───────────────────────────────────────────────────────

        /// @brief Returns this quaternion normalised (does not modify this).
        Quaternion normalized() const;

        /// @brief Returns the magnitude (norm) of this quaternion.
        T magnitude() const;

        /// @brief Normalises this quaternion in place.
        void Normalize();

        // ── Instance methods ─────────────────────────────────────────────────

        /**
         * @brief Checks equality with another quaternion (component-wise).
         * @param other  Quaternion to compare.
         */
        bool Equals(const Quaternion& other) const;

        /**
         * @brief Sets all four components at once.
         * @param w  Scalar part.
         * @param x  i component.
         * @param y  j component.
         * @param z  k component.
         */
        void Set(T w, T x, T y, T z);

        /**
         * @brief Sets this quaternion to the rotation from @p from to @p to.
         * @param from  Source direction (unit vector recommended).
         * @param to    Target direction (unit vector recommended).
         */
        void SetFromToRotation(const Vector3<T>& from, const Vector3<T>& to);

        /**
         * @brief Sets this quaternion so that the forward axis aligns with @p forward.
         * @param forward  Target forward direction.
         * @param up       World up vector (default: Vector3::up).
         */
        void SetLookRotation(const Vector3<T>& forward, const Vector3<T>& up = Vector3<T>::up);

        /**
         * @brief Decomposes this quaternion into an angle-axis representation.
         * @param[out] angle  Rotation angle in radians.
         * @param[out] axis   Unit rotation axis.
         */
        void ToAngleAxis(T& angle, Vector3<T>& axis) const;

        /**
         * @brief Returns a quaternion representing a rotation of @p degrees
         *        around the given @p axis (non-static overload).
         * @param degrees  Rotation in degrees.
         * @param axis     Rotation axis (will be normalised internally).
         */
        Quaternion<T> AngleAxis(float degrees, Vector3<T>& axis);

        /// @brief Returns a string representation, e.g. "(w, x, y, z)".
        std::string toString() const;

        // ── Static methods ───────────────────────────────────────────────────

        /**
         * @brief Returns the angle in degrees between quaternions @p a and @p b.
         */
        static T Angle(const Quaternion<T>& a, const Quaternion<T>& b);

        /**
         * @brief Creates a rotation of @p angle degrees around @p axis.
         * @param angle  Rotation in degrees.
         * @param axis   Unit rotation axis.
         */
        static Quaternion<T> AngleAxis(T angle, const Vector3<T>& axis);

        /// @brief Returns the dot product of @p a and @p b.
        static T Dot(const Quaternion<T>& a, const Quaternion<T>& b);

        /**
         * @brief Creates a quaternion from Euler angles (degrees).
         * @param x  Pitch (rotation around X).
         * @param y  Yaw   (rotation around Y).
         * @param z  Roll  (rotation around Z).
         */
        static Quaternion<T> Euler(T x, T y, T z);

        /**
         * @brief Returns the rotation that rotates @p from to @p to.
         */
        static Quaternion<T> FromToRotation(const Vector3<T>& from, const Vector3<T>& to);

        /// @brief Returns the inverse of @p q (conjugate for unit quaternions).
        static Quaternion<T> Inverse(const Quaternion<T>& q);

        /**
         * @brief Linear interpolation between @p a and @p b, clamped to [0,1].
         * @param t  Blend factor.
         */
        static Quaternion<T> Lerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /// @brief Linear interpolation without clamping @p t.
        static Quaternion<T> LerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /**
         * @brief Creates a quaternion whose forward axis aligns with @p forward.
         * @param forward   Target forward direction.
         * @param upwards   World up vector.
         */
        static Quaternion<T> LookRotation(const Vector3<T>& forward,
                                           const Vector3<T>& upwards = Vector3<T>::up());

        /**
         * @brief Rotates @p from toward @p to by at most @p maxDegreesDelta degrees.
         */
        static Quaternion<T> RotateTowards(const Quaternion<T>& from,
                                            const Quaternion<T>& to,
                                            T maxDegreesDelta);

        /**
         * @brief Spherical linear interpolation clamped to [0,1].
         * @param t  Blend factor.
         */
        static Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        /// @brief Spherical linear interpolation without clamping @p t.
        static Quaternion<T> SlerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);

        // ── Operators ────────────────────────────────────────────────────────

        /// @brief Quaternion multiplication (composes two rotations).
        Quaternion operator*(const Quaternion& rhs) const;

        /// @brief Scalar multiplication.
        Quaternion operator*(T scalar) const;

        /// @brief Component-wise addition.
        Quaternion operator+(const Quaternion& rhs) const;

        /// @brief Component-wise subtraction.
        Quaternion operator-(const Quaternion& rhs) const;

        /// @brief Component-wise equality comparison.
        bool operator==(const Quaternion& rhs) const;

        /// @brief Index access: 0=w, 1=x, 2=y, 3=z.
        const T& operator[](int index) const;
    };
} // namespace MathsLib

#include "Quaternion.inl"
