#include "SelectShipStateProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"

namespace CoreEngine
{

    SelectShipStateProcessor::SelectShipStateProcessor()
    {
        _document = new ControlDocument("GUI/selectship.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _document->GetControlByName("panel")->SetRenderOrder(1);
    }


    SelectShipStateProcessor::~SelectShipStateProcessor()
    {
    }

    GameState::State SelectShipStateProcessor::Update(float time)
    {
        return GameState::ShipSelect;
    }

    void SelectShipStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void SelectShipStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void SelectShipStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void SelectShipStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (key == OIS::KC_R)
        {
            LevelManager::Instance()->SetStarted(false);
            Game::Instance()->ChangeState(GameState::Level);
        }
    }


    void SelectShipStateProcessor::Hide()
    {
        _document->Hide();
    }

    void SelectShipStateProcessor::Show()
    {
        _document->Show();
    }

    void SelectShipStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "start")
        {
            LevelManager::Instance()->SetStarted(false);
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (control->GetName() == "restart")
        {
            LevelManager::Instance()->SetStarted(false);
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (control->GetName() == "back")
        {
            LevelManager::Instance()->SetStarted(false);
            if (LevelManager::Instance()->IsPuzzle())
            {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
            else {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
        }
    }

    bool SelectShipStateProcessor::IsOverlapping()
    {
        return Game::Instance()->GetState() != GameState::MainMenu;
    }


}