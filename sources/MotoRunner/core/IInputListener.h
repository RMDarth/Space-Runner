#pragma once
#include "Basic.h"
#include "OIS/OIS.h"

class IInputListener
{
public:
	virtual bool OnMouseMove(int x, int y, float deltaTime) { return false; };
	virtual bool OnMouseDown(int x, int y)  { return false; };
	virtual bool OnMouseUp(int x, int y)  { return false; };

	virtual void OnKeyPressed(OIS::KeyCode key) { };
	virtual void OnKeyReleased(OIS::KeyCode key) { };
};