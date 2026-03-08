#pragma once
/**
 * @file Matrice4.h
 * @brief 4x4 matrix template class used for 3D transformations.
 * @ingroup MathsLib
 */

#include <stdexcept>
#include <string>
#include "Quaternion.h"
#include "Vector3.h"

namespace MathsLib
{
    /**
     * @brief A 4×4 homogeneous transformation matrix.
     *
     * @tparam T Scalar type (e.g. float, double).
     *
     * Supports projection matrices (perspective, orthographic), TRS composition,
     * inversion, transposition and quaternion extraction.
     */
    template <typename T>
    class Matrice4
    {
    public:
        T m4[4][4]; ///< Raw 4×4 storage, accessed as m4[row][col].

        /**
         * @brief Constructs a matrix with 16 explicit element values (row-major).
         */
        Matrice4<T>(T a=0,T b=0,T c=0,T d=0,
                    T e=0,T f=0,T g=0,T h=0,
                    T i=0,T j=0,T k=0,T l=0,
                    T m=0,T n=0,T o=0,T p=0);

        // ── Static constants ─────────────────────────────────────────────────

        static Matrice4 identity; ///< Identity matrix (diagonal = 1, rest = 0).
        static Matrice4 zero;     ///< All-zero matrix.

        // ── Properties ───────────────────────────────────────────────────────

        /// @brief Computes the determinant of this matrix.
        T determinant() const;

        /// @brief Returns the inverse of this matrix.
        /// @throws std::runtime_error if the matrix is singular.
        Matrice4 inverse() const;

        /**
         * @brief Returns true if the matrix is approximately the identity.
         * @param epsilon  Tolerance for floating-point comparison.
         */
        bool isIdentity(T epsilon = static_cast<T>(1e-6)) const;

        /// @brief Extracts the rotation as a Quaternion (assumes orthonormal upper-left 3×3).
        Quaternion<T> rotation() const;

        /// @brief Returns the transpose of this matrix.
        Matrice4 transpose() const;

        // ── Static factory methods ───────────────────────────────────────────

        /**
         * @brief Creates an orthographic projection matrix.
         * @param left, right  Horizontal clipping planes.
         * @param bottom, top  Vertical clipping planes.
         * @param near, far    Depth clipping planes.
         */
        Matrice4 ortho(T left, T right, T bottom, T top, T near, T far);

        /**
         * @brief Creates a perspective projection matrix.
         * @param fovY    Vertical field of view (radians).
         * @param aspect  Width / height aspect ratio.
         * @param near    Near clipping distance.
         * @param far     Far clipping distance.
         */
        Matrice4 perspective(T fovY, T aspect, T near, T far);

        /**
         * @brief Creates a rotation matrix from a quaternion @p q.
         */
        Matrice4 rotate(Quaternion<T>& q);

        /**
         * @brief Creates a scale matrix from a @p vector.
         */
        Matrice4 scale(Vector3<T>& vector);

        /**
         * @brief Creates a translation matrix from a @p vector.
         */
        Matrice4 translate(Vector3<T>& vector);

        /**
         * @brief Combines translation, rotation and scale into a single TRS matrix.
         * @param pos  Translation vector.
         * @param q    Rotation quaternion.
         * @param s    Scale vector.
         */
        Matrice4 trs(Vector3<T>& pos, Quaternion<T>& q, Vector3<T>& s);

        // ── Operators ────────────────────────────────────────────────────────

        /// @brief Matrix multiplication.
        Matrice4 operator*(const Matrice4& rhs);

        /// @brief Row access (returns pointer to the row array).
        T* operator[](int row) { return m4[row]; }

        /// @brief Const row access.
        const T* operator[](int row) const { return m4[row]; }

        // ── Public methods ───────────────────────────────────────────────────

        /**
         * @brief Returns the element at column @p index of the first row (legacy helper).
         */
        T GetColumn(int index);

        /// @brief Extracts the translation (position) from this TRS matrix.
        Vector3<T> GetPosition();

        /**
         * @brief Returns the element at row @p index of the first column (legacy helper).
         */
        T GetRow(int index);

        /**
         * @brief Transforms a 3D point using the full 4×4 matrix (w-divide applied).
         */
        Vector3<T> MultiplyPoint(Vector3<T> position);

        /**
         * @brief Transforms a 3D point using only the upper 3×4 portion (no w-divide).
         * Slightly faster than MultiplyPoint when the last row is (0,0,0,1).
         */
        Vector3<T> MultiplyPoint3x4(Vector3<T> position);

        /**
         * @brief Transforms a direction vector (ignores translation).
         */
        Vector3<T> MultiplyPointVector(Vector3<T> direction);

        /**
         * @brief Sets column @p index with values (a, b, c, d).
         */
        void SetColumn(int index, T a, T b, T c, T d);

        /**
         * @brief Sets row @p index with values (a, b, c, d).
         */
        void SetRow(int index, T a, T b, T c, T d);

        /// @brief Returns a human-readable string representation.
        std::string ToString();
    };
} // namespace MathsLib

#include "Matrice4.inl"
