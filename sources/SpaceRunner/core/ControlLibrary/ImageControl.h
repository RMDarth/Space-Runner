#pragma once
#include "Control.h"

class ImageControl : public Control
{
public:
	ImageControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	virtual void SetImage(std::string image);
	virtual void SetCustomAttribute(std::string name, std::string value);
	
	virtual std::string GetCustomAttribute(std::string name);
};