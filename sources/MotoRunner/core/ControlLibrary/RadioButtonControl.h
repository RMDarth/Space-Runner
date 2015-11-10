#pragma once
#include "Control.h"

class RadioButtonControl : public Control
{
	bool _checked;
	std::string _checkedMaterial;
	std::string _uncheckedMaterial;

public:
	RadioButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);
	virtual bool OnMouseUp(int x, int y);

	bool IsChecked();
	void SetChecked(bool value);
	
	virtual std::string GetCustomAttribute(std::string name);

private:
	void UpdateState();
};