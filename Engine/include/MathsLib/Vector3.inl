//#include "Vector3.h"
//#pragma once

namespace MathsLib
{
	//template <typename T>
	//Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z)
	//{
	//}

	template <typename T>
	Vector3<T>::Vector3()
		: x(0), y(0), z(0)
	{}

	template <typename T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x), y(y), z(z)
	{}
#pragma region StaticProperties
	template<typename T>  Vector3<T> Vector3<T>::back(0, 0, -1);
	template<typename T>  Vector3<T> Vector3<T>::down(0, -1, 0);
	template<typename T>  Vector3<T> Vector3<T>::forward(0, 0, 1);
	template<typename T>  Vector3<T> Vector3<T>::left(-1, 0, 0);
	template<typename T>  Vector3<T> Vector3<T>::right(1, 0, 0);
	template<typename T>  Vector3<T> Vector3<T>::up(0, 1, 0);
	template<typename T>  Vector3<T> Vector3<T>::zero(0, 0, 0);
	template<typename T>  Vector3<T> Vector3<T>::one(1, 1, 1);
	template<typename T>  Vector3<T> Vector3<T>::negativeInfinity(
		-std::numeric_limits<T>::infinity(),
		-std::numeric_limits<T>::infinity(),
		-std::numeric_limits<T>::infinity()
	);
	template<typename T> Vector3<T> Vector3<T>::positiveInfinity(
		std::numeric_limits<T>::infinity(),
		std::numeric_limits<T>::infinity(),
		std::numeric_limits<T>::infinity()
	);

#pragma endregion

#pragma region Properties
	template <typename T>
	T Vector3<T>::magnitude() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::normalized() const
	{
		T m = magnitude();
		return (m > T(0)) ? /*Vector3(x / m, y / m, z / m)*/ (*this) * (T(1) / m) : Vector3(T(0), T(0), T(0));
	}

	template <typename T>
	T Vector3<T>::sqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

#pragma endregion

#pragma region Operators
	template <typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& o) const {
		return Vector3(x + o.x, y + o.y, z + o.z);
	}
	template <typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& o) const {
		return Vector3(x - o.x, y - o.y, z - o.z);
	}

	template<typename T>
	inline Vector3<T> Vector3<T>::operator*(Vector3& v1) const
	{
		return Vector3(v1.x * x, v1.y * y, v1.z * z);
	}

	template <typename T>
	Vector3<T> Vector3<T>::operator*(T scalar) const {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}


	template <typename T>
	Vector3<T> Vector3<T>::operator/(T scalar) const {
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	template <typename T>
	bool Vector3<T>::operator/=(const Vector3& o) const
	{
		return (x != o.x && y != o.y && z != o.z);
		//keep first one
		//return ((this->x != o.x) and (this->y != o.y) and (this->z != o.z)) ? true : false;
	}
	template <typename T>
	bool Vector3<T>::operator==(const Vector3& o) const
	{
		return (x == o.x && y == o.y && z == o.z);
		//keep first one
		//return ((this->x == o.x) and (this->y == o.y) and (this->z == o.z)) ? true : false;
	}
	//template <typename T>
	//bool Vector3<T>::operator==(const Vector3& o) const
	//{
	//	const T eps = std::numeric_limits<T>::epsilon();
	//	return (std::abs(x - o.x) < eps &&
	//		std::abs(y - o.y) < eps &&
	//		std::abs(z - o.z) < eps);
	//}

	template <typename T>
	const T& Vector3<T>::operator[](int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
			throw std::out_of_range("Vector3 index out of range");
		}
	}
#pragma endregion

#pragma region Statics Methods


	template <typename T>
	Vector3<T> Vector3<T>::Cross(const Vector3& o, const Vector3& p) {
		return Vector3(o.y * p.z - o.z * p.y, o.z * p.x - o.x * p.z, o.x * p.y - o.y * p.x);
	}

	template <typename T>
	Vector3<T> Vector3<T>::OrthoNormalize(const Vector3& u, const Vector3& v) {

		Vector3<T> uNorm = u.normalized();
		Vector3<T> vProj = v - uNorm * Dot(v, uNorm);
		Vector3<T> vNorm = vProj.normalized();

		return vNorm;
	}

	template <typename T>
	Vector3<T> Vector3<T>::Project(const Vector3& vector, const Vector3& onNormal)
	{
		T denom = Dot(onNormal, onNormal);
		if (denom == 0.0f)
			return Vector3(0, 0, 0);
		T scalar = Dot(vector, onNormal) / denom;
		return Vector3(onNormal.x * scalar, onNormal.y * scalar, onNormal.z * scalar);
	}

	template <typename T>
	Vector3<T> Vector3<T>::ProjectOnPlane(const Vector3& vector, const Vector3 planeNormal)
	{
		return vector - planeNormal * Dot(vector, planeNormal);
	}

	template <typename T>
	Vector3<T> Vector3<T>::RotateTowards(const Vector3& current, const Vector3& target, T maxDistanceDelta)
	{
		Vector3 delta = target - current;
		T dist = delta.magnitude();

		return (dist <= maxDistanceDelta || dist == 0.0f) ? target : current + delta * (maxDistanceDelta / dist);
	}

	template <typename T>
	Vector3<T> Vector3<T>::Slerp(const Vector3& from, const Vector3& to, T t)
	{
		t = std::clamp(t, 0.f, 1.f);

		Vector3 a = from.normalized();
		Vector3 b = to.normalized();

		T dot = Dot(a, b);
		dot = std::clamp(dot, -1.0f, 1.0f);

		T theta = std::acos(dot) * t;

		Vector3 relative = (b - a * dot).normalized();

		return a * std::cos(theta) + relative * std::sin(theta);
	}

	template <typename T>
	Vector3<T> Vector3<T>::SlerpUnclamped(const Vector3& from, const Vector3& to, T t)
	{
		Vector3 a = from.normalized();
		Vector3 b = to.normalized();

		T dot = Dot(a, b);
		dot = std::fmax(std::fmin(dot, 1.0f), -1.0f);

		T theta = std::acos(dot) * t;

		Vector3 relative = (b - a * dot).normalized();

		return a * std::cos(theta) + relative * std::sin(theta);
	}

	template<typename T>
	bool Vector3<T>::Equals(const Vector3& v) const
	{
		return (x == v.x && y == v.y && z == v.z) ? true : false;
	}

	template<typename T>
	void Vector3<T>::Normalize()
	{
		T m = magnitude();
		if (m > 0)
		{
			x = x / m;
			y = y / m;
			z = z / m;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
		}
	}

	template<typename T>
	void Vector3<T>::Set(T X, T Y, T Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	template<typename T>
	std::string Vector3<T>::toString()
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + " , " + std::to_string(z) + ")";
	}

#pragma endregion

#pragma region Static Methods

	template<typename T>
	T Vector3<T>::Angle(const Vector3& v1, const Vector3& v2) {
		T mag1 = v1.magnitude();
		T mag2 = v2.magnitude();
		if (mag1 == 0 || mag2 == 0) {
			std::cerr << "Error: null vector has no angle defined\n";
			return 0;
		}
		T dot = Dot(v1, v2) / (mag1 * mag2);
		dot = std::clamp(dot, -1.0, 1.0);
		return std::acos(dot);
	}


	template<typename T>
	Vector3<T> Vector3<T>::ClampMagnitude(const Vector3& v1, T maxLength)
	{
		T mag = v1.magnitude();

		if (mag > maxLength)
		{
			return v1.normalized() * maxLength;
		}
		else
			return v1;
	}

	template<typename T>
	T Vector3<T>::Distance(const Vector3& v1, const Vector3& v2)
	{
		Vector3 sous(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		return sous.magnitude();
	}

	template<typename T>
	T Vector3<T>::Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::Lerp(const Vector3& v1, const Vector3& v2, T t)
	{
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;

		return v1 + (v2 - v1) * t;
	}

	template<typename T>
	Vector3<T> Vector3<T>::LerpUnclamped(const Vector3& v1, const Vector3& v2, T t)
	{
		return v1 + (v2 - v1) * t;
	}

	template<typename T>
	Vector3<T> Vector3<T>::Max(const Vector3& v1, const Vector3& v2)
	{
		Vector3 maxVec(0, 0, 0);
		if (v1.x > v2.x) { maxVec.x = v1.x; }
		else { maxVec.x = v2.x; }
		if (v1.y > v2.y) { maxVec.y = v1.y; }
		else { maxVec.y = v2.y; }
		if (v1.z > v2.z) { maxVec.z = v1.z; }
		else { maxVec.z = v2.z; }

		return maxVec;
	}

	template<typename T>
	Vector3<T> Vector3<T>::Min(const Vector3& v1, const Vector3& v2)
	{
		Vector3 minVec(0, 0, 0);
		if (v1.x < v2.x) { minVec.x = v1.x; }
		else { minVec.x = v2.x; }
		if (v1.y < v2.y) { minVec.y = v1.y; }
		else { minVec.y = v2.y; }
		if (v1.z < v2.z) { minVec.z = v1.z; }
		else { minVec.z = v2.z; }

		return minVec;
	}

	template<typename T>
	Vector3<T> Vector3<T>::MoveTowards(const Vector3& v1, const Vector3& v2, T maxDistanceDelta)
	{
		Vector3 toVector = v2 - v1;
		T distance = Distance(v1, v2);

		if (distance <= maxDistanceDelta || distance == 0)
		{
			return v2;
		}
		else
		{
			return v1 + toVector * (maxDistanceDelta / distance);
		}
	}


	template<typename T>
	Vector3<T> Vector3<T>::Reflect(const Vector3& v1, const Vector3& v2)
	{
		return v1 - v2.normalized() * (2 * Dot(v1, v2.normalized()));
	}

	template<typename T>
	Vector3<T> Vector3<T>::Scale(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	template<typename T>
	T Vector3<T>::SignedAngle(const Vector3& v1, const Vector3& v2, const Vector3& axis)
	{
		Vector3<T> n1 = v1.normalized();
		Vector3<T> n2 = v2.normalized();

		T dot = Dot(n1, n2);
		Vector3<T> cross = Cross(n1, n2);

		// L'angle est signé selon la projection sur l'axe
		T angle = std::atan2(cross.dot(axis), dot);

		return angle;
	}

#pragma endregion

}

