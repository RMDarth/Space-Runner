#pragma once
#include "Control.h"

class SwipeBarControl : public Control
{
	int _storedX;
	int _diff;
public:
	enum Position
	{
		Right = 0,
		Left = 1,
		None = 2,
	};

	SwipeBarControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	void SetSwipePosition(Position pos);
	Position GetSwipePosition();
	
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseUp(int x, int y);

	virtual std::string GetCustomAttribute(std::string name);
	virtual void SetCustomAttribute(std::string name, std::string value);

protected:
	Position _position;
};