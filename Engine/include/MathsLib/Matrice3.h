#pragma once
namespace MathsLib {
	template<typename T>
	class Matrice3
	{
	public:
		T m[3][3];

		Matrice3<T>() = default;
		Matrice3<T>(
			T a = 0.0f, T b = 0.0f, T c = 0.0f,
			T d = 0.0f, T e = 0.0f, T f = 0.0f,
			T g = 0.0f, T h = 0.0f, T i = 0.0f
		);

		Matrice3<T> operator*(Matrice3<T>&);
		Matrice3<T> operator+(Matrice3<T>&);
		Matrice3<T> operator-(Matrice3<T>&);
		Matrice3<T> operator+(T);
		Matrice3<T> operator-(T);
		bool operator==(const Matrice3<T>&);

	};

}
#include "Matrice3.inl"