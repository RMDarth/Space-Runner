#include "ControlDocument.h"
#include "ControlFactory.h"
#include "Render/SceneSector.h"
#include "RenderProcessor.h"
#include "../tinyxml/tinyxml2.h"
#include "FileSystem.h"
#include <functional>
#include <algorithm>
using namespace std;
using namespace tinyxml2;

ControlDocument::ControlDocument(std::string filename)
{
	LoadDocument(filename);
}

ControlDocument::~ControlDocument()
{
	delete _sector;
	_controlList.clear();
}

void ControlDocument::LoadDocument(std::string filename)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	FileInputRef * ref = new FileInputRef(filename);
	string content = ref->ReadToEnd();
	ref->Close();

	auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
	auto sceneNode = sceneManager->createSceneNode();
	sceneManager->getRootSceneNode()->addChild(sceneNode);
	_sector = new SceneSector(sceneNode);

	if (doc->Parse(content.c_str()) == XML_NO_ERROR)
	{
		XMLElement* element = doc->FirstChildElement("document");
	
		Control* parent = ControlFactory::Instance()->CreateControl(
				_sector,
				"Container",
				"document",
				0.0f, 0.0f, 1.0f, 1.0f, nullptr);

		AddChildren(parent, element);
	}

	delete ref;
	delete doc;
}

void ControlDocument::SetCustomAttributes(Control * control, tinyxml2::XMLElement* element)
{
	for (auto attribute = element->FirstAttribute(); attribute != 0; attribute = attribute->Next())
	{
		control->SetCustomAttribute(attribute->Name(), attribute->Value());
	}
}

void ControlDocument::AddChildren(Control* parent, tinyxml2::XMLElement* element)
{
	std::string alignment;
	
	float next_x = 0;
	float next_y = 0;
	float padding = 0.05f;

	for (XMLElement* control = element->FirstChildElement();
				control != 0; control = control->NextSiblingElement())
	{
		float x;
		float y;

		if (control->Attribute("padding") != nullptr)
		{
			padding = control->FloatAttribute("padding");
		} else {
			padding = 0.05f;
		}

		if (control->Attribute("x") != nullptr)
		{
			x = control->FloatAttribute("x");
		} else {
			x = next_x;
		}
		
		if (control->Attribute("y") != nullptr)
		{
			y = control->FloatAttribute("y");
		} else {
			y = next_y;
		}

		float width = control->FloatAttribute("width");
		float height = control->FloatAttribute("height");

		next_x += width + padding;

		if (control->Attribute("alignment") != nullptr)
		{
			alignment = control->Attribute("alignment");
			if (alignment == "center")
			{
				int p_width = parent->GetWidth();
				int p_height = parent->GetHeight();
					
				x = (((float)p_width * 0.5f) - (width * p_width * 0.5f)) / (float)p_width;

				next_x = 0;
				next_y += height + padding;
			}
		}

		Control* guiControl = ControlFactory::Instance()->CreateControl(
			_sector,
			control->Name(),
			control->Attribute("name"),
			x,	y,	width,	height, parent);

		if (control->Attribute("text") != nullptr)
		{
			guiControl->SetText(control->Attribute("text"));
		}

		if (control->Attribute("mousetransparent") != nullptr)
		{
			guiControl->SetMouseTransparent(control->BoolAttribute("mousetransparent"));
		}

		SetCustomAttributes(guiControl, control);

		if (guiControl != nullptr)
		{
			parent->AddChild(guiControl);
			_controlList.push_back(shared_ptr<Control>(guiControl));
		}	

		AddChildren(guiControl, control);
	}
}

void ControlDocument::Show()
{
	//_sector->GetNode()->setVisible(true);
	for_each(_controlList.begin(), _controlList.end(), bind(&Control::SetVisible, placeholders::_1, true));
}

void ControlDocument::Hide()
{
	//_sector->GetNode()->setVisible(false);
	for_each(_controlList.begin(), _controlList.end(), bind(&Control::SetVisible, placeholders::_1, false));
}

string ControlDocument::GetName()
{
	return _name;
}

void ControlDocument::SetMouseDownHandler(IEventHandler* handler)
{
	_mouseMoveHandler = handler;
	for_each(_controlList.begin(), _controlList.end(), bind(&Control::SetMouseDownHandler, placeholders::_1, handler));
}

void ControlDocument::SetMouseUpHandler(IEventHandler* handler)
{
	_mouseUpHandler = handler;
	for_each(_controlList.begin(), _controlList.end(), bind(&Control::SetMouseUpHandler, placeholders::_1, handler));
}

void ControlDocument::SetMouseMoveHandler(IEventHandler* handler)
{
	_mouseMoveHandler = handler;
	for_each(_controlList.begin(), _controlList.end(), bind(&Control::SetMouseMoveHandler, placeholders::_1, handler));
}

void ControlDocument::AddControl(Control* control)
{
	_controlList.push_back(shared_ptr<Control>(control));
}

std::shared_ptr<Control> ControlDocument::GetControlByName(std::string name)
{
	for (auto i = _controlList.begin(); i != _controlList.end(); i++)
	{
		if ((*i)->GetName() == name)
		{
			return *i;
		}
	}

	return nullptr;
}

bool ControlDocument::OnMouseMove(int x, int y, float deltaTime)
{
	bool result = false;
	for (auto i = _controlList.begin(); i != _controlList.end(); i++)
	{
		result = result | (*i)->OnMouseMove(x, y, deltaTime);
	}

	return result;
}

bool ControlDocument::OnMouseDown(int x, int y)
{
	bool result = false;
	for (auto i = _controlList.begin(); i != _controlList.end(); i++)
	{
		result = result | (*i)->OnMouseDown(x, y);
	}

	return result;
}

bool ControlDocument::OnMouseUp(int x, int y)
{
	bool result = false;
	for (auto i = _controlList.begin(); i != _controlList.end(); i++)
	{
		result = result | (*i)->OnMouseUp(x, y);
	}

	return result;
}