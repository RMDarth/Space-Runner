#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

namespace CoreEngine
{
    class SelectShipStateProcessor : public IStateProcessor, public IEventHandler
    {
        ControlDocument * _document;

    public:
        SelectShipStateProcessor();
        ~SelectShipStateProcessor();

        virtual GameState::State Update(float time);
        virtual void OnMouseDown(int x, int y);
        virtual void OnMouseUp(int x, int y);
        virtual void OnMouseMove(int x, int y);
        virtual void OnKeyPressed(OIS::KeyCode key);

        virtual void Hide();
        virtual void Show();
        virtual bool IsOverlapping();

        // IEventProcessor interface //
        virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);
    };
}

