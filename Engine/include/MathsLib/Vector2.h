#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <numbers>

namespace MathsLib
{
	template <typename T>
	class Vector2
	{
	public:
		Vector2<T>() = default;
		Vector2<T>(T X = 0.0f, T Y = 0.0f);

#pragma region Static Properties

		static Vector2 down;
		static Vector2 left;
		static Vector2 right;
		static Vector2 up;
		static Vector2 zero;
		static Vector2 one;
		static Vector2 negativeInfinity;
		static Vector2 positiveInfinity;

#pragma endregion

#pragma region Properties

		T x, y;

		T magnitude() const;
		Vector2<T> normalized() const;
		T sqrMagnitude() const;

#pragma endregion


#pragma region Public Methods

		bool Equals(const Vector2&) const;

		void Normalize();

		void Set(T X, T Y);

		std::string toString();

#pragma endregion

#pragma region Static Methods

		static T Angle(const Vector2&, const Vector2&);
		static Vector2 ClampMagnitude(const Vector2&, T);
		static T Distance(const Vector2&, const Vector2&);
		static T Dot(const Vector2&, const Vector2&);
		static Vector2 Lerp(const Vector2&, const Vector2&, T);
		static Vector2 LerpUnclamped(const Vector2&, const Vector2&, T);
		static Vector2 Max(const Vector2&, const Vector2&);
		static Vector2 Min(const Vector2&, const Vector2&);
		static Vector2 MoveTowards(const Vector2&, const Vector2&, T);
		static Vector2 Perpandicular(const Vector2&);
		static Vector2 Reflect(const Vector2&, const Vector2&);
		static Vector2 Scale(const Vector2&, const Vector2&);
		static T SignedAngle(const Vector2&, const Vector2&);

#pragma endregion 

#pragma region Operators
		template<typename TcT>
		operator TcT() { return TcT{ x, y }; }

		Vector2 operator+(const Vector2&) const;
		Vector2 operator-(const Vector2&) const;
		Vector2 operator*(T) const;
		Vector2 operator/(T) const;
		bool operator/=(const Vector2&) const;
		bool operator==(const Vector2&) const;

#pragma endregion

	};

}
#include "Vector2.inl"

