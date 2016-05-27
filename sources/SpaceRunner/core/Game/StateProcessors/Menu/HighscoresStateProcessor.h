
#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

namespace CoreEngine
{
    class HighscoresStateProcessor : public IStateProcessor, public IEventHandler
    {
        ControlDocument * _document;
        bool _isWeekly = true;
        bool _logged;

    public:
        HighscoresStateProcessor();
        ~HighscoresStateProcessor();

        virtual GameState::State Update(float time);

        virtual void OnMouseDown(int x, int y);
        virtual void OnMouseUp(int x, int y);
        virtual void OnMouseMove(int x, int y);
        virtual void OnKeyPressed(OIS::KeyCode key);

        virtual void Hide();
        virtual void Show();
        virtual bool IsOverlapping() { return false; }

        // IEventProcessor interface //
        virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

    private:
        void UpdateBoard();
    };
}