#pragma once
#include "Control.h"

class PanelControl : public Control
{
public:
	PanelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);
};