#include <RenderProcessor.h>
#include "SlidePanelControl.h"
#include <OgreOverlayContainer.h>

SlidePanelControl::SlidePanelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent)
        : Control(sector, "SlidePanel", name, x, y, width, height, "windows/untitled.png", parent)
{
    this->SetRenderOrder(0);
    _realWidth = _width;
    _originalPos = x;
    _curPos = x;
}

bool SlidePanelControl::OnMouseMove(int x, int y, float deltaTime)
{
    if (Control::OnMouseMove(x, y, deltaTime))
    {
        if (_moving)
        {
            _speed = (x - _prevX) * 0.5;
            _prevX = x;
            Move(x);
        }
        return true;
    }

    return false;
}

bool SlidePanelControl::OnMouseDown(int x, int y)
{
    if (Control::OnMouseDown(x, y))
    {
        _storedX = x;
        _prevX = x;
        _moving = true;
        _stopping = false;
        return true;
    }

    return false;
}

bool SlidePanelControl::OnMouseUp(int x, int y)
{
    if (Control::OnMouseUp(x, y))
    {
        if (_moving)
        {
            _stopping = true;
            _stoppingTime = 0;
            _storedX = 0;
        }

        _moving = false;
        _curPos = _x;

        return true;
    }

    return false;
}

std::string SlidePanelControl::GetCustomAttribute(std::string name)
{
    if (name == "realwidth")
    {
        char str[20];
        sprintf(str, "%d", _realWidth);
        return std::string(str);
    }

    return Control::GetCustomAttribute(name);
}

void SlidePanelControl::SetCustomAttribute(std::string name, std::string value)
{
    if (name == "realwidth")
    {
        int c_x, c_y, c_width, c_height;
        if (_parent == nullptr)
        {
            RenderProcessor::Instance()->GetViewport()->getActualDimensions(c_x, c_y, c_width, c_height);
            c_x = 0; c_y = 0;
        }
        else {
            _parent->GetPos(c_x, c_y);
            c_width = _parent->GetWidth();
            c_height = _parent->GetHeight();
        }

        float width = (float)atof(value.c_str());
        _realWidth = (int)(width * c_width);
    }
    else
    {
        Control::SetCustomAttribute(name, value);
    }
}

void SlidePanelControl::Update(float time)
{
    _stoppingTime += time;
    Control::Update(time);

    if (_stopping)
    {
        if (_stoppingTime > 0.3f)
        {
            _stopping = false;
        }
        else
        {
            Move((1.0f - _stoppingTime) * _speed);
            _curPos = _x;
        }
    }
}

void SlidePanelControl::Move(int x)
{
    int newX = _curPos + x - _storedX;
    if (-newX + _realWidth > _width)
        newX = _realWidth - _width;
    if (newX > _originalPos)
        newX = _originalPos;

    auto oldX = _x;
    _x = newX;
    _container->setPosition((float)newX, (float)_y);

    for (auto i = 0; i<_children.size(); i++)
    {
        _children[i]->ParentMoved(oldX, _y);
    }
}













