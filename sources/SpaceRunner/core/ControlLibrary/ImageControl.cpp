#include "ImageControl.h"

ImageControl::ImageControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent, Control * parent2)
	: Control(sector, "Image", name, x, y, width, height, "empty.png", parent, RenderType::Image, 30, parent2)
{

}

void ImageControl::SetImage(std::string image)
{
	CreateMaterial(image);
	_defaultMaterial = image;
	SetMaterial(image);
}

void ImageControl::SetCustomAttribute(std::string name, std::string value)
{
	if (name == "source")
	{
		SetImage(value);
	} 
	Control::SetCustomAttribute(name, value);
}

std::string ImageControl::GetCustomAttribute(std::string name)
{
	if (name == "source")
		return _defaultMaterial;

	return Control::GetCustomAttribute(name);
}