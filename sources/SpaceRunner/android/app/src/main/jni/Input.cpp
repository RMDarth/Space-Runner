#include "Input.h"

Input::Input(Ogre::RenderWindow * win)
{
    unsigned int width, height, depth;
    int top, left;
    win->getMetrics(width, height, depth, left, top);

    _timeMousePressed = -2;
    _timeMouseReleased = -1;
    for (auto i = 0; i < 2; i++)
    {
        _lastPressed[i] = 0;
        _lastReleased[i] = 0;
    }
    _minMove[0] = (int)(width * 0.1f);
    _minMove[1] = (int)(height * 0.1f);

    _totalTime = 0;
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

		int pos[2] = {x, y};

		std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseMove, std::placeholders::_1, x, y, 0));
        if(action == AMOTION_EVENT_ACTION_DOWN) 
		{
			std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseDown, std::placeholders::_1, x, y));

			if (_timeMouseReleased - _timeMousePressed <= 0.5f
				&& _totalTime - _timeMouseReleased <= 0.5f
				&& isSame(_lastPressed, _lastReleased)
				&& isSame(_lastReleased, pos))
			{
				std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseDoubleClick, std::placeholders::_1, x, y));
			} else {
				_timeMousePressed = _totalTime;
			}
			_lastPressed[0] = x;
			_lastPressed[1] = y;
		}           
		if(action == AMOTION_EVENT_ACTION_UP)
		{
			_timeMouseReleased = _totalTime;
			_lastReleased[0] = x;
			_lastReleased[1] = y;
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
	_totalTime+=time;
}


bool Input::isSame(int *point1, int *point2)
{
	return abs(point1[0] - point2[0]) <= _minMove[0]
		   && abs(point1[1] - point2[1]) <= _minMove[1];
}
