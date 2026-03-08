#pragma once
/**
 * @file Vector2.h
 * @brief 2D vector template class.
 * @ingroup MathsLib
 */

#include <iostream>
#include <string>
#include <cmath>
#include <numbers>

namespace MathsLib
{
    /**
     * @brief Generic 2D vector with common math operations.
     *
     * @tparam T Scalar type (e.g. float, double, int).
     *
     * Provides arithmetic operators, static utility methods (Dot, Lerp, Distance…)
     * and direction constants (up, down, left, right, zero, one).
     */
    template <typename T>
    class Vector2
    {
    public:
        // ── Constructors ────────────────────────────────────────────────────

        /// @brief Default constructor – initialises to (0, 0).
        Vector2<T>() = default;

        /**
         * @brief Constructs a Vector2 with explicit coordinates.
         * @param X  Horizontal component.
         * @param Y  Vertical component.
         */
        Vector2<T>(T X = 0.0f, T Y = 0.0f);

        // ── Static direction constants ───────────────────────────────────────

        static Vector2 down;             ///< (0, -1)
        static Vector2 left;             ///< (-1, 0)
        static Vector2 right;            ///< (1, 0)
        static Vector2 up;               ///< (0, 1)
        static Vector2 zero;             ///< (0, 0)
        static Vector2 one;              ///< (1, 1)
        static Vector2 negativeInfinity; ///< (-∞, -∞)
        static Vector2 positiveInfinity; ///< (+∞, +∞)

        // ── Components ───────────────────────────────────────────────────────

        T x; ///< Horizontal component.
        T y; ///< Vertical component.

        // ── Properties ───────────────────────────────────────────────────────

        /// @brief Returns the Euclidean length of the vector.
        T magnitude() const;

        /// @brief Returns a unit vector with the same direction (does not modify this).
        Vector2<T> normalized() const;

        /// @brief Returns the squared magnitude (cheaper than magnitude()).
        T sqrMagnitude() const;

        // ── Instance methods ─────────────────────────────────────────────────

        /**
         * @brief Checks component-wise equality with another vector.
         * @param other  Vector to compare.
         * @return true if both components match.
         */
        bool Equals(const Vector2& other) const;

        /// @brief Normalises this vector in place (makes it unit length).
        void Normalize();

        /**
         * @brief Sets the components of this vector.
         * @param X  New x value.
         * @param Y  New y value.
         */
        void Set(T X, T Y);

        /// @brief Returns a human-readable string representation, e.g. "(1.0, 2.0)".
        std::string toString();

        // ── Static utility methods ───────────────────────────────────────────

        /// @brief Returns the unsigned angle (radians) between two vectors.
        static T Angle(const Vector2& a, const Vector2& b);

        /// @brief Returns @p v clamped so its magnitude does not exceed @p maxLength.
        static Vector2 ClampMagnitude(const Vector2& v, T maxLength);

        /// @brief Returns the Euclidean distance between @p a and @p b.
        static T Distance(const Vector2& a, const Vector2& b);

        /// @brief Returns the dot product of @p a and @p b.
        static T Dot(const Vector2& a, const Vector2& b);

        /**
         * @brief Linearly interpolates between @p a and @p b, clamped to [0,1].
         * @param t  Blend factor in [0,1].
         */
        static Vector2 Lerp(const Vector2& a, const Vector2& b, T t);

        /// @brief Linearly interpolates between @p a and @p b without clamping @p t.
        static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, T t);

        /// @brief Returns a vector whose components are the component-wise maximum.
        static Vector2 Max(const Vector2& a, const Vector2& b);

        /// @brief Returns a vector whose components are the component-wise minimum.
        static Vector2 Min(const Vector2& a, const Vector2& b);

        /**
         * @brief Moves @p current toward @p target by at most @p maxDelta.
         * @param maxDelta  Maximum movement per call (must be positive).
         */
        static Vector2 MoveTowards(const Vector2& current, const Vector2& target, T maxDelta);

        /// @brief Returns a vector perpendicular to @p v (rotated 90° CCW).
        static Vector2 Perpandicular(const Vector2& v);

        /// @brief Reflects @p inDirection off a surface with the given @p inNormal.
        static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal);

        /**
         * @brief Component-wise multiplication: (a.x*b.x, a.y*b.y).
         */
        static Vector2 Scale(const Vector2& a, const Vector2& b);

        /// @brief Returns the signed angle (radians) from @p from to @p to.
        static T SignedAngle(const Vector2& from, const Vector2& to);

        // ── Operators ────────────────────────────────────────────────────────

        /// @brief Implicit conversion to another vector type @p TcT.
        template<typename TcT>
        operator TcT() { return TcT{ x, y }; }

        /// @brief Component-wise addition.
        Vector2 operator+(const Vector2& rhs) const;
        /// @brief Component-wise subtraction.
        Vector2 operator-(const Vector2& rhs) const;
        /// @brief Scalar multiplication.
        Vector2 operator*(T scalar) const;
        /// @brief Scalar division.
        Vector2 operator/(T scalar) const;
        /// @brief Compound division-equality (returns bool; used for comparison helpers).
        bool operator/=(const Vector2& rhs) const;
        /// @brief Component-wise equality comparison.
        bool operator==(const Vector2& rhs) const;
    };
} // namespace MathsLib

#include "Vector2.inl"
