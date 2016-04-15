#pragma once
#include "PrecompileHeaders.h"
#include "Basic.h"
using namespace CoreEngine;

class Camera
{
	Ogre::Camera *_camera;
	Vector3 _target;
public:
	Camera(Ogre::Camera* camera);
	~Camera();

	void SetPosition(Vector3 pos);
	Vector3 GetPosition();

	void SetRotation(float yaw, float pitch, float roll);
	void SetTarget(Vector3 pos);
	void SetOrthogonal();
	void SetPerspective();
	Vector3 GetTarget();
	Vector3 GetViewportCameraRay(float screenposX, float screenposY);
};

