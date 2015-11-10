#include "ControlFactory.h"
#include "ButtonControl.h"
#include "PanelControl.h"
#include "LabelControl.h"
#include "RadioButtonControl.h"
#include "RadioButtonGroup.h"
#include "ImageControl.h"
#include "ContainerControl.h"
#include "LevelButtonControl.h"
#include "SwipeBarControl.h"

ControlFactory* ControlFactory::_instance = nullptr;

ControlFactory* ControlFactory::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new ControlFactory();
	}
	return _instance;
}

ControlFactory::ControlFactory()
{

}

Control* ControlFactory::CreateControl(SceneSector* sector, std::string type, std::string name, float x, float y, float width, float height, Control * parent)
{
	if (type == "Button")
	{
		return new ButtonControl(sector, name, x, y, width, height, parent);
	}
	if (type == "Label")
	{
		return new LabelControl(sector, name, x, y, width, height, parent);
	}
	if (type == "Panel")
	{
		return new PanelControl(sector, name, x, y, width, height, parent);
	}
	if (type == "RadioButton")
	{
		return new RadioButtonControl(sector, name, x, y, width, height, parent);
	}
	if (type == "RadioGroup")
	{
		return new RadioButtonGroup(sector, name, parent);
	}
	if (type == "Image")
	{
		return new ImageControl(sector, name, x, y, width, height, parent);
	}
	if (type == "Container")
	{
		return new ContainerControl(sector, name, x, y, width, height, parent);
	}
	if (type == "LevelButton")
	{
		return new LevelButtonControl(sector, name, x, y, width, height, parent);
	}
	if (type == "SwipeBar")
	{
		return new SwipeBarControl(sector, name, x, y, width, height, parent);
	}

	return nullptr;
}