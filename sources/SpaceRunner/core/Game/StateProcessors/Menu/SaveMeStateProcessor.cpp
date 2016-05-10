#include <Game/Config.h>
#include <Game/Scores.h>
#include <BillingProcessor.h>
#include "SaveMeStateProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"
#include "SoundSystem.h"

namespace CoreEngine
{

    SaveMeStateProcessor::SaveMeStateProcessor()
    {
        _document = new ControlDocument("GUI/saveme.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _query = new ControlDocument("GUI/resurrectquery.xml");
        _query->SetMouseUpHandler(this);
        _query->Hide();

        _query->RaisePriority(6);

        _queryVisible = false;
    }


    SaveMeStateProcessor::~SaveMeStateProcessor()
    {
    }

    GameState::State SaveMeStateProcessor::Update(float time)
    {
        return GameState::SaveMe;
    }

    void SaveMeStateProcessor::OnMouseDown(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseDown(x, y);
        else
            _document->OnMouseDown(x, y);
    }

    void SaveMeStateProcessor::OnMouseUp(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseUp(x, y);
        else
            _document->OnMouseUp(x, y);
    }

    void SaveMeStateProcessor::OnMouseMove(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseMove(x, y, 0);
        else
            _document->OnMouseMove(x, y, 0);
    }

    void SaveMeStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (key == OIS::KC_R)
        {
            LevelManager::Instance()->SetStarted(false);
            LevelManager::Instance()->SetLives(1);
            LevelManager::Instance()->ResetMovies();
            Game::Instance()->ChangeState(GameState::Level);
        }
    }


    void SaveMeStateProcessor::Hide()
    {
        _document->Hide();
    }

    void SaveMeStateProcessor::Show()
    {
        using namespace std;
        _document->Show();
        _queryVisible = false;

        if (LevelManager::Instance()->IsMoviesLeft())
        {
            _document->GetControlByName("reviveforads")->SetVisibleRecursive(true);
            _document->GetControlByName("reviveforitem")->SetVisibleRecursive(true);
            _document->GetControlByName("reviveforitem2")->SetVisibleRecursive(false);
            _document->GetControlByName("resurrectcount")->SetText(to_string(Config::Instance()->GetResurrectCount()));
        } else {
            _document->GetControlByName("reviveforads")->SetVisibleRecursive(false);
            _document->GetControlByName("reviveforitem")->SetVisibleRecursive(false);
            _document->GetControlByName("reviveforitem2")->SetVisibleRecursive(true);
            _document->GetControlByName("resurrectcount2")->SetText(to_string(Config::Instance()->GetResurrectCount()));
        }
    }

    void SaveMeStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "back")
        {
            if (!LevelManager::Instance()->IsPuzzle()
                && LevelManager::Instance()->GetScore() >= 100)
            {
                if (Config::Instance()->IsSoundEnabled())
                    _successSound->Play();
                LevelManager::Instance()->SetVictory(true);
            }

            _document->Hide();
            Game::Instance()->ChangeState(GameState::Score);
        }
        if (control->GetName() == "reviveforads")
        {
            LevelManager::Instance()->SetStarted(true);
            LevelManager::Instance()->IncreaseLive();
            LevelManager::Instance()->DecreaseMovies();
            // TODO: Run video ads here //
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (control->GetName() == "reviveforitem" || control->GetName() == "reviveforitem2")
        {
            if (Config::Instance()->GetResurrectCount() > 0)
            {
                Config::Instance()->SetResurrectCount(Config::Instance()->GetResurrectCount() - 1);
                LevelManager::Instance()->SetStarted(true);
                LevelManager::Instance()->IncreaseLive();
                Game::Instance()->ChangeState(GameState::Level);
            } else {
                if (Scores::Instance()->GetTotalEnergy() + LevelManager::Instance()->GetScore() >= 5000)
                    _query->GetControlByName("buy")->SetText("Buy for §5000");
                else
                    _query->GetControlByName("buy")->SetText("Buy for $2.49");
                _query->Show();
                _queryVisible = true;

                _document->GetControlByName("back")->SetVisible(false);
            }
        }

        if (control->GetName()=="return")
        {
            _query->Hide();
            _queryVisible = false;

            _document->GetControlByName("back")->SetVisible(true);
        }
        if (control->GetName()=="buy")
        {
            _query->Hide();
            _queryVisible = false;

            if (Scores::Instance()->GetTotalEnergy() + LevelManager::Instance()->GetScore() >= 5000)
            {
                int totalEnergy = Scores::Instance()->GetTotalEnergy();
                if (totalEnergy < 5000)
                {
                    LevelManager::Instance()->SetScore(LevelManager::Instance()->GetScore() - (5000 - totalEnergy));
                    Scores::Instance()->UpdateTotalEnergy(-totalEnergy);
                } else {
                    Scores::Instance()->UpdateTotalEnergy(-5000);
                }
                Config::Instance()->SetResurrectCount(1);
            } else {
                BillingProcessor::Instance()->BuyItem("ResurrectItem");
                if (BillingProcessor::Instance()->IsItemBought("ResurrectItem"))
                {
                    Config::Instance()->SetResurrectCount(1);
                }
            }

            using namespace std;
            if (LevelManager::Instance()->IsMoviesLeft())
                _document->GetControlByName("resurrectcount")->SetText(to_string(Config::Instance()->GetResurrectCount()));
            else
                _document->GetControlByName("resurrectcount2")->SetText(to_string(Config::Instance()->GetResurrectCount()));
        }
    }
}