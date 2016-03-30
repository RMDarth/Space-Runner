#pragma once
#include "PrecompileHeaders.h"
#include "IInputListener.h"
#include <android/input.h>

class Input
{
	//OIS::InputManager* _inputManager;
	//OIS::Mouse* _mouse;

	bool _mousePressed;

	std::vector<IInputListener*> _listenerList;

public:
	Input(Ogre::RenderWindow * win);
	~Input();

	void AddListener(IInputListener* listener);
	void RemoveListener(IInputListener* listener);
	void Update(float time);

	void HandleInput( AInputEvent* event);
	void HandleInput( OIS::KeyCode code);
};

