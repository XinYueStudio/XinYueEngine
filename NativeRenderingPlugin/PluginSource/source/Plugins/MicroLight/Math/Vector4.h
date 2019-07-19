#pragma once
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
// 4D vector
///////////////////////////////////////////////////////////////////////////////
 struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	// ctors
	Vector4() : x(0), y(0), z(0), w(0) {};
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	// utils functions
	void        set(float x, float y, float z, float w);
	float       length() const;                         //
	float       distance(const Vector4& vec) const;     // distance between two vectors
	Vector4&    normalize();                            //
	float       dot(const Vector4& vec) const;          // dot product
	bool        equal(const Vector4& vec, float e) const; // compare with epsilon

	// operators
	Vector4     operator-() const;                      // unary operator (negate)
	Vector4     operator+(const Vector4& rhs) const;    // add rhs
	Vector4     operator-(const Vector4& rhs) const;    // subtract rhs
	Vector4&    operator+=(const Vector4& rhs);         // add rhs and update this object
	Vector4&    operator-=(const Vector4& rhs);         // subtract rhs and update this object
	Vector4     operator*(const float scale) const;     // scale
	Vector4     operator*(const Vector4& rhs) const;    // multiply each element
	Vector4&    operator*=(const float scale);          // scale and update this object
	Vector4&    operator*=(const Vector4& rhs);         // multiply each element and update this object
	Vector4     operator/(const float scale) const;     // inverse scale
	Vector4&    operator/=(const float scale);          // scale and update this object
	bool        operator==(const Vector4& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Vector4& rhs) const;   // exact compare, no epsilon
	bool        operator<(const Vector4& rhs) const;    // comparison for sort
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float&      operator[](int index);                  // subscript operator v[0], v[1]

	friend Vector4 operator*(const float a, const Vector4 vec);
	friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);
};

///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector4
///////////////////////////////////////////////////////////////////////////////
inline Vector4 Vector4::operator-() const {
	return Vector4(-x, -y, -z, -w);
}

inline Vector4 Vector4::operator+(const Vector4& rhs) const {
	return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

inline Vector4 Vector4::operator-(const Vector4& rhs) const {
	return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

inline Vector4& Vector4::operator+=(const Vector4& rhs) {
	x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;
}

inline Vector4& Vector4::operator-=(const Vector4& rhs) {
	x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
}

inline Vector4 Vector4::operator*(const float a) const {
	return Vector4(x*a, y*a, z*a, w*a);
}

inline Vector4 Vector4::operator*(const Vector4& rhs) const {
	return Vector4(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}

inline Vector4& Vector4::operator*=(const float a) {
	x *= a; y *= a; z *= a; w *= a; return *this;
}

inline Vector4& Vector4::operator*=(const Vector4& rhs) {
	x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this;
}

inline Vector4 Vector4::operator/(const float a) const {
	return Vector4(x / a, y / a, z / a, w / a);
}

inline Vector4& Vector4::operator/=(const float a) {
	x /= a; y /= a; z /= a; w /= a; return *this;
}

inline bool Vector4::operator==(const Vector4& rhs) const {
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

inline bool Vector4::operator!=(const Vector4& rhs) const {
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
}

inline bool Vector4::operator<(const Vector4& rhs) const {
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	if (w < rhs.w) return true;
	if (w > rhs.w) return false;
	return false;
}

inline float Vector4::operator[](int index) const {
	return (&x)[index];
}

inline float& Vector4::operator[](int index) {
	return (&x)[index];
}

inline void Vector4::set(float x_, float y_, float z_, float w_) {
	this->x = x_; this->y = y_; this->z = z_; this->w = w_;
}

inline float Vector4::length() const {
	return sqrtf(x*x + y * y + z * z + w * w);
}

inline float Vector4::distance(const Vector4& vec) const {
	return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z) + (vec.w - w)*(vec.w - w));
}

inline Vector4& Vector4::normalize() {
	//NOTE: leave w-component untouched
	//@@const float EPSILON = 0.000001f;
	float xxyyzz = x * x + y * y + z * z;
	//@@if(xxyyzz < EPSILON)
	//@@    return *this; // do nothing if it is zero vector

	//float invLength = invSqrt(xxyyzz);
	float invLength = 1.0f / sqrtf(xxyyzz);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}

inline float Vector4::dot(const Vector4& rhs) const {
	return (x*rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
}

inline bool Vector4::equal(const Vector4& rhs, float epsilon) const {
	return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon &&
		fabs(z - rhs.z) < epsilon && fabs(w - rhs.w) < epsilon;
}

inline Vector4 operator*(const float a, const Vector4 vec) {
	return Vector4(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
}

inline std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}
// END OF VECTOR4 /////////////////////////////////////////////////////////////
