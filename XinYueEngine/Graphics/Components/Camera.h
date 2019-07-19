#pragma once
#include "Resources.h"
class Camera
{
public:

	Camera();

	~Camera() {}



public:
	 

private:
	/*** Camera parameters ***/
	XMFLOAT3 mPosition;		// Camera's coordinates
	XMFLOAT3 mTarget;		// View target's coordinates
	XMFLOAT3 mUp;			// Camera's up vector end coordinates

	/*** Projection parameters ***/
	float mAngle;			// Angle of view frustum
	float mClientWidth;		// Window's width
	float mClientHeight;	// Window's height
	float mNearest;			// Nearest view frustum plane
	float mFarthest;		// Farthest view frustum plane

	XMFLOAT4X4  mView;		// View matrix
	XMFLOAT4X4	mProj;		// Projection matrix
	XMFLOAT4X4	mOrtho;		// Ortho matrix for drawing without tranformation
};




