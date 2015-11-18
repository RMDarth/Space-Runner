#pragma once
#include "Control.h"

class ContainerControl : public Control
{
public:
	ContainerControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);
};