#pragma once
#include "Control.h"

class LevelButtonControl : public Control
{
	static float _pressedFontColor[4];
	static float _movedFontColor[4];
	static float _normalFontColor[4];
	bool _isFontColorChanged;

	Ogre::Rectangle2D* _starList[3];
	std::string _starMaterial;
	std::string _grayStarMaterial;
	bool _noStars;

public:
	LevelButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	virtual void SetImage(std::string image);
	virtual void SetStarsNum(std::string num);

	virtual void SetCustomAttribute(std::string name, std::string value);
	virtual std::string GetCustomAttribute(std::string name);

	virtual void SetVisible(bool visible);
	virtual void InitOverlay();

	virtual bool OnMouseMove(int x, int y, float deltaTime);
	virtual bool OnMouseDown(int x, int y);
};