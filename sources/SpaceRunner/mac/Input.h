#pragma once
#include "PrecompileHeaders.h"
#include "Input/IInputListener.h"

class Input : OIS::KeyListener
{
	OIS::InputManager* _inputManager;
	OIS::Mouse* _mouse;
	OIS::Keyboard *_keyboard;

	bool _mousePressed;

	float _timeMousePressed;
	float _timeMouseReleased;
	float _totalTime;

	int _lastPressed[2];
	int _lastReleased[2];
	int _minMove[2];

	std::vector<IInputListener*> _listenerList;

public:
	Input(Ogre::RenderWindow * win);
	~Input();

	void AddListener(IInputListener* listener);
	void RemoveListener(IInputListener* listener);
	void Update(float time);

	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

private:
	bool isSame(int* point1, int* point2);

};

