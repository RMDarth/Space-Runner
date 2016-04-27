#pragma once
#include "Control.h"
#include <vector>
#include <memory>

class SceneSector;

namespace tinyxml2
{
	class XMLElement;
}

class ControlDocument : IInputListener
{
	std::vector<std::shared_ptr<Control>> _controlList;
	std::string _name;

	SceneSector *_sector;

	IEventHandler *_mouseDownHandler;
	IEventHandler *_mouseUpHandler;
	IEventHandler *_mouseMoveHandler;

public:
	ControlDocument(std::string filename);
	virtual ~ControlDocument();

	void LoadDocument(std::string filename);

	void Show();
	void Hide();

	std::string GetName();

	void SetMouseDownHandler(IEventHandler* handler);
	void SetMouseUpHandler(IEventHandler* handler);
	void SetMouseMoveHandler(IEventHandler* handler);

	void AddControl(Control* control);
	std::shared_ptr<Control> GetControlByName(std::string name);

	virtual bool OnMouseMove(int x, int y, float deltaTime);
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseUp(int x, int y);

	void RaisePriority(int level);

private:
	void AddChildren(Control* parent, tinyxml2::XMLElement* element);
	void SetCustomAttributes(Control * control, tinyxml2::XMLElement* element);
};