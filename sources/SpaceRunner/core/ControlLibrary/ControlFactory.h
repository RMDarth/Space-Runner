#pragma once
#include "Control.h"

class ControlFactory
{
	static ControlFactory* _instance;
public:
	static ControlFactory* Instance();

	Control* CreateControl(SceneSector* sector, std::string type, std::string name, float x, float y, float width, float height, Control * parent, Control * parent2 = nullptr);

private:
	ControlFactory();
};