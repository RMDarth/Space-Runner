#include "LabelControl.h"

LabelControl::LabelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "Label", name, x, y, width, height, "empty.png", parent)
{

}