#pragma once
#include "PrecompileHeaders.h"
#include "IInputListener.h"

class Input : OIS::KeyListener
{
	OIS::InputManager* _inputManager;
	OIS::Mouse* _mouse;
	OIS::Keyboard *_keyboard;

	bool _mousePressed;

	std::vector<IInputListener*> _listenerList;

public:
	Input(Ogre::RenderWindow * win);
	~Input();

	void AddListener(IInputListener* listener);
	void RemoveListener(IInputListener* listener);
	void Update(float time);

	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

};

