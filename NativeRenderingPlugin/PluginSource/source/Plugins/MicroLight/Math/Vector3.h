#pragma once
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include "Vector4.h"
#include <iostream>
const float PI = 3.1415926535897931f;
const float Deg2Rad = 0.0174532924F;
const float Rad2Deg = 57.29578F;
 
///////////////////////////////////////////////////////////////////////////////
// 3D vector
///////////////////////////////////////////////////////////////////////////////
 struct Vector3
{
	float x;
	float y;
	float z;

	// ctors
	Vector3() : x(0), y(0), z(0) {};
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

	// utils functions
	void        set(float x, float y, float z);
	float       length() const;                         //
	float       distance(const Vector3& vec) const;     // distance between two vectors
	Vector3&    normalize();                            //
	float       dot(const Vector3& vec) const;          // dot product
	Vector3     cross(const Vector3& vec) const;        // cross product
	bool        equal(const Vector3& vec, float e) const; // compare with epsilon

	// operators
	Vector3     operator-() const;                      // unary operator (negate)
	Vector3     operator+(const Vector3& rhs) const;    // add rhs
	Vector3     operator-(const Vector3& rhs) const;    // subtract rhs
	Vector3&    operator+=(const Vector3& rhs);         // add rhs and update this object
	Vector3&    operator-=(const Vector3& rhs);         // subtract rhs and update this object
	Vector3     operator*(const float scale) const;     // scale
	Vector3     operator*(const Vector3& rhs) const;    // multiplay each element
	Vector3&    operator*=(const float scale);          // scale and update this object
	Vector3&    operator*=(const Vector3& rhs);         // product each element and update this object
	Vector3     operator/(const float scale) const;     // inverse scale
	Vector3&    operator/=(const float scale);          // scale and update this object
	bool        operator==(const Vector3& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Vector3& rhs) const;   // exact compare, no epsilon
	bool        operator<(const Vector3& rhs) const;    // comparison for sort
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float&      operator[](int index);                  // subscript operator v[0], v[1]

	friend Vector3 operator*(const float a, const Vector3 vec);
	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
};




///////////////////////////////////////////////////////////////////////////////
// inline functions for Vector3
///////////////////////////////////////////////////////////////////////////////
inline Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator+(const Vector3& rhs) const {
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline Vector3 Vector3::operator-(const Vector3& rhs) const {
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline Vector3& Vector3::operator+=(const Vector3& rhs) {
	x += rhs.x; y += rhs.y; z += rhs.z; return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& rhs) {
	x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
}

inline Vector3 Vector3::operator*(const float a) const {
	return Vector3(x*a, y*a, z*a);
}

inline Vector3 Vector3::operator*(const Vector3& rhs) const {
	return Vector3(x*rhs.x, y*rhs.y, z*rhs.z);
}

inline Vector3& Vector3::operator*=(const float a) {
	x *= a; y *= a; z *= a; return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& rhs) {
	x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
}

inline Vector3 Vector3::operator/(const float a) const {
	return Vector3(x / a, y / a, z / a);
}

inline Vector3& Vector3::operator/=(const float a) {
	x /= a; y /= a; z /= a; return *this;
}

inline bool Vector3::operator==(const Vector3& rhs) const {
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

inline bool Vector3::operator!=(const Vector3& rhs) const {
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

inline bool Vector3::operator<(const Vector3& rhs) const {
	if (x < rhs.x) return true;
	if (x > rhs.x) return false;
	if (y < rhs.y) return true;
	if (y > rhs.y) return false;
	if (z < rhs.z) return true;
	if (z > rhs.z) return false;
	return false;
}

inline float Vector3::operator[](int index) const {
	return (&x)[index];
}

inline float& Vector3::operator[](int index) {
	return (&x)[index];
}

inline void Vector3::set(float x_, float y_, float z_) {
	this->x = x_; this->y = y_; this->z = z_;
}

inline float Vector3::length() const {
	return sqrtf(x*x + y * y + z * z);
}

inline float Vector3::distance(const Vector3& vec) const {
	return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z));
}
inline float Dot(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
inline float SqrMagnitude(const Vector3& inV)
{
	return   Dot(inV, inV);
}
 
inline float Magnitude(const Vector3& inV)
{
	return sqrt(Dot(inV, inV));
}
// Normalizes a vector, asserts if the vector can be normalized
inline Vector3 Normalize(const Vector3& inV)
{

	return  inV / Magnitude(inV);
}
inline float Distance(const Vector3& point1, const Vector3& point2)
{
	float distance = sqrt((point1.x - point2.x) * (point1.x - point2.x) +
		(point1.y - point2.y) * (point1.y - point2.y) +
		(point1.z - point2.z) * (point1.z - point2.z));
	return distance;
}


/// Returns a Vector3 that moves lhs towards rhs by a maximum of clampedDistance
inline Vector3 MoveTowards(const Vector3& lhs, const Vector3& rhs, float clampedDistance)
{
	Vector3 delta = rhs - lhs;
	float sqrDelta = SqrMagnitude(delta);
	float sqrClampedDistance = clampedDistance * clampedDistance;
	if (sqrDelta > sqrClampedDistance)
	{
		float deltaMag = sqrt(sqrDelta);
		if (deltaMag > 0.00001F)
			return lhs + delta / deltaMag * clampedDistance;
		else
			return lhs;
	}
	else
		return rhs;
}
inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x);
}
inline  float Abs(float v)
{
	return v < 0.0F ? -v : v;
}
// Given 3D vector 'v', returns its module (i.e. length)
inline float Module(const float * v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline float Module(const Vector3& rhs)
{
	return sqrt(rhs.x* rhs.x + rhs.y *rhs.y + rhs.z * rhs.z);
}

// Given two 3D vectors, returns the dot product between them
inline float Dot(const float * a, const float * b)
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

// Given a pair of 3D vectors, returns the angle between them.
// Angle is expressed in radians, in range [0, PI]
inline float Angle(const float * a, const float * b)
{
	//
	// From dot product definition:
	//
	// a . b = |a| |b| cos(theta)
	//
	//  ==>  cos(theta) = a . b / (|a| |b|)
	//
	//  ==>  theta = acos( a . b / (|a| |b|) )
	//

	return acos(Dot(a, b) / (Module(a) * Module(b)));
}
inline float Angle(const Vector3& lhs, const Vector3& rhs)
{
	//
	// From dot product definition:
	//
	// a . b = |a| |b| cos(theta)
	//
	//  ==>  cos(theta) = a . b / (|a| |b|)
	//
	//  ==>  theta = acos( a . b / (|a| |b|) )
	//

	return acos(Dot(lhs, rhs) / (Module(lhs) * Module(rhs)));
}

inline  float Atan(float f)
{
	return (float)atan(f);
}
inline Vector3 GetIntersectWithLineAndPlane(const Vector3& point, const Vector3& direct, const Vector3& planeNormal, const Vector3& planePoint)
{
	float d = Dot(planePoint - point, planeNormal) / Dot(Normalize(direct), planeNormal);

	return d * Normalize(direct) + point;
}



inline Vector3& Vector3::normalize() {
	//@@const float EPSILON = 0.000001f;
	float xxyyzz = x * x + y * y + z * z;
	//@@if(xxyyzz < EPSILON)
	//@@    return *this; // do nothing if it is ~zero vector

	//float invLength = invSqrt(xxyyzz);
	float invLength = 1.0f / sqrtf(xxyyzz);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}

inline float Vector3::dot(const Vector3& rhs) const {
	return (x*rhs.x + y * rhs.y + z * rhs.z);
}

inline Vector3 Vector3::cross(const Vector3& rhs) const {
	return Vector3(y*rhs.z - z * rhs.y, z*rhs.x - x * rhs.z, x*rhs.y - y * rhs.x);
}

inline bool Vector3::equal(const Vector3& rhs, float epsilon) const {
	return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon && fabs(z - rhs.z) < epsilon;
}

inline Vector3 operator*(const float a, const Vector3 vec) {
	return Vector3(a*vec.x, a*vec.y, a*vec.z);
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}
// END OF VECTOR3 /////////////////////////////////////////////////////////////

#include <DirectXMath.h>

using namespace DirectX;

inline XMVECTOR GMathFV(XMFLOAT3 val)
{
	return XMLoadFloat3(&val);
}
inline XMFLOAT3 GMathVF(XMVECTOR vec)
{
	XMFLOAT3 val;
	XMStoreFloat3(&val, vec);
	return val;
}
inline XMFLOAT4X4 GMathMF(FXMMATRIX  vec)
{
	XMFLOAT4X4 val;
	XMStoreFloat4x4(&val, vec);
	return val;
}
inline FXMMATRIX GMathFM(XMFLOAT4X4  val)
{
 
	return XMLoadFloat4x4(&val);
}

inline XMFLOAT3 Up(Vector3 position, Vector3 target)
{
	XMFLOAT3	mPosition = XMFLOAT3(position.x, position.y, position.z);
	XMFLOAT3	mTarget = XMFLOAT3(target.x, target.y, target.z);
	XMFLOAT3	mUp = GMathVF(GMathFV(mPosition) + GMathFV(XMFLOAT3(0, 1, 0)));

	return GMathVF(GMathFV(mUp) - GMathFV(mPosition));

}
inline Vector3 GetForward(Vector3 eulerangles)
{
	float fX, fY, fZ, mag;

	// Calculate forward based on yaw and pitch
	fX = sin(eulerangles.y * Deg2Rad) * cos(eulerangles.x * Deg2Rad);
	fY = -sin(eulerangles.x * Deg2Rad);
	fZ = cos(eulerangles.y * Deg2Rad) * cos(eulerangles.x * Deg2Rad);

	// Calculate magnitude, normalize
	mag = sqrt(fX*fX + fY * fY + fZ * fZ);
	fX /= mag; fY /= mag; fZ /= mag;


	return Vector3(fX, fY, fZ);
}
