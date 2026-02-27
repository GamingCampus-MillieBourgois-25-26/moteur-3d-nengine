#include "Matrice4.h"
#pragma once

namespace MathsLib
{
    template <typename T>
    Matrice4<T>::Matrice4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
    {
        m4[0][0] = a; m4[0][1] = b; m4[0][2] = c; m4[0][3] = d;
        m4[1][0] = e; m4[1][1] = f; m4[1][2] = g; m4[1][3] = h;
        m4[2][0] = i; m4[2][1] = j; m4[2][2] = k; m4[2][3] = l;
        m4[3][0] = m; m4[3][1] = n; m4[3][2] = o; m4[3][3] = p;
    }
#pragma region Static Properties


    template<typename T> Matrice4<T> Matrice4<T>::identity = Matrice4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    template<typename T> Matrice4<T> Matrice4<T>::zero = Matrice4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);


#pragma endregion

#pragma region Properties

    template <typename T>
    T Matrice4<T>::determinant() const
    {
        const T* m = &this->m4[0][0];
        return
            m[0] * m[5] * m[10] * m[15] + m[0] * m[9] * m[14] * m[7] + m[0] * m[13] * m[6] * m[11]
            - m[0] * m[13] * m[10] * m[7] - m[0] * m[9] * m[6] * m[15] - m[0] * m[5] * m[14] * m[11]
            - m[4] * m[1] * m[10] * m[15] - m[8] * m[1] * m[14] * m[7] - m[12] * m[1] * m[6] * m[11]
            + m[12] * m[1] * m[10] * m[7] + m[8] * m[1] * m[6] * m[15] + m[4] * m[1] * m[14] * m[11]
            + m[4] * m[9] * m[2] * m[15] + m[8] * m[13] * m[2] * m[7] + m[12] * m[5] * m[2] * m[11]
            - m[12] * m[9] * m[2] * m[7] - m[8] * m[5] * m[2] * m[15] - m[4] * m[13] * m[2] * m[11]
            - m[4] * m[9] * m[14] * m[3] - m[8] * m[13] * m[6] * m[3] - m[12] * m[5] * m[10] * m[3]
            + m[12] * m[9] * m[6] * m[3] + m[8] * m[5] * m[14] * m[3] + m[4] * m[13] * m[10] * m[3];
    }

    template <typename T>
    Matrice4<T> Matrice4<T>::inverse() const
    {
        Matrice4<T> inv;
        const T* m = &this->m4[0][0];
        T invOut[16];

        invOut[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        invOut[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        invOut[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        invOut[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        invOut[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        invOut[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        invOut[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        invOut[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        invOut[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        invOut[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        invOut[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        invOut[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        invOut[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        invOut[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        invOut[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        invOut[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        T det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];

        if (det == static_cast<T>(0))
            throw std::runtime_error("Matrix can't be inverse det = 0");

        det = static_cast<T>(1) / det;

        for (int i = 0; i < 16; i++)
            (&inv.m4[0][0])[i] = invOut[i] * det;

        return inv;
    }

    template <typename T>
    bool Matrice4<T>::isIdentity(T epsilon) const
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (i == j)
                {
                    if (std::abs(m4[i][j] - static_cast<T>(1)) > epsilon)
                        return false;
                }
                else
                {
                    if (std::abs(m4[i][j]) > epsilon)
                        return false;
                }
            }
        }
        return true;
    }

    template <typename T>
    Quaternion<T> Matrice4<T>::rotation() const
    {
        Quaternion q;
        T trace = m4[0][0] + m4[1][1] + m4[2][2];

        if (trace > 0)
        {
            T s = std::sqrt(trace + static_cast<T>(1)) * static_cast<T>(2);
            q.w = static_cast<T>(0.25) * s;
            q.x = (m4[2][1] - m4[1][2]) / s;
            q.y = (m4[0][2] - m4[2][0]) / s;
            q.z = (m4[1][0] - m4[0][1]) / s;
        }
        else if ((m4[0][0] > m4[1][1]) && (m4[0][0] > m4[2][2]))
        {
            T s = std::sqrt(static_cast<T>(1) + m4[0][0] - m4[1][1] - m4[2][2]) * static_cast<T>(2);
            q.w = (m4[2][1] - m4[1][2]) / s;
            q.x = static_cast<T>(0.25) * s;
            q.y = (m4[0][1] + m4[1][0]) / s;
            q.z = (m4[0][2] + m4[2][0]) / s;
        }
        else if (m4[1][1] > m4[2][2])
        {
            T s = std::sqrt(static_cast<T>(1) + m4[1][1] - m4[0][0] - m4[2][2]) * static_cast<T>(2);
            q.w = (m4[0][2] - m4[2][0]) / s;
            q.x = (m4[0][1] + m4[1][0]) / s;
            q.y = static_cast<T>(0.25) * s;
            q.z = (m4[1][2] + m4[2][1]) / s;
        }
        else
        {
            T s = std::sqrt(static_cast<T>(1) + m4[2][2] - m4[0][0] - m4[1][1]) * static_cast<T>(2);
            q.w = (m4[1][0] - m4[0][1]) / s;
            q.x = (m4[0][2] + m4[2][0]) / s;
            q.y = (m4[1][2] + m4[2][1]) / s;
            q.z = static_cast<T>(0.25) * s;
        }

        return q;
    }

    template <typename T>
    Matrice4<T> Matrice4<T>::transpose() const
    {
        Matrice4 mT;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                mT[i][j] = m4[j][i];
        return mT;
    }
#pragma endregion

#pragma region StaticMethods    

    template<typename T>
    inline Matrice4<T> Matrice4<T>::ortho(T left, T right, T bottom, T top, T zNear, T zFar)
    {
        Matrice4 mO;
        mO[0][0] = 2 / (right - left); mO[0][1] = 0; mO[0][2] = 0; mO[0][3] = -(right + left) / (right - left);
        mO[1][0] = 0; mO[1][1] = 2 / (top - bottom); mO[1][2] = 0; mO[1][3] = -(top + bottom) / (top - bottom);
        mO[2][0] = 0; mO[2][1] = 0; mO[2][2] = -2 / (zFar - zNear); mO[2][3] = -(zFar + zNear) / (zFar - zNear);
        mO[3][0] = 0; mO[3][1] = 0; mO[3][2] = 0; mO[3][3] = 1;
        return mO;
    }
    template<typename T>
    inline Matrice4<T> Matrice4<T>::perspective(T fov, T aspect, T zNear, T zFar)
    {
        T S = 1 / std::tan(fov * static_cast<T>(0.5));
        Matrice4 mP;
        mP[0][0] = S / aspect; mP[0][1] = 0; mP[0][2] = 0; mP[0][3] = 0;
        mP[1][0] = 0; mP[1][1] = S; mP[1][2] = 0; mP[1][3] = 0;
        mP[2][0] = 0; mP[2][1] = 0; mP[2][2] = -(zFar + zNear) / (zFar - zNear); mP[2][3] = -(2 * zFar * zNear) / (zFar - zNear);
        mP[3][0] = 0; mP[3][1] = 0; mP[3][2] = -1; mP[3][3] = 0;
        return mP;
    }
    template<typename T>
    inline Matrice4<T> Matrice4<T>::rotate(Quaternion<T>& q)
    {
        Matrice4 mR;
        mR[0][0] = 1 - 2 * std::pow(q.y, 2) - 2 * std::pow(q.z, 2); mR[0][1] = 2 * q.x * q.y - 2 * q.w * q.z; mR[0][2] = 2 * q.x * q.z + 2 * q.w * q.y; mR[0][3] = 0;
        mR[1][0] = 2 * q.x * q.y + 2 * q.w * q.z; mR[1][1] = 1 - 2 * std::pow(q.x, 2) - 2 * std::pow(q.z, 2); mR[1][2] = 2 * q.y * q.z - 2 * q.w * q.x; mR[1][3] = 0;
        mR[2][0] = 2 * q.x * q.z - 2 * q.w * q.y; mR[2][1] = 2 * q.y * q.z + 2 * q.w * q.x; mR[2][2] = 1 - 2 * std::pow(q.x, 2) - 2 * std::pow(q.y, 2); mR[2][3] = 0;
        mR[3][0] = 0; mR[3][1] = 0; mR[3][2] = 0; mR[3][3] = 1;
        return mR;
        /*        Matrice4 mRx, mRy, mRz;
        mRx[0][0] = 1; mRx[0][1] = 0; mRx[0][2] = 0; mRx[0][3] = 0;
        mRx[1][0] = 0; mRx[1][1] = 0; mRx[1][2] = 0; mRx[1][3] = 0;
        mRx[2][0] = 0; mRx[2][1] = 0; mRx[2][2] = ...; mRx[2][3] = 0;
        mRx[3][0] = 0; mRx[3][1] = 0; mRx[3][2] = ...; mRx[3][3] = 0;

        return mRx * mRy * mRz;*/
    }
    template<typename T>
    inline Matrice4<T> Matrice4<T>::scale(Vector3<T>& vector)
    {
        Matrice4 mS;
        mS[0][0] = vector.x; mS[0][1] = 0; mS[0][2] = 0; mS[0][3] = 0;
        mS[1][0] = 0; mS[1][1] = vector.y; mS[1][2] = 0; mS[1][3] = 0;
        mS[2][0] = 0; mS[2][1] = 0; mS[2][2] = vector.z; mS[2][3] = 0;
        mS[3][0] = 0; mS[3][1] = 0; mS[3][2] = 0; mS[3][3] = 1;
        return mS;
    }
    template<typename T>
    inline Matrice4<T> Matrice4<T>::translate(Vector3<T>& vector)
    {
        Matrice4 mT;
        mT[0][0] = 1; mT[0][1] = 0; mT[0][2] = 0; mT[0][3] = 0;
        mT[1][0] = 0; mT[1][1] = 1; mT[1][2] = 0; mT[1][3] = 0;
        mT[2][0] = 0; mT[2][1] = 0; mT[2][2] = 1; mT[2][3] = 0;
        mT[3][0] = vector.x; mT[3][1] = vector.y; mT[3][2] = vector.z; mT[3][3] = 1;

        return mT;
    }
    template<typename T>
    inline Matrice4<T> Matrice4<T>::trs(Vector3<T>& pos, Quaternion<T>& q, Vector3<T>& s)
    {
        return scale(s) * rotate(q) * translate(pos);
    }

#pragma endregion

#pragma region Operators
    template <typename T>
    Matrice4<T> Matrice4<T>::operator*(const Matrice4& m) {
        Matrice4<T> result;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result[i][j] = m4[i][0] * m[0][j]
                    + m4[i][1] * m[1][j]
                    + m4[i][2] * m[2][j]
                    + m4[i][3] * m[3][j];
            }
        }
        return result;
    }

#pragma endregion

#pragma region Public Methods

    template <typename T>
    T Matrice4<T>::GetColumn(int i)
    {
        T column[1][4] = { m4[0][i], m4[1][i], m4[2][i], m4[3][i] };
        return column;
    }

    template <typename T>
    Vector3<T> Matrice4<T>::GetPosition()
    {
        return Vector3<T>(m4[3][0], m4[3][1], m4[3][2]);
    }

    template <typename T>
    T Matrice4<T>::GetRow(int i)
    {
        T row[1][4] = { m4[i][0], m4[i][1], m4[i][2], m4[i][3] };
        return row;
    }

    template <typename T>
    Vector3<T> Matrice4<T>::MultiplyPoint(Vector3<T> position)
    {
        T x = m4[0][0] * position.x + m4[0][1] * position.y + m4[0][2] * position.z + m4[0][3];
        T y = m4[1][0] * position.x + m4[1][1] * position.y + m4[1][2] * position.z + m4[1][3];
        T z = m4[2][0] * position.x + m4[2][1] * position.y + m4[2][2] * position.z + m4[2][3];
        T w = m4[3][0] * position.x + m4[3][1] * position.y + m4[3][2] * position.z + m4[3][3];

        if (w != static_cast<T>(0) && w != static_cast<T>(1))
        {
            T invW = static_cast<T>(1) / w;
            x *= invW;
            y *= invW;
            z *= invW;
        }

        return Vector3<T>(x, y, z);
    }

    template <typename T>
    Vector3<T> Matrice4<T>::MultiplyPoint3x4(Vector3<T> position)
    {
        T x = m4[0][0] * position.x + m4[0][1] * position.y + m4[0][2] * position.z + m4[0][3];
        T y = m4[1][0] * position.x + m4[1][1] * position.y + m4[1][2] * position.z + m4[1][3];
        T z = m4[2][0] * position.x + m4[2][1] * position.y + m4[2][2] * position.z + m4[2][3];

        return Vector3<T>(x, y, z);
    }

    template <typename T>
    Vector3<T> Matrice4<T>::MultiplyPointVector(Vector3<T> direction)
    {
        T x = m4[0][0] * direction.x + m4[0][1] * direction.y + m4[0][2] * direction.z;
        T y = m4[1][0] * direction.x + m4[1][1] * direction.y + m4[1][2] * direction.z;
        T z = m4[2][0] * direction.x + m4[2][1] * direction.y + m4[2][2] * direction.z;

        return Vector3<T>(x, y, z);
    }

    template <typename T>
    void Matrice4<T>::SetColumn(int i, T a, T b, T c, T d)
    {
        m4[0][i] = a;
        m4[1][i] = b;
        m4[2][i] = c;
        m4[3][i] = d;
    }

    template <typename T>
    void Matrice4<T>::SetRow(int i, T a, T b, T c, T d)
    {
        m4[i][0] = a;
        m4[i][1] = b;
        m4[i][2] = c;
        m4[i][3] = d;
    }

    template <typename T>
    std::string Matrice4<T>::ToString()
    {
        std::string matriceInString =
            std::to_string(m4[0][0]) + "," + std::to_string(m4[0][1]) + "," + std::to_string(m4[0][2]) + "," + std::to_string(m4[0][3]) + "\n" +
            std::to_string(m4[1][0]) + "," + std::to_string(m4[1][1]) + "," + std::to_string(m4[1][2]) + "," + std::to_string(m4[1][3]) + "\n" +
            std::to_string(m4[2][0]) + "," + std::to_string(m4[2][1]) + "," + std::to_string(m4[2][2]) + "," + std::to_string(m4[2][3]) + "\n" +
            std::to_string(m4[3][0]) + "," + std::to_string(m4[3][1]) + "," + std::to_string(m4[3][2]) + "," + std::to_string(m4[3][3]) + "\n";
        return matriceInString;
    }
#pragma endregion



}