#include "LevelButtonControl.h"
#include "../PrecompileHeaders.h"
#include "../SceneSector.h"
#include "../Basic.h"

#include "OgreOverlayElement.h"

#include <string>
#include <sstream>

float LevelButtonControl::_pressedFontColor[4] = { 0.722f, 0.973f, 0.992f, 1.0f };
float LevelButtonControl::_movedFontColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float LevelButtonControl::_normalFontColor[4] = { 0.086f, 0.925f, 0.976f, 1.0f };

LevelButtonControl::LevelButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "LevelButton", name, x, y, width, -1, "level0.png", parent)
{
	_noStars = false;
	_overlayText->setPosition(0, -0.045f);
	_overlayTextShadow->setPosition(0.002f, -0.042f);

	_isFontColorChanged = false;
	SetFontColor(_normalFontColor);
}


void LevelButtonControl::SetImage(std::string image)
{
	CreateMaterial(image);
	_defaultMaterial = image;
	SetMaterial(image);
}

void LevelButtonControl::InitOverlay()
{
	Control::InitOverlay();
	_overlayText->setPosition(0, -0.045f);
	_overlayTextShadow->setPosition(0.002f, -0.042f);
}

void LevelButtonControl::SetStarsNum(std::string num)
{
	if (num == "-1")
	{
		SetDefaultMaterial("levelx0.png");
		_noStars = true;
		return;
	}
	else {
		SetDefaultMaterial("level" + num + ".png");
	}
}

void LevelButtonControl::SetCustomAttribute(std::string name, std::string value)
{
	if (name == "source")
	{
		SetImage(value);
	} 
	if (name == "stars")
	{
		SetStarsNum(value);
	}
	
	Control::SetCustomAttribute(name, value);
}

std::string LevelButtonControl::GetCustomAttribute(std::string name)
{
	if (name == "source")
		return _defaultMaterial;

	return Control::GetCustomAttribute(name);
}

void LevelButtonControl::SetVisible(bool visible)
{
	Control::SetVisible(visible);
}

bool LevelButtonControl::OnMouseMove(int x, int y, float deltaTime)
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

bool LevelButtonControl::OnMouseDown(int x, int y)
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