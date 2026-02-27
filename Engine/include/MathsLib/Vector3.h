#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>

#include <numbers>
namespace MathsLib
{
	template <typename T>
	class Vector3
	{
	public:
		T x, y, z;
		Vector3<T>() = default;
		Vector3<T>(T x = 0.0f, T y = 0.0f, T z = 0.0f);

		static Vector3 back;
		static Vector3 down;
		static Vector3 forward;
		static Vector3 left;
		static Vector3 right;
		static Vector3 up;
		static Vector3 zero;
		static Vector3 one;
		static Vector3 negativeInfinity;
		static Vector3 positiveInfinity;

		T magnitude() const;
		Vector3 normalized() const;
		T sqrMagnitude() const;
		bool Equals(const Vector3&) const;

		void Normalize();

		void Set(T X, T Y,T Z);

		std::string toString();
		Vector3 operator+(const Vector3&) const;
		Vector3 operator-(const Vector3&) const;
		Vector3 operator*(Vector3&) const;
		Vector3 operator*(T) const;
		Vector3 operator/(T) const;
		const T& operator[](int) const;
		bool operator/=(const Vector3&) const;
		bool operator==(const Vector3&) const;


		
		static Vector3 Cross(const Vector3&, const Vector3&) ;
		static Vector3 OrthoNormalize(const Vector3&, const Vector3&);
		static Vector3 Project(const Vector3&,const Vector3&);
		static Vector3 ProjectOnPlane(const Vector3&, const Vector3);
		static Vector3 RotateTowards(const Vector3&, const Vector3&,T);
		static Vector3 Slerp(const Vector3&, const Vector3&, T);
		static Vector3 SlerpUnclamped(const Vector3& from, const Vector3& to, T t);

		static T Angle(const Vector3&, const Vector3&);
		static Vector3 ClampMagnitude(const Vector3&, T);
		static T Distance(const Vector3&, const Vector3&);
		static T Dot(const Vector3&, const Vector3&);
		static Vector3 Lerp(const Vector3&, const Vector3&, T);
		static Vector3 LerpUnclamped(const Vector3&, const Vector3&, T);
		static Vector3 Max(const Vector3&, const Vector3&);
		static Vector3 Min(const Vector3&, const Vector3&);
		static Vector3 MoveTowards(const Vector3&, const Vector3&, T);
		static Vector3 Reflect(const Vector3&, const Vector3&);
		static Vector3 Scale(const Vector3&, const Vector3&);
		static T SignedAngle(const Vector3&, const Vector3&, const Vector3&);

	};
}

#include "Vector3.inl"
