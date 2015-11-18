#pragma once
#include "Control.h"

class ButtonControl : public Control
{
	static float _pressedFontColor[4];
	static float _movedFontColor[4];
	static float _normalFontColor[4];
	bool _isFontColorChanged;
public:
	ButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	virtual bool OnMouseMove(int x, int y, float deltaTime);
	virtual bool OnMouseDown(int x, int y);
};