#include "Control.h"
#include "../PrecompileHeaders.h"
#include "OgreOverlay.h"
#include "OgreOverlayContainer.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"
#include "OgreRectangle2D.h"
#include "Render/SceneSector.h"
#include <sstream>

int Control::_id = 0;

Control::Control(SceneSector* sector,
                 std::string type,
                 std::string name,
                 float x,
                 float y,
                 float width,
                 float height,
                 std::string defaultImage,
                 Control* parent,
                 Control::RenderType renderType,
                 float fontSize)
{
    _sector = sector;
    _type = type;
    _fontSize = fontSize;

    int c_x, c_y, c_width, c_height;
    if (parent == nullptr)
    {
        RenderProcessor::Instance()->GetViewport()->getActualDimensions(c_x, c_y, c_width, c_height);
        c_x = 0; c_y = 0;
    }
    else {
        parent->GetPos(c_x, c_y);
        c_width = parent->GetWidth();
        c_height = parent->GetHeight();
    }

    _x = c_x + (int)(x * c_width);
    _y = c_y + (int)(y * c_height);
    _width = (int)(width * c_width);
    _height = (int)(height * c_height);

    if (height < 0)
        _height = (int)(-_width * height);
    if (width < 0)
        _width = (int)(-_height * width);

    _name = name;
    _parent = std::shared_ptr<Control>(parent);

    CreateMaterial(defaultImage);
    _defaultMaterial = defaultImage;
    _currentMaterial = _defaultMaterial;
    _renderType = renderType;

    _useDisabledMaterial = false;
    _useHoverMaterial = false;
    _usePushMaterial = false;
    _useText = false;
    _pressed = false;
    _mouseTransparent = false;

    _visible = true;
    _enabled = true;

    _myID = _id;
    _id++;

    _diffuseColor[0] = 1; _diffuseColor[1] = 1; _diffuseColor[2] = 1; _diffuseColor[3] = 1;

    _overlay =  nullptr;
    InitOverlay();
    SetRenderOrder(1);
}

void Control::ParentMoved(int oldX, int oldY)
{
    int myOldX = _x;
    int myOldY = _y;

    if (!_parent)
        return;
    int c_x, c_y;
    _parent->GetPos(c_x, c_y);

    auto shiftX = _x - oldX;
    auto shiftY = _y - oldY;
    _x = c_x + shiftX;
    _y = c_y + shiftY;

    _container->setPosition((float)_x, (float)_y);

    for (auto i = 0; i < _children.size(); i++)
    {
        _children[i]->ParentMoved(myOldX, myOldY);
    }
}

Control::~Control()
{
    //delete overlay
    if (_overlay != nullptr)
    {
        std::stringstream stream;
        stream << "Text_" << _myID;
        _overlay->hide();
        Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
        _container->removeChild(stream.str());
        _overlay->remove2D(_container);
        overlayManager->destroyOverlayElement(_overlayText);
        overlayManager->destroyOverlayElement(_overlayTextShadow);
        overlayManager->destroyOverlayElement(_container);
        overlayManager->destroy(_overlay);
    }
}

float Control::ToRectCoords(int pos, int size)
{
    return ((float)pos / size ) * 2.0f - 1.0f;
}

void Control::Init()
{

}

void Control::InitOverlay()
{
    int zorder = -1000;
    std::stringstream stream;
    int left, top, width, height;
    RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);

    stream << "Overlay_" << _myID;
    auto overlayManager = Ogre::OverlayManager::getSingletonPtr();
    if (_overlay != nullptr)
    {
        zorder = _overlay->getZOrder();
        _overlay->remove2D(_container);
        overlayManager->destroyOverlayElement(_overlayText);
        overlayManager->destroyOverlayElement(_overlayTextShadow);
        overlayManager->destroyOverlayElement(_container);
        overlayManager->destroy(_overlay);
    }
    _overlay = overlayManager->create(stream.str());
    stream.str("Panel_");
    stream << _myID;
    _container = static_cast<Ogre::OverlayContainer*>(overlayManager->createOverlayElement("Panel", stream.str()));

    _overlay->add2D(_container);

    _container->setMetricsMode(Ogre::GMM_PIXELS);
    _container->setPosition((float)_x, (float)_y);
    _container->setDimensions((float)_width, (float)_height);
    _container->setMaterialName(_defaultMaterial);
 
    stream.str("Text_");
    stream << _myID;
    _overlayText = overlayManager->createOverlayElement("TextArea", stream.str());


    float fontSize = _fontSize / 400.0f;

    _overlayText->setMetricsMode(Ogre::GMM_RELATIVE);
    _overlayText->setPosition(0, -fontSize/2);


    stream.str("");
    stream << _fontSize / 400.0f;
    std::string fontSizeStr = stream.str();

    _overlayText->setParameter("font_name", "GUIfont");
    _overlayText->setParameter("char_height", fontSizeStr);
    _overlayText->setParameter("horz_align", "center");
    _overlayText->setParameter("vert_align", "center");
    if (_width > 0)
        _overlayText->setParameter("alignment", "center");
    else
        _overlayText->setParameter("alignment", "left");

    _overlayText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));
    _container->addChild(_overlayText);

    // init shadow
    stream.str("Text_");
    stream << _myID << "_shadow";
    _overlayTextShadow = overlayManager->createOverlayElement("TextArea", stream.str());

    _overlayTextShadow->setMetricsMode(Ogre::GMM_RELATIVE);
    _overlayTextShadow->setPosition(0.002f, -fontSize/2 + 0.0025f);
    _overlayTextShadow->setParameter("font_name", "GUIfont");
    _overlayTextShadow->setParameter("char_height", fontSizeStr);
    _overlayTextShadow->setParameter("horz_align", "center");
    _overlayTextShadow->setParameter("vert_align", "center");
    if (_width > 0)
        _overlayTextShadow->setParameter("alignment", "center");
    else
        _overlayTextShadow->setParameter("alignment", "left");

    _overlayTextShadow->setColour(Ogre::ColourValue(0.0, 0.0, 0.0));
    _container->addChild(_overlayTextShadow);

    _useText = true;

    if (zorder != -1000)
    {
        _overlay->setZOrder(zorder);
    }
}

void Control::CreateMaterial(std::string image)
{
    if (!Ogre::MaterialManager::getSingleton().getByName(image, "General").isNull())
        return;
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(image, "General");
    material->getTechnique(0)->getPass(0)->setVertexProgram("simpletex_vs");
    material->getTechnique(0)->getPass(0)->setFragmentProgram("simpletex_ps");
    auto state = material->getTechnique(0)->getPass(0)->createTextureUnitState(image);
    state->setTextureNameAlias("diffuseMap");
    state->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
    //material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
}

void Control::SetRenderOrder(int order)
{
    _overlay->setZOrder(order);
    //_rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - order);
}

int Control::GetRenderOrder()
{
    return _overlay->getZOrder();
}

void Control::SetPushMaterial(std::string image)
{
    _usePushMaterial = true;
    CreateMaterial(image);
    _pushMaterial = image;
}

void Control::SetHoverMaterial(std::string image)
{
    _useHoverMaterial = true;
    CreateMaterial(image);
    _hoverMaterial = image;
}

void Control::SetDisabledMaterial(std::string image)
{
    _useDisabledMaterial = true;
    CreateMaterial(image);
    _disabledMaterial = image;
}

void Control::SetDefaultMaterial(std::string image, bool create)
{
    if (create)
        CreateMaterial(image);
    _defaultMaterial = image;
    SetMaterial(image);
}

void Control::SetMaterial(std::string material)
{
    if (material != _currentMaterial)
    {
        _container->setMaterialName(material);
        _currentMaterial = material;
    }
}

void Control::SetDiffuseColor(float* rgbaColor)
{
    for (int i = 0; i < 4; i++)
        _diffuseColor[i] = rgbaColor[i];

    std::stringstream str;
    str << " " << _diffuseColor[0] << " " << _diffuseColor[1] << " " << _diffuseColor[2] << " " << _diffuseColor[3];
    auto materialName = _defaultMaterial + str.str();
    if (Ogre::MaterialManager::getSingleton().getByName(materialName).isNull())
    {
        auto newMaterial = Ogre::MaterialManager::getSingleton().create(materialName, "General");
        Ogre::MaterialManager::getSingleton().getByName(_defaultMaterial)->copyDetailsTo(newMaterial);
        newMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters()->setNamedConstant("color",
                                                                                                      _diffuseColor, 1,
                                                                                                      4);
    }
    _defaultMaterial = materialName;

    SetMaterial(_defaultMaterial);
}

void Control::SetFontColor(float* rgbaColor)
{
    _overlayText->setColour(Ogre::ColourValue(rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]));
}

void Control::SetFontSize(float size)
{
    _fontSize = size;
    InitOverlay();
    _overlayText->setCaption(_text);
    _overlayTextShadow->setCaption(_text);
    _overlay->show();
}

void Control::SetRenderType(RenderType type)
{
    _renderType = type;
    Init();
}

void Control::SetText(std::string text)
{
    _text = text;
    _useText = true;

    _overlayText->setCaption(text);
    _overlayTextShadow->setCaption(text);
    _overlay->show();
}

void Control::SetVisible(bool visible)
{
    if (_useText)
    {
        if (visible)
            _overlay->show();
        else
            _overlay->hide();
        _visible = visible;
    }
}

void Control::SetVisibleRecursive(bool visible)
{
    SetVisible(visible);
    for (auto &child : _children)
    {
        child->SetVisibleRecursive(visible);
    }
}

void Control::SetCustomAttribute(std::string name, std::string value)
{
    if (name == "hoverimage")
    {
        SetHoverMaterial(value);
    }
    if (name == "pressedimage")
    {
        SetPushMaterial(value);
    }
    if (name == "singleimage" && value == "true")
    {
        _useHoverMaterial = false;
        _usePushMaterial = false;
        _useDisabledMaterial = false;
    }
    if (name == "image")
    {
        SetDefaultMaterial(value);
    }
    if (name == "background-color")
    {
        std::stringstream str(value);
        float color[4];
        str >> color[0] >> color[1] >> color[2] >> color[3];
        SetDiffuseColor(color);
    }
    if (name == "font-color")
    {
        std::stringstream str(value);
        float color[4];
        str >> color[0] >> color[1] >> color[2] >> color[3];
        SetFontColor(color);
    }
    if (name == "font-size")
    {
        std::stringstream str(value);
        float size;
        str >> size;
        SetFontSize(size);
    }
}

void Control::SetEnabled(bool enabled)
{
    _enabled = enabled;
    if (_useDisabledMaterial && !enabled)
    {
        SetMaterial(_disabledMaterial);
    }
}

void Control::SetMouseTransparent(bool mouseTransparent)
{
    _mouseTransparent = mouseTransparent;
}

void Control::SetSize(int width, int height)
{
    _width = width;
    _height = height;
    InitOverlay();
}

void Control::SetPos(int x, int y)
{
    _x = x;
    _y = y;
    InitOverlay();
}

void Control::SetName(std::string name)
{
    _name = name;
}


Control::RenderType Control::GetRenderType()
{
    return _renderType;
}

std::string Control::GetText()
{
    return _text;
}

bool Control::IsVisible()
{
    return _visible;
}

bool Control::IsEnabled()
{
    return _enabled;
}

bool Control::IsMouseTransparent()
{
    return _mouseTransparent;
}

void Control::GetPos(int &x, int &y)
{
    x = _x;
    y = _y;
}

int Control::GetWidth()
{
    return _width;
}

int Control::GetHeight()
{
    return _height;
}

std::string Control::GetName()
{
    return _name;
}

std::string Control::GetCustomAttribute(std::string name)
{
    return "";
}

std::string Control::GetClassType()
{
    return _type;
}

std::shared_ptr<Control> Control::GetParent()
{
    return _parent;
}

void Control::SetMouseDownHandler(IEventHandler* handler)
{
    _mouseDownHandlerList.push_back(handler);
}

void Control::SetMouseUpHandler(IEventHandler* handler)
{
    _mouseUpHandlerList.push_back(handler);
}

void Control::SetMouseMoveHandler(IEventHandler* handler)
{
    _mouseMoveHandlerList.push_back(handler);
}

bool Control::IsInside(int x, int y)
{
    if (x > _x && x < _x + _width)
    {
        if (y > _y && y < _y + _height)
        {
            return true;
        }
    }
    return false;
}

bool Control::OnMouseMove(int x, int y, float deltaTime)
{ 
    if (IsInside(x, y))
    {
        if (_useHoverMaterial && !_pressed)
        {
            SetMaterial(_hoverMaterial);
        }
        if (_mouseMoveHandlerList.size() != 0)
        {
            std::for_each(_mouseMoveHandlerList.begin(), _mouseMoveHandlerList.end(), std::bind(&IEventHandler::ProcessEvent, std::placeholders::_1, this, IEventHandler::MouseMove, x, y));
        }

        return true;
    } else {
        if (!_pressed)
        {
            SetMaterial(_defaultMaterial);
        }
    }

    return false;
}

bool Control::OnMouseDown(int x, int y)  
{ 
    if (IsInside(x, y) && _visible)
    {
        if (_usePushMaterial)
        {
            SetMaterial(_pushMaterial);
        }
        if (_mouseDownHandlerList.size() != 0)
        {
            std::for_each(_mouseDownHandlerList.begin(), _mouseDownHandlerList.end(), std::bind(&IEventHandler::ProcessEvent, std::placeholders::_1, this, IEventHandler::MouseDown, x, y));
        }
        _pressed = true;

        if (!_mouseTransparent)
            return true;
    }

    return false;
}

bool Control::OnMouseUp(int x, int y)  
{
    bool result = false;
    if (IsInside(x, y) && _visible)
    {
        SetMaterial(_defaultMaterial);
        if (_mouseUpHandlerList.size() != 0)
        {
            std::for_each(_mouseUpHandlerList.begin(), _mouseUpHandlerList.end(), std::bind(&IEventHandler::ProcessEvent, std::placeholders::_1, this, IEventHandler::MouseUp, x, y));
        }

        if (!_mouseTransparent)
            result = true;
    }
    _pressed = false;

    return result;
}

void Control::AddChild(Control* control)
{
    _children.push_back(std::shared_ptr<Control>(control));
    if (control->_parent == nullptr)
    {
        control->_parent = std::shared_ptr<Control>(this);
    }
}

std::shared_ptr<Control> Control::GetChild(int id)
{
    return _children[id];
}

int Control::GetChildrenCount()
{
    return _children.size();
}