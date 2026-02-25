#pragma once
#include <stdexcept>
#include <string>
#include "Quaternion.h"
#include "Vector3.h"

namespace MathsLib
{
	template <typename T>
	class Matrice4
	{
	public:
		T m4[4][4];

		//Matrice4<T>() = default;
		Matrice4<T>(T a = 0, T b = 0, T c = 0, T d = 0, T e = 0, T f = 0, T g = 0, T h = 0, T i = 0, T j = 0, T k = 0, T l = 0, T m = 0, T n = 0, T o = 0, T p = 0);

#pragma region Static properties

		static Matrice4 identity;
		static Matrice4 zero;

#pragma endregion

#pragma region Properties

		T determinant() const;
		Matrice4 inverse() const;
		bool isIdentity(T epsilon = static_cast<T>(1e-6)) const;
		Quaternion<T> rotation() const;
		Matrice4 transpose() const;

#pragma endregion
#pragma region StaticMethods

		Matrice4 ortho(T, T, T, T, T, T);
		Matrice4 perspective(T, T, T, T);
		Matrice4 rotate(Quaternion<T>& q);
		Matrice4 scale(Vector3<T>& vector);
		Matrice4 translate(Vector3<T>& vector);
		Matrice4 trs(Vector3<T>& pos, Quaternion<T>& q, Vector3<T>& s);
#pragma endregion

#pragma region Operators

		Matrice4 operator*(const Matrice4& m4);
		T* operator[](int row) { return m4[row]; }
		const T* operator[](int row) const { return m4[row]; }
#pragma endregion

#pragma region Public Methods

		T GetColumn(int index);
		Vector3<T> GetPosition();
		T GetRow(int index);
		Vector3<T> MultiplyPoint(Vector3<T> position);
		Vector3<T> MultiplyPoint3x4(Vector3<T> position);
		Vector3<T> MultiplyPointVector(Vector3<T> direction);
		void SetColumn(int index, T a, T b, T c, T d);
		void SetRow(int index, T a, T b, T c, T d);
		std::string ToString();

#pragma endregion

	};
}
#include "Matrice4.inl"
