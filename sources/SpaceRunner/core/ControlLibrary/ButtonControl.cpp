#include "ButtonControl.h"

float ButtonControl::_defaultPressedFontColor[4] = { 0.082f, 0.925f, 0.976f, 1.0f };
float ButtonControl::_defaultMovedFontColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float ButtonControl::_defaultNormalFontColor[4] = { 0.086f, 0.925f, 0.976f, 1.0f };

ButtonControl::ButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "Button", name, x, y, width, height, "button_normal.png", parent)
{
	this->SetHoverMaterial("button_hover.png");
	this->SetPushMaterial("button_pressed.png");

    for (auto i = 0; i < 4; i++)
    {
        _normalFontColor[i] = _defaultNormalFontColor[i];
        _movedFontColor[i] = _defaultMovedFontColor[i];
        _pressedFontColor[i] = _defaultPressedFontColor[i];
    }

	_isFontColorChanged = false;
	SetFontColor(_normalFontColor);
}

bool ButtonControl::OnMouseMove(int x, int y, float deltaTime)
{
	bool result = Control::OnMouseMove(x, y, deltaTime);
	if (result && _useText)
	{
		if (_pressed)
			SetFontColor(_pressedFontColor);
		else
			SetFontColor(_movedFontColor);
		_isFontColorChanged = true;
	}
	else if (_isFontColorChanged) {
		SetFontColor(_normalFontColor);
	}

	return result;
}

bool ButtonControl::OnMouseDown(int x, int y)
{
	bool result = Control::OnMouseDown(x, y);
	if (result)
	{
		SetFontColor(_pressedFontColor);
		_isFontColorChanged = true;
	}
	else if (_isFontColorChanged) {
		SetFontColor(_normalFontColor);
	}

	return result;
}

void ButtonControl::SetCustomAttribute(std::string name, std::string value)
{
	if (name == "font-color")
	{
		std::stringstream str(value);
		str >> _normalFontColor[0] >> _normalFontColor[1] >> _normalFontColor[2] >> _normalFontColor[3];
		SetFontColor(_normalFontColor);
	} else if (name == "pressed-font-color")
	{
		std::stringstream str(value);
		str >> _pressedFontColor[0] >> _pressedFontColor[1] >> _pressedFontColor[2] >> _pressedFontColor[3];
	} else {
		Control::SetCustomAttribute(name, value);
	}
}

