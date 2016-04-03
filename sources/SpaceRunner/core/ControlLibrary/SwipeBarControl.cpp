#include "SwipeBarControl.h"
#include "../PrecompileHeaders.h"
#include "Render/SceneSector.h"

SwipeBarControl::SwipeBarControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
: Control(sector, "SwipeBar", name, x, y, width, height, "swipebar_left.png", parent)
{
	_position = Left;
	_diff = 0;
}

void SwipeBarControl::SetSwipePosition(Position pos)
{
	_position = pos;
	
	switch (_position)
	{
	case Left:
		SetDefaultMaterial("swipebar_left.png");
		break;
	case Right:
		SetDefaultMaterial("swipebar_right.png");
		break;
	default:
		SetDefaultMaterial("swipebar.png");
		break;
	}
}

bool SwipeBarControl::OnMouseDown(int x, int y)
{
	if (Control::OnMouseDown(x, y))
	{
		_storedX = x;
		return true;
	}

	return false;
}

bool SwipeBarControl::OnMouseUp(int x, int y)
{
	int c_x, c_y, c_width, c_height;
	RenderProcessor::Instance()->GetViewport()->getActualDimensions(c_x, c_y, c_width, c_height);

	if (_pressed)
	{
		_diff = _storedX - x;
		if (abs(_diff) > c_width * 0.03f && _position != None)
		{
			if (_diff > 0)
				SetSwipePosition(Left);
			else
				SetSwipePosition(Right);
		}

		if (_mouseUpHandlerList.size() != 0)
		{
			std::for_each(_mouseUpHandlerList.begin(), _mouseUpHandlerList.end(), std::bind(&IEventHandler::ProcessEvent, std::placeholders::_1, this, IEventHandler::MouseUp, x, y));
		}

		_pressed = false;
		return true;
	}

	return false;
}

std::string SwipeBarControl::GetCustomAttribute(std::string name)
{
	if (name == "SwipePosition")
	{
		switch (_position)
		{
		case Left:
			return "Left";
		case Right:
			return "Right";
		default:
			return "None";
		}
	}

	return Control::GetCustomAttribute(name);
}

void SwipeBarControl::SetCustomAttribute(std::string name, std::string value)
{
	if (name == "SwipePosition")
	{
		if (value == "Left")
		{
			SetSwipePosition(Left);
		} 
		else if (value == "Right")
		{
			SetSwipePosition(Right);
		}
		else
		{
			SetSwipePosition(None);
		}
	}
	else 
	{
		Control::SetCustomAttribute(name, value);
	}
}

SwipeBarControl::Position SwipeBarControl::GetSwipePosition()
{
	return _position;
}