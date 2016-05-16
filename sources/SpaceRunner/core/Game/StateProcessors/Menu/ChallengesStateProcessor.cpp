#include "ChallengesStateProcessor.h"
#include "Game/Game.h"

namespace CoreEngine
{
    ChallengesStateProcessor::ChallengesStateProcessor()
    {
        _document = new ControlDocument("GUI/challenges.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _document->RaisePriority(4);
    }


    ChallengesStateProcessor::~ChallengesStateProcessor()
    {
        delete _document;
    }

    GameState::State ChallengesStateProcessor::Update(float time)
    {
        return GameState::Challenges;
    }

    void ChallengesStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void ChallengesStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void ChallengesStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void ChallengesStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
        }
    }


    void ChallengesStateProcessor::Hide()
    {
        _document->Hide();
    }

    void ChallengesStateProcessor::Show()
    {
        _document->Show();
    }

    void ChallengesStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "back")
        {
            Game::Instance()->ChangeState(GameState::Storyboard);
            return;
        }
    }
}