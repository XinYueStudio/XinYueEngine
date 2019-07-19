#pragma once
#include "Component.h"

class Transform:Component
{
public:
	Transform();
	~Transform();

	Vector3 Position;
	Vector3 LocalPosition;
	Vector3 EulerAngles;
	Vector3 LocalEulerAngles;
	Vector3 Scale;
	Vector3 LocalScale;


};