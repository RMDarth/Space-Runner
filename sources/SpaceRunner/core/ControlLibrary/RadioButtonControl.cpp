#include "RadioButtonControl.h"
#include "OgreOverlayElement.h"
#include "RenderProcessor.h"


RadioButtonControl::RadioButtonControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
	: Control(sector, "RadioButton", name, x, y, width, -1, "check_radio/radiobutton_clear_norm.png", parent)
{
	_checkedMaterial = "check_radio/radiobutton_norm.png";
	_uncheckedMaterial = _defaultMaterial;

	CreateMaterial("check_radio/radiobutton_norm.png");
	this->SetHoverMaterial("check_radio/radiobutton_float.png");
	_useHoverMaterial = false;

	_checked = false;

	this->SetPushMaterial("check_radio/radiobutton_down.png");

	int c_x, c_y, c_width, c_height;
	RenderProcessor::Instance()->GetViewport()->getActualDimensions(c_x, c_y, c_width, c_height);

	_overlayText->setParameter("alignment", "left");
	_overlayText->setParameter("horz_align", "left");
	_overlayText->setPosition(_width / (float)c_width + 0.005f, -0.025f);

	_overlayTextShadow->setParameter("alignment", "left");
	_overlayTextShadow->setParameter("horz_align", "left");
	_overlayTextShadow->setPosition(_width / (float)c_width + 0.007f, -0.022f);
}

bool RadioButtonControl::OnMouseUp(int x, int y)
{
	if (IsInside(x, y))
	{
		if (_pressed)
		{
			_checked = true;
			UpdateState();
		}
	}

	return Control::OnMouseUp(x, y);
}

bool RadioButtonControl::IsChecked()
{
	return _checked; 
}

std::string RadioButtonControl::GetCustomAttribute(std::string name)
{
	if (name == "checked")
		return _checked ? "true" : "false";

	return Control::GetCustomAttribute(name);
}

void RadioButtonControl::SetChecked(bool value)
{
	_checked = value;
	UpdateState();
}

void RadioButtonControl::UpdateState()
{
	if (_checked)
	{
		_defaultMaterial = _checkedMaterial;
		_useHoverMaterial = true;
	} else {
		_defaultMaterial = _uncheckedMaterial;
		_useHoverMaterial = false;
	}

	SetMaterial(_defaultMaterial);
}