#include "Input.h"



Input::Input(Ogre::RenderWindow * win)
{
    OIS::ParamList parameters;
    unsigned int windowHandle = 0;
    std::ostringstream windowHandleString;
    win->getCustomAttribute("WINDOW", &windowHandle);
    windowHandleString << windowHandle;
    parameters.insert(std::make_pair("WINDOW", windowHandleString.str()));

    #if defined OIS_WIN32_PLATFORM
    parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    parameters.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    parameters.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
    parameters.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    parameters.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    parameters.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    parameters.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

    _inputManager = OIS::InputManager::createInputSystem(parameters);
    _mouse = static_cast<OIS::Mouse*>(_inputManager->createInputObject(OIS::OISMouse, false));
    _keyboard =  static_cast<OIS::Keyboard*>(_inputManager->createInputObject(OIS::OISKeyboard, true));

    unsigned int width, height, depth;
    int top, left;
    win->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &ms = _mouse->getMouseState();
    ms.width = width;
    ms.height = height;

    _timeMousePressed = -2;
    _timeMouseReleased = -1;
    for (auto i = 0; i < 2; i++)
    {
        _lastPressed[i] = 0;
        _lastReleased[i] = 0;
    }
    _minMove[0] = (int)(width * 0.01f);
    _minMove[1] = (int)(height * 0.01f);

    _totalTime = 0;

    _keyboard->setEventCallback(this);

    _mousePressed = false;
}


Input::~Input(void)
{
    _inputManager->destroyInputObject(_mouse);
    _inputManager->destroyInputObject(_keyboard);
    OIS::InputManager::destroyInputSystem(_inputManager);
}

void Input::AddListener(IInputListener* listener)
{
    _listenerList.push_back(listener);
}

void Input::RemoveListener(IInputListener* listener)
{
    _listenerList.erase(std::remove(_listenerList.begin(), _listenerList.end(), listener), _listenerList.end());
}

void Input::Update(float time)
{
    _totalTime += time;
    _mouse->capture();
    int xRel = _mouse->getMouseState().X.rel;
    int yRel = _mouse->getMouseState().Y.rel;
    int x = _mouse->getMouseState().X.abs;
    int y = _mouse->getMouseState().Y.abs;
    int pos[2] = {x, y};

    std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseMove, std::placeholders::_1, x, y, time));

    if (_mouse->getMouseState().buttons == 1)
    {
        if (!_mousePressed)
        {
            _mousePressed = true;
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

    } else {
        if (_mousePressed)
        {
            std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnMouseUp, std::placeholders::_1, x, y));
            _timeMouseReleased = _totalTime;
            _lastReleased[0] = x;
            _lastReleased[1] = y;
        }
        _mousePressed = false;
    }

    _keyboard->capture();
}

bool Input::keyPressed(const OIS::KeyEvent &arg)
{
    std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnKeyPressed, std::placeholders::_1, arg.key));
    return true;
}

bool Input::keyReleased(const OIS::KeyEvent &arg)
{
    std::for_each(_listenerList.begin(), _listenerList.end(), std::bind(&IInputListener::OnKeyReleased, std::placeholders::_1, arg.key));
    return true;
}

bool Input::isSame(int *point1, int *point2)
{
    return abs(point1[0] - point2[0]) <= _minMove[0]
           && abs(point1[1] - point2[1]) <= _minMove[1];
}


