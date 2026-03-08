#pragma once
/**
 * @file Vector3.h
 * @brief 3D vector template class.
 * @ingroup MathsLib
 */

#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <numbers>

namespace MathsLib
{
    /**
     * @brief Generic 3D vector with common math operations.
     *
     * @tparam T Scalar type (e.g. float, double).
     *
     * Provides arithmetic operators, dot/cross products, interpolation,
     * and direction constants (up, down, forward, back, left, right, zero, one).
     */
    template <typename T>
    class Vector3
    {
    public:
        T x; ///< X (right) component.
        T y; ///< Y (up) component.
        T z; ///< Z (forward/backward) component.

        // ── Constructors ────────────────────────────────────────────────────

        /// @brief Default constructor – initialises to (0, 0, 0).
        Vector3();

        /**
         * @brief Constructs a Vector3 with explicit coordinates.
         * @param x  X component.
         * @param y  Y component.
         * @param z  Z component.
         */
        Vector3(T x, T y, T z);

        // ── Static direction constants ───────────────────────────────────────

        static Vector3 back;             ///< (0, 0, -1)
        static Vector3 down;             ///< (0, -1, 0)
        static Vector3 forward;          ///< (0, 0, 1)
        static Vector3 left;             ///< (-1, 0, 0)
        static Vector3 right;            ///< (1, 0, 0)
        static Vector3 up;               ///< (0, 1, 0)
        static Vector3 zero;             ///< (0, 0, 0)
        static Vector3 one;              ///< (1, 1, 1)
        static Vector3 negativeInfinity; ///< (-∞, -∞, -∞)
        static Vector3 positiveInfinity; ///< (+∞, +∞, +∞)

        // ── Properties ───────────────────────────────────────────────────────

        /// @brief Returns the Euclidean length of the vector.
        T magnitude() const;

        /// @brief Returns a unit vector with the same direction (does not modify this).
        Vector3 normalized() const;

        /// @brief Returns the squared magnitude (cheaper than magnitude()).
        T sqrMagnitude() const;

        // ── Instance methods ─────────────────────────────────────────────────

        /**
         * @brief Checks component-wise equality with another vector.
         * @param other  Vector to compare.
         * @return true if all three components match.
         */
        bool Equals(const Vector3& other) const;

        /// @brief Normalises this vector in place.
        void Normalize();

        /**
         * @brief Sets the components of this vector.
         * @param X  New x value.
         * @param Y  New y value.
         * @param Z  New z value.
         */
        void Set(T X, T Y, T Z);

        /// @brief Returns a human-readable string, e.g. "(1.0, 2.0, 3.0)".
        std::string toString();

        // ── Operators ────────────────────────────────────────────────────────

        Vector3 operator+(const Vector3& rhs) const; ///< Component-wise addition.
        Vector3 operator-(const Vector3& rhs) const; ///< Component-wise subtraction.
        Vector3 operator*(Vector3& rhs) const;        ///< Component-wise multiplication.
        Vector3 operator*(T scalar) const;             ///< Scalar multiplication.
        Vector3 operator/(T scalar) const;             ///< Scalar division.
        const T& operator[](int index) const;          ///< Index access (0=x, 1=y, 2=z).
        bool operator/=(const Vector3& rhs) const;    ///< Compound division-equality (comparison helper).
        bool operator==(const Vector3& rhs) const;    ///< Component-wise equality.

        // ── Static utility methods ───────────────────────────────────────────

        /// @brief Returns the cross product of @p lhs and @p rhs.
        static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);

        /// @brief Returns @p normal orthonormalised relative to @p tangent.
        static Vector3 OrthoNormalize(const Vector3& normal, const Vector3& tangent);

        /// @brief Projects @p vector onto @p onNormal.
        static Vector3 Project(const Vector3& vector, const Vector3& onNormal);

        /// @brief Projects @p vector onto the plane defined by @p planeNormal.
        static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3 planeNormal);

        /**
         * @brief Rotates @p current toward @p target by at most @p maxRadiansDelta.
         * @param maxRadiansDelta  Max angular change per call (radians).
         */
        static Vector3 RotateTowards(const Vector3& current, const Vector3& target, T maxRadiansDelta);

        /// @brief Spherical linear interpolation between @p from and @p to, clamped to [0,1].
        static Vector3 Slerp(const Vector3& from, const Vector3& to, T t);

        /// @brief Spherical linear interpolation without clamping @p t.
        static Vector3 SlerpUnclamped(const Vector3& from, const Vector3& to, T t);

        /// @brief Returns the unsigned angle (radians) between @p from and @p to.
        static T Angle(const Vector3& from, const Vector3& to);

        /// @brief Returns @p v clamped so its magnitude does not exceed @p maxLength.
        static Vector3 ClampMagnitude(const Vector3& v, T maxLength);

        /// @brief Returns the Euclidean distance between @p a and @p b.
        static T Distance(const Vector3& a, const Vector3& b);

        /// @brief Returns the dot product of @p a and @p b.
        static T Dot(const Vector3& a, const Vector3& b);

        /// @brief Linear interpolation clamped to [0,1].
        static Vector3 Lerp(const Vector3& a, const Vector3& b, T t);

        /// @brief Linear interpolation without clamping @p t.
        static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, T t);

        /// @brief Component-wise maximum.
        static Vector3 Max(const Vector3& a, const Vector3& b);

        /// @brief Component-wise minimum.
        static Vector3 Min(const Vector3& a, const Vector3& b);

        /// @brief Moves @p current toward @p target by at most @p maxDelta.
        static Vector3 MoveTowards(const Vector3& current, const Vector3& target, T maxDelta);

        /// @brief Reflects @p inDirection off a surface with the given @p inNormal.
        static Vector3 Reflect(const Vector3& inDirection, const Vector3& inNormal);

        /// @brief Component-wise multiplication: (a.x*b.x, a.y*b.y, a.z*b.z).
        static Vector3 Scale(const Vector3& a, const Vector3& b);

        /**
         * @brief Signed angle (radians) from @p from to @p to, around @p axis.
         */
        static T SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);
    };
} // namespace MathsLib

#include "Vector3.inl"
