#include "Camera.h"


Camera::Camera(Ogre::Camera* camera)
{
	_camera = camera;
	_target = Vector3();
}


Camera::~Camera()
{
}

void Camera::SetPosition(Vector3 pos)
{
	_camera->setPosition(VectorToOgre(pos));
}

Vector3 Camera::GetPosition()
{
	return VectorFromOgre(_camera->getPosition());
}

void Camera::SetRotation(float yaw, float pitch, float roll)
{
	_camera->yaw(Ogre::Degree(yaw));
	_camera->pitch(Ogre::Degree(pitch));
	_camera->roll(Ogre::Degree(roll));
}

void Camera::SetTarget(Vector3 pos)
{
	_camera->lookAt(VectorToOgre(pos));
	_target = pos;
}

Vector3 Camera::GetTarget()
{
	return _target;
}

Vector3 Camera::GetViewportCameraRay(float screenposX, float screenposY)
{
	auto ray = _camera->getCameraToViewportRay(screenposX, screenposY);
	return VectorFromOgre(ray.getDirection());
}

void Camera::SetOrthogonal()
{
	_camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	//_camera->setAspectRatio(1.0f);
}

void Camera::SetPerspective()
{
	_camera->setProjectionType(Ogre::PT_PERSPECTIVE);
}



