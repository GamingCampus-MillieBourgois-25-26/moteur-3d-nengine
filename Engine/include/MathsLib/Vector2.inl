#pragma once
namespace MathsLib
{
	template<typename T>  Vector2<T> Vector2<T>::down(0, -1);
	template<typename T>  Vector2<T> Vector2<T>::left(-1, 0);
	template<typename T>  Vector2<T> Vector2<T>::right(1, 0);
	template<typename T>  Vector2<T> Vector2<T>::up(0, 1);
	template<typename T>  Vector2<T> Vector2<T>::zero(0, 0);
	template<typename T>  Vector2<T> Vector2<T>::one(1, 1);
	template<typename T>  Vector2<T> Vector2<T>::negativeInfinity(
		-std::numeric_limits<T>::infinity(),
		-std::numeric_limits<T>::infinity()
	);
	template<typename T> Vector2<T> Vector2<T>::positiveInfinity(
		std::numeric_limits<T>::infinity(),
		std::numeric_limits<T>::infinity()
	);
#pragma region Properties

	template <typename T>
	T Vector2<T>::magnitude() const
	{
		return std::sqrt(x * x + y * y);
	}

	template <typename T>
	Vector2<T> Vector2<T>::normalized() const
	{
		T m = magnitude();
		return (m > 0) ? Vector2(x / m, y / m) : Vector2(0, 0);
	}

	template <typename T>
	T Vector2<T>::sqrMagnitude() const
	{
		return x * x + y * y;
	}

#pragma endregion

	template<typename T>
	Vector2<T>::Vector2(T X, T Y) : x(X), y(Y) {}

#pragma region Public Methods

	template<typename T>
	bool Vector2<T>::Equals(const Vector2& v) const
	{
		return (x == v.x && y == v.y) ? true : false;
	}

	template<typename T>
	void Vector2<T>::Normalize()
	{
		T m = magnitude();
		if (m > 0)
		{
			x = x / m;
			y = y / m;
		}
		else
		{
			x = 0;
			y = 0;
		}
	}

	template<typename T>
	void Vector2<T>::Set(T X, T Y)
	{
		x = X;
		y = Y;
	}

	template<typename T>
	std::string Vector2<T>::toString()
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

#pragma endregion

#pragma region Static Methods

	template<typename T>
	static T Vector2<T>::Angle(const Vector2& v1, const Vector2& v2) {
		T dot = Dot(v1, v2);
		T mag1 = v1.magnitude();
		T mag2 = v2.magnitude();

		if (mag1 == 0 || mag2 == 0) { std::cerr << "Error : A vector null has not angle defined\n"; }
		return 0;

		T cosTheta = dot / (mag1 * mag2);
		if (cosTheta > 1.0) cosTheta = 1.0;
		else if (cosTheta < -1.0) cosTheta = -1.0;

		return acos(cosTheta);
	}

	template<typename T>
	static Vector2<T> Vector2<T>::ClampMagnitude(const Vector2& v1, T maxLength)
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
	static T Vector2<T>::Distance(const Vector2& v1, const Vector2& v2) 
	{
		Vector2 sous(v1.x - v2.x, v1.y - v2.y);
		return sous.magnitude();
	}

	template<typename T>
	T Vector2<T>::Dot(const Vector2& v1, const Vector2& v2)  {
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<typename T>
	static Vector2<T> Vector2<T>::Lerp(const Vector2& v1, const Vector2& v2, T t) 
	{
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;

		return v1 + (v2 - v1) * t;
	}

	template<typename T>
	static Vector2<T> Vector2<T>::LerpUnclamped(const Vector2& v1, const Vector2& v2, T t) 
	{
		return v1 + (v2 - v1) * t;
	}

	template<typename T>
	static Vector2<T> Vector2<T>::Max(const Vector2& v1, const Vector2& v2) 
	{
		Vector2 maxVec(0, 0);
		if (v1.x > v2.x) { maxVec.x = v1.x; }
		else { maxVec.x = v2.x; }
		if (v1.y > v2.y) { maxVec.y = v1.y; }
		else { maxVec.y = v2.y; }

		return maxVec;
	}

	template<typename T>
	static Vector2<T> Vector2<T>::Min(const Vector2& v1, const Vector2& v2) 
	{
		Vector2 minVec(0, 0);
		if (v1.x < v2.x) { minVec.x = v1.x; }
		else { minVec.x = v2.x; }
		if (v1.y < v2.y) { minVec.y = v1.y; }
		else { minVec.y = v2.y; }

		return minVec;
	}

	template<typename T>
	static Vector2<T> Vector2<T>::MoveTowards(const Vector2& v1, const Vector2& v2, T maxDistanceDelta) 
	{
		Vector2 toVector = v2 - v1;
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
	static Vector2<T> Vector2<T>::Perpandicular(const Vector2& v1) 
	{
		return Vector2(-v1.y, v1.x);
	}

	template<typename T>
	static Vector2<T> Vector2<T>::Reflect(const Vector2& v1, const Vector2& v2) 
	{
		return v1 - v2.normalized() * (2 * Dot(v1, v2.normalized()));
	}
	
	template<typename T>
	static Vector2<T> Vector2<T>::Scale(const Vector2& v1, const Vector2& v2) 
	{
		return Vector2(v1.x * v2.x, v1.y * v2.y);
	}

	template<typename T>
	static T Vector2<T>::SignedAngle(const Vector2& v1, const Vector2& v2) 
	{
		T dot = Dot(v1, v2);
		T det = v1.x * v2.x - v1.y * v2.y;

		return std::atan2(det, dot) * 180 / std::numbers::pi;
	}


#pragma endregion

#pragma region Operators

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& o) const {
		return Vector2(x + o.x, y + o.y);
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& o) const {
		return Vector2(x - o.x, y - o.y);
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(T scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	template<typename T>
	bool Vector2<T>::operator/=(const Vector2& o) const
	{
		x /= o.x; 
		y /= o.y; 
		return *this;
	}

	template<typename T>
	bool Vector2<T>::operator==(const Vector2& o) const
	{
		return x == o.x && y == o.y;
	}

#pragma endregion
}