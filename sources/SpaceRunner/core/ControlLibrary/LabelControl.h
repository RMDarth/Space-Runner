#pragma once
#include "Control.h"

class LabelControl : public Control
{
public:
	LabelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);
};