#pragma once
#include <string>
#include "Input/IInputListener.h"
#include <memory>
#include <vector>
class SceneSector;
class Control;

namespace Ogre
{
	class ManualObject;
	class Rectangle2D;
	class Overlay;
	class OverlayElement;
	class OverlayContainer;
}



class IEventHandler
{
public:
	enum EventType
	{
		MouseUp,
		MouseDown,
		MouseMove
	};

	virtual void ProcessEvent(Control* control, EventType type, int x, int y) = 0; 
};

class Control : IInputListener
{
public:
	enum RenderType
	{
		Vertical, // 3 parts vertical
		Horizontal, // 3 parts horizontal
		Box, // 9 parts box
		Image // plane image
	};

protected:
	SceneSector* _sector;
	//Ogre::ManualObject* _manual;
	//Ogre::Rectangle2D *_rect;

	std::vector<std::shared_ptr<Control> > _children;
	std::shared_ptr<Control> _parent;

	// Text
	Ogre::Overlay* _overlay;
	Ogre::OverlayElement* _overlayText;
	Ogre::OverlayElement* _overlayTextShadow;
	Ogre::OverlayContainer* _container;

	static int _id;
	int _myID;

	std::string _type;
	std::string _name;

	std::string _defaultMaterial;
	std::string _pushMaterial;
	std::string _hoverMaterial;
	std::string _disabledMaterial;
	std::string _currentMaterial;
	bool _usePushMaterial;
	bool _useHoverMaterial;
	bool _useDisabledMaterial;


	RenderType _renderType;

	std::string _text;
	bool _useText;

	bool _visible;
	bool _enabled;

	bool _pressed;
	bool _mouseTransparent;

	int _x, _y;
	int _width, _height;

	float _diffuseColor[4];

	std::vector<IEventHandler*> _mouseDownHandlerList;
	std::vector<IEventHandler*> _mouseUpHandlerList;
	std::vector<IEventHandler*> _mouseMoveHandlerList;
public:
	Control(SceneSector* sector, std::string type, std::string name, float x, float y, float width, float height, std::string defaultImage, Control* parent = nullptr, RenderType renderType = RenderType::Image);
	virtual ~Control();

	virtual void SetDefaultMaterial(std::string image, bool create = true);
	virtual void SetPushMaterial(std::string image);
	virtual void SetHoverMaterial(std::string image);
	virtual void SetDisabledMaterial(std::string image);
	
	virtual void SetMaterial(std::string material);
	virtual void SetRenderOrder(int order);

	virtual void SetRenderType(RenderType type);
	virtual void SetText(std::string text);
	virtual void SetVisible(bool visible);
	virtual void SetEnabled(bool enabled);
	virtual void SetMouseTransparent(bool mouseTransparent);
	virtual void SetSize(int width, int height);
	virtual void SetPos(int x, int y);
	virtual void SetName(std::string name);
	virtual void SetCustomAttribute(std::string name, std::string value);
	virtual void SetDiffuseColor(float* rgbaColor);
	virtual void SetFontColor(float* rgbaColor);

	virtual RenderType GetRenderType();
	virtual std::string GetText();
	virtual bool IsVisible();
	virtual bool IsEnabled();
	virtual bool IsMouseTransparent();
	virtual void GetPos(int &x, int &y);
	virtual int GetWidth();
	virtual int GetHeight();
	virtual std::string GetName();
	virtual std::string GetCustomAttribute(std::string name);

	virtual std::string GetClassType();
	virtual std::shared_ptr<Control> GetParent();

	virtual void SetMouseDownHandler(IEventHandler* handler);
	virtual void SetMouseUpHandler(IEventHandler* handler);
	virtual void SetMouseMoveHandler(IEventHandler* handler);

	virtual bool OnMouseMove(int x, int y, float deltaTime);
	virtual bool OnMouseDown(int x, int y);
	virtual bool OnMouseUp(int x, int y);

	// children
	virtual void AddChild(Control* control);
	virtual std::shared_ptr<Control> GetChild(int id);
	virtual int GetChildrenCount();

private:
	void Init();
	

protected:
	void CreateMaterial(std::string image);
	bool IsInside(int x, int y);
	float ToRectCoords(int pos, int size);
	virtual void InitOverlay();
};

