#include "ContainerControl.h"

ContainerControl::ContainerControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "Container", name, x, y, width, height, "empty.png", parent)
{
	this->SetRenderOrder(0);
}