#pragma once
#include "RadioButtonControl.h"
#include <memory>
#include <vector>

class RadioButtonGroup : public Control, public IEventHandler
{
	std::vector<std::shared_ptr<RadioButtonControl> > _buttonList;
public:
	RadioButtonGroup(SceneSector* sector, std::string name, Control * parent = nullptr);

	virtual void AddChild(Control* control);

	std::string SelectedValue();
	std::shared_ptr<RadioButtonControl> SelectedControl();

	virtual std::string GetCustomAttribute(std::string name);
	virtual void SetCustomAttribute(std::string name, std::string value);

	virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

	virtual int GetWidth();
	virtual int GetHeight();
};