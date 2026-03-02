//#include "Quaternion.h"
//#pragma once

namespace MathsLib
{
	template <typename T>
	Quaternion<T>::Quaternion() 
		: w(1), x(0), y(0), z(0) {}

	template<typename T>
	Quaternion<T>::Quaternion(T w, T x, T y, T z) 
		: w(w), x(x), y(y), z(z) {}
	//Quaternion<T>::Quaternion(T w, T x, T y, T z) 
	//	: w(w), x(x), y(y), z(z) {}

#pragma region StaticProperties

	template<typename T> Quaternion<T> Quaternion<T>::identity = Quaternion<T>(1, 0, 0, 0);

#pragma endregion

#pragma region Properties


	template <typename T>
	Quaternion<T> Quaternion<T>::normalized() const
	{
		T m = magnitude();
		return (m > T(0)) ? (*this) * (T(1) / m) : identity;
	}

	template<typename T>
	inline T Quaternion<T>::magnitude() const
	{
		return std::sqrt(w * w + x * x + y * y + z * z);
	}

	template<typename T>
	inline void Quaternion<T>::Normalize()
	{
		T mag = magnitude();
		if (mag > T(0)) {
			T invMag = T(1) / mag;
			w *= invMag;
			x *= invMag;
			y *= invMag;
			z *= invMag;
		}
		else {
			w = 1; x = 0; y = 0; z = 0;
		}
	}

	template<typename T>
	inline bool Quaternion<T>::Equals(const Quaternion& q) const
	{
		return (x == q.x && y == q.y && z == q.z && w == q.w);
	}

	template<typename T>
	inline void Quaternion<T>::Set(T w, T x, T y, T z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	inline void Quaternion<T>::SetFromToRotation(const Vector3<T>& from, const Vector3<T>& to)
	{
		Vector3<T> f = from.normalized();
		Vector3<T> t = to.normalized();

		T dot = Vector3<T>::Dot(f, t);
		if (dot >= 1.0f) {
			Set(1, 0, 0, 0);
			return;
		}
		if (dot <= -1.0f) {
			Vector3<T> ortho = Vector3<T>::OrthoNormalize(f, Vector3<T>(1, 0, 0));
			if (ortho.sqrMagnitude() < 1e-6) {
				ortho = Vector3<T>::OrthoNormalize(f, Vector3<T>(0, 1, 0));
			}
			Set(0, ortho.x, ortho.y, ortho.z);
			return;
		}

		Vector3<T> cross = Vector3<T>::Cross(f, t);
		T s = std::sqrt((1 + dot) * 2);
		T invs = 1 / s;

		Set(s * 0.5f, cross.x * invs * 0.5f, cross.y * invs * 0.5f, cross.z * invs * 0.5f);
		Normalize();
	}

	template<typename T>
	inline void Quaternion<T>::SetLookRotation(const Vector3<T>& forward, const Vector3<T>& up)
	{
		Vector3<T> f = forward.normalized();
		Vector3<T> u = up.normalized();
		Vector3<T> r = Vector3<T>::Cross(u, f).normalized();
		u = Vector3<T>::Cross(f, r);

		T m00 = r.x, m01 = r.y, m02 = r.z;
		T m10 = u.x, m11 = u.y, m12 = u.z;
		T m20 = -f.x, m21 = -f.y, m22 = -f.z;

		T trace = m00 + m11 + m22;
		if (trace > 0) {
			T s = 0.5f / std::sqrt(trace + 1.0f);
			Set(
				0.25f / s,
				(m21 - m12) * s,
				(m02 - m20) * s,
				(m10 - m01) * s
			);
		}
		else {
			if (m00 > m11 && m00 > m22) {
				T s = 2.0f * std::sqrt(1.0f + m00 - m11 - m22);
				Set(
					(m21 - m12) / s,
					0.25f * s,
					(m01 + m10) / s,
					(m02 + m20) / s
				);
			}
			else if (m11 > m22) {
				T s = 2.0f * std::sqrt(1.0f + m11 - m00 - m22);
				Set(
					(m02 - m20) / s,
					(m01 + m10) / s,
					0.25f * s,
					(m12 + m21) / s
				);
			}
			else {
				T s = 2.0f * std::sqrt(1.0f + m22 - m00 - m11);
				Set(
					(m10 - m01) / s,
					(m02 + m20) / s,
					(m12 + m21) / s,
					0.25f * s
				);
			}
		}
		Normalize();
	}

	template<typename T>
	inline void Quaternion<T>::ToAngleAxis(T& angle, Vector3<T>& axis) const
	{
		Quaternion q = normalized();
		angle = 2 * std::acos(std::clamp(q.w, T(-1), T(1)));
		//T s = std::sqrt(std::max(T(0), T(1) - q.w * q.w));
		T s = std::sqrt((std::max)(T(0), T(1) - q.w * q.w));
		if (s < 0.001f)
			axis = Vector3<T>(1, 0, 0);
		else
			axis = Vector3<T>(q.x / s, q.y / s, q.z / s);
	}

	//template<typename T>
	//Quaternion<T> Quaternion<T>::AngleAxis(float degrees, Vector3<T>& axis) {
	//	float radians = degrees * 0.0174533f;

	//	if (MagnitudeSq(axis) != 1) { // Do epsilon check here!
	//		axis = Normalize(axis);
	//	}

	//	Quaternion result;
	//	result.x = axis.x * sinf(radians * 0.5f);
	//	result.y = axis.y * sinf(radians * 0.5f);
	//	result.z = axis.z * sinf(radians * 0.5f);
	//	result.w = cosf(radians * 0.5f);

	//	return result;
	//}



#pragma endregion
#pragma region PublicMethods
	template <typename T>
	inline std::string Quaternion<T>::toString() const
	{
		return "Quaternion(" + std::to_string(w) + ", " +
			std::to_string(x) + ", " +
			std::to_string(y) + ", " +
			std::to_string(z) + ")";
	}

	template<typename T>
	inline T Quaternion<T>::Angle(const Quaternion<T>& a, const Quaternion<T>& b)
	{
		T dot = Dot(a, b);
		return std::acos(std::clamp(dot, T(-1), T(1))) * 2.0;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::AngleAxis(T angle, const Vector3<T>& axis) {
		Vector3<T> normAxis = axis.normalized();
		T halfAngle = angle * 0.5;
		T s = std::sin(halfAngle);
		return Quaternion(std::cos(halfAngle), normAxis.x * s, normAxis.y * s, normAxis.z * s);
	}

	template<typename T>
	T Quaternion<T>::Dot(const Quaternion<T>& a, const Quaternion<T>& b) {
		return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	}
	
	template<typename T>
		Quaternion<T> Quaternion<T>::Euler(T x, T y, T z) {
		T c1 = std::cos(y * 0.5);
		T s1 = std::sin(y * 0.5);
		T c2 = std::cos(z * 0.5);
		T s2 = std::sin(z * 0.5);
		T c3 = std::cos(x * 0.5);
		T s3 = std::sin(x * 0.5);

		return Quaternion(
			c1 * c2 * c3 - s1 * s2 * s3,
			c1 * c2 * s3 + s1 * s2 * c3,
			s1 * c2 * c3 + c1 * s2 * s3,
			c1 * s2 * c3 - s1 * c2 * s3
		);
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::FromToRotation(const Vector3<T>& from, const Vector3<T>& to) {
		Quaternion q;
		q.SetFromToRotation(from, to);
		return q;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::Inverse(const Quaternion<T>& q) {
		T magSq = q.magnitude();
		if (magSq > T(0)) {
			T invMagSq = T(1) / (magSq * magSq);
			return Quaternion(q.w * invMagSq, -q.x * invMagSq, -q.y * invMagSq, -q.z * invMagSq);
		}
		return identity;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::Lerp(const Quaternion<T>& a, const Quaternion<T>& b, T t) {
		t = std::clamp(t, T(0), T(1));
		return LerpUnclamped(a, b, t);
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::LerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t) {
		return (a * (T(1) - t) + b * t).normalized();
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::LookRotation(const Vector3<T>& forward, const Vector3<T>& upwards) {
		Quaternion<T> q;
		q.SetLookRotation(forward, upwards);
		return q;
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::RotateTowards(const Quaternion<T>& from, const Quaternion<T>& to, T maxDegreesDelta) {
		T angle = Angle(from, to);
		if (angle == 0) return to;
		T t = ((std::min)(T(1), maxDegreesDelta / angle));
		return SlerpUnclamped(from, to, t);
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t) {
		t = std::clamp(t, T(0), T(1));
		return SlerpUnclamped(a, b, t);
	}

	template<typename T>
	Quaternion<T> Quaternion<T>::SlerpUnclamped(const Quaternion<T>& a, const Quaternion<T>& b, T t) {
		Quaternion<T> qa = a.normalized();
		Quaternion<T> qb = b.normalized();

		T dot = Dot(qa, qb);

		if (dot < 0.0f) {
			qb = Quaternion(-qb.w, -qb.x, -qb.y, -qb.z);
			dot = -dot;
		}

		if (dot > 0.9995) {
			Quaternion<T> result = Quaternion(
				qa.w + t * (qb.w - qa.w),
				qa.x + t * (qb.x - qa.x),
				qa.y + t * (qb.y - qa.y),
				qa.z + t * (qb.z - qa.z)
			);
			result.Normalize();
			return result;
		}

		T theta_0 = std::acos(dot);
		T theta = theta_0 * t;
		T sin_theta = std::sin(theta);
		T sin_theta_0 = std::sin(theta_0);

		Quaternion<T> qc = (qa * std::cos(theta) + (qb - qa * dot) * (sin_theta / sin_theta_0));
		return qc;
	}
#pragma endregion


#pragma region Operators
	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& q) const {
		return Quaternion<T>(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		);
	}


	template<typename T>
	Quaternion<T> Quaternion<T>::operator*(T scalar) const
	{
		return Quaternion<T>(w * scalar, x * scalar, y * scalar, z * scalar);
	}

	template<typename T>
	inline Quaternion<T> Quaternion<T>::operator+(const Quaternion<T>& q) const
	{
		return Quaternion<T>(w + q.w, x + q.x, y + q.y, z + q.z);
	}

	template<typename T>
	inline Quaternion<T> Quaternion<T>::operator-(const Quaternion<T>& q) const
	{
		return Quaternion<T>(w - q.w, x - q.x, y - q.y, z - q.z);
	}

	template<typename T>
	bool Quaternion<T>::operator==(const Quaternion<T>& q) const
	{
		return (w == q.w && x == q.x && y == q.y && z == q.z);
	}


	template <typename T>
	const T& Quaternion<T>::operator[](int i) const {
		switch (i) {
		case 0: return w;
		case 1: return x;
		case 2: return y;
		case 3: return z;
		default:
			throw std::out_of_range("Quaternion index out of range");
		}
	}
#pragma endregion



}