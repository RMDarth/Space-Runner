#include "Input.h"

Input::Input(Ogre::RenderWindow * win)
{

}


Input::~Input(void)
{
	
}

void Input::AddListener(IInputListener* listener)
{
	_listenerList.push_back(listener);
}

void Input::RemoveListener(IInputListener* listener)
{
	_listenerList.erase(std::remove(_listenerList.begin(), _listenerList.end(), listener), _listenerList.end());
}

void Input::HandleInput( AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
    {
        int action = (int)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event));
           
		int x = AMotionEvent_getRawX(event, 0);
		int y = AMotionEvent_getRawY(event, 0);
		std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseMove, std::placeholders::_1, x, y, 0));
        if(action == AMOTION_EVENT_ACTION_DOWN) 
		{
			std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseDown, std::placeholders::_1, x, y));
		}           
		if(action == AMOTION_EVENT_ACTION_UP)
		{
			std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseUp, std::placeholders::_1, x, y));
		}     
    } 
}

void Input::HandleInput( OIS::KeyCode code)
{
	std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnKeyPressed, std::placeholders::_1, code));
}

void Input::Update(float time)
{

}
