#include <Game/StateProcessors/Level/LevelManager.h>
#include <Game/SkinManager.h>
#include <Game/Scores.h>
#include "ChallengesStateProcessor.h"
#include "Game/Game.h"
using namespace std;

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
        UpdateHUD();
    }

    void ChallengesStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "back")
        {
            Game::Instance()->ChangeState(GameState::Storyboard);
            return;
        }
        else if (control->GetName() == "challengedaily" || control->GetName() == "challengetime")
        {
            LevelManager::Instance()->SetLevelTypes(LevelType::Rush);
            LevelManager::Instance()->SetLevelNum(1);
            LevelManager::Instance()->SetLives(SkinManager::Instance()->LivesCount());
            LevelManager::Instance()->ResetMovies();
            Game::Instance()->ChangeState(GameState::ShipSelect);
        }
        else if (control->GetClassType() == "Button")
        {
            int levelNum = stoi(control->GetName());

            LevelManager::Instance()->SetStarted(false);
            LevelManager::Instance()->SetLives(0);
            LevelManager::Instance()->ResetMovies();
            LevelManager::Instance()->SetLevelTypes(LevelType::Challenge);
            LevelManager::Instance()->SetLevelNum(levelNum);
            Game::Instance()->ChangeState(GameState::Level);
        }
    }

    void ChallengesStateProcessor::UpdateHUD()
    {
        for (auto i = 1; i <= 6; i++)
        {
            if (Scores::Instance()->IsChallengeCompleted(i))
            {
                _document->GetControlByName(to_string(i))->SetText("Completed");
                _document->GetControlByName(to_string(i))->SetCustomAttribute("font-color", "0.9 0.88 0.4 0.8");
            }
        }

        std::string controlNames[2] = { "challengedaily", "challengetime" };

        for (auto i = 0; i < 2; i++)
        {
            if (Scores::Instance()->IsDailyChallengeCompleted(i))
            {
                _document->GetControlByName(controlNames[i])->SetText("Completed");
                _document->GetControlByName(controlNames[i])->SetCustomAttribute("font-color", "0.9 0.88 0.4 0.8");
            }
        }
    }


}