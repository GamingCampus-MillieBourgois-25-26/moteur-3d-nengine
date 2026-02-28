#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <numbers>
#include"Vector3.h"

/*

w
x = i
y = j
z = k

j*i = -k


j*k = i


i*k = -j


i*k*j = i*i = j*j = k*k

*/
namespace MathsLib
{
	template<typename T>
	class Quaternion {
	public:
		T w, x, y, z;
		//Quaternion<T>() = default;
		//Quaternion<T>(T w = 1.0f, T x = 0.0f, T y = 0.0f, T z = 0.0f);

		Quaternion();
		Quaternion(T w, T x, T y, T z);

		Quaternion normalized() const;
		T magnitude() const;
		void Normalize();


		static Quaternion identity;

		bool Equals(const Quaternion&) const;
		void Set(T w, T x, T y, T z);
		void SetFromToRotation(const Vector3<T>& from, const Vector3<T>& to);
		void SetLookRotation(const Vector3<T>& forward, const Vector3<T>& up = Vector3<T>::up);
		void ToAngleAxis(T& angle, Vector3<T>& axis) const;

		Quaternion<T> AngleAxis(float degrees, Vector3<T>& axis);

		std::string toString() const;


		static T Angle(const Quaternion<T>& a, const Quaternion<T>& b);
		static Quaternion<T> AngleAxis(T angle, const Vector3<T>& axis);
		static T Dot(const Quaternion<T>& a, const Quaternion<T>& b);
		static Quaternion<T> Euler(T x, T y, T z);
		static Quaternion<T> FromToRotation(const Vector3<T>& from, const Vector3<T>& to);
		static Quaternion<T> Inverse(const Quaternion<T>& q);
		static Quaternion<T> Lerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);
		static Quaternion<T> LerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);
		static Quaternion<T> LookRotation(const Vector3<T>& forward, const Vector3<T>& upwards = Vector3<T>::up());
		static Quaternion<T> RotateTowards(const Quaternion<T>& from, const Quaternion<T>& to, T maxDegreesDelta);
		static Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);
		static Quaternion<T> SlerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t);



		Quaternion operator*(const Quaternion&) const;
		Quaternion operator*(T) const;
		Quaternion operator+(const Quaternion&) const;
		Quaternion operator-(const Quaternion&) const;

		bool operator==(const Quaternion&) const;
		const T& operator[](int) const;


	};


}

#include "Quaternion.inl"