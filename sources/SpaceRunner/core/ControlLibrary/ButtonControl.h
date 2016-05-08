#pragma once
#include "Control.h"

class ButtonControl : public Control
{
	static float _defaultPressedFontColor[4];
	static float _defaultMovedFontColor[4];
	static float _defaultNormalFontColor[4];

    float _pressedFontColor[4];
    float _movedFontColor[4];
    float _normalFontColor[4];

	bool _isFontColorChanged;
public:
    virtual void SetCustomAttribute(std::string name, std::string value) override;

    ButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	virtual bool OnMouseMove(int x, int y, float deltaTime);
	virtual bool OnMouseDown(int x, int y);



};