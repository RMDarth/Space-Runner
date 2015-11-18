#include "PanelControl.h"

PanelControl::PanelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "Panel", name, x, y, width, height, "windows/untitled.png", parent)
{
	this->SetRenderOrder(0);
}