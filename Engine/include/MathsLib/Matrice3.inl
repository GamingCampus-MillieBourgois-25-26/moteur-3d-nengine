#include "Matrice3.h"
#pragma once

namespace MathsLib {
	template<typename T>
	Matrice3<T>::Matrice3(T a, T b, T c,
		T d, T e, T f,
		T g, T h, T i)
	{
		m[0][0] = a; m[1][0] = b; m[2][0] = c;
		m[0][1] = d; m[1][1] = e; m[2][1] = f;
		m[0][2] = g; m[1][2] = h; m[2][2] = i;
	}

#pragma region Operators
	template<typename T>
	inline Matrice3<T> Matrice3<T>::operator*(Matrice3&)
	{

	}

	template<typename T>
	inline Matrice3<T> Matrice3<T>::operator+(Matrice3& m)
	{

	}

	template<typename T>
	inline Matrice3<T> Matrice3<T>::operator-(Matrice3& m)
	{
		return Matrice3<T>();
	}

	template<typename T>
	inline Matrice3<T> Matrice3<T>::operator+(T scalar)
	{
		Matrice3<T> newM;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				newM[i][j] = m[i][j] += scalar;
			}
		}
		return newM;
	}

	template<typename T>
	inline Matrice3<T> Matrice3<T>::operator-(T scalar)
	{
		return Matrice3<T>();
	}

	template<typename T>
	inline bool Matrice3<T>::operator==(const Matrice3<T>& m3)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (m[i][j] != m3.m[i][j])
					return false;
			}
		}
		return true;
	}

#pragma endregion


}