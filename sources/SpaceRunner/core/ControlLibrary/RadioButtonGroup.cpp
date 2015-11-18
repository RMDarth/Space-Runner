#include "RadioButtonGroup.h"

RadioButtonGroup::RadioButtonGroup(SceneSector* sector, std::string name, Control * parent)
	: Control(sector, "RadioButtonGroup", name, 0, 0, 0, 0, "empty.png", parent)
{
	this->SetRenderOrder(0);
}

void RadioButtonGroup::AddChild(Control* control)
{
	_buttonList.push_back(std::shared_ptr<RadioButtonControl>((RadioButtonControl*)control));
	control->SetMouseUpHandler(this);

	Control::AddChild(control);
}

void RadioButtonGroup::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
{
	for (auto i = _buttonList.begin(); i != _buttonList.end(); i++)
	{
		if ((*i).get() != control)
		{
			(*i)->SetChecked(false);
		}
	}
}

std::string RadioButtonGroup::SelectedValue()
{
	for (auto i = _buttonList.begin(); i != _buttonList.end(); i++)
	{
		if ((*i)->IsChecked())
		{
			return (*i)->GetText();
		}
	}

	return "";
}

std::shared_ptr<RadioButtonControl> RadioButtonGroup::SelectedControl()
{
	for (auto i = _buttonList.begin(); i != _buttonList.end(); i++)
	{
		if ((*i)->IsChecked())
		{
			return (*i);
		}
	}

	return nullptr;
}

std::string RadioButtonGroup::GetCustomAttribute(std::string name)
{
	if (name == "SelectedValue")
		return SelectedValue();

	return Control::GetCustomAttribute(name);
}

void RadioButtonGroup::SetCustomAttribute(std::string name, std::string value)
{
	if (name == "SelectedValue")
	{
		for (auto i = _buttonList.begin(); i != _buttonList.end(); i++)
		{
			if ((*i).get()->GetText() != value)
			{
				(*i)->SetChecked(false);
			}
			else {
				(*i)->SetChecked(true);
			}
		}
	}
	else
	{
		Control::SetCustomAttribute(name, value);
	}
}

int RadioButtonGroup::GetWidth()
{
	if (_parent != nullptr)
	{
		return _parent->GetWidth();
	} else {
		return Control::GetWidth();
	}
}

int RadioButtonGroup::GetHeight()
{
	if (_parent != nullptr)
	{
		return _parent->GetHeight();
	} else {
		return Control::GetHeight();
	}
}