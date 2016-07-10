#include "HighscoresStateProcessor.h"
#include "Game/Game.h"
#include "BillingProcessor.h"
#include <string>
#include <Game/Scores.h>
#include <Game/Config.h>

namespace CoreEngine
{
    HighscoresStateProcessor::HighscoresStateProcessor()
    {
        _document = new ControlDocument("GUI/highscores.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _logged = false;
    }


    HighscoresStateProcessor::~HighscoresStateProcessor()
    {
        delete _document;
    }

    GameState::State HighscoresStateProcessor::Update(float time)
    {
        if (_logged != BillingProcessor::Instance()->IsLoggedGoogle())
        {
            _logged = BillingProcessor::Instance()->IsLoggedGoogle();
            if (_logged)
            {
                if (Config::Instance()->GetScoreSubmitted() > 0)
                {
                    BillingProcessor::Instance()->UpdateScore(Config::Instance()->GetScoreSubmitted());
                }
                Config::Instance()->SetScoreSubmitted(0);
            }
            UpdateBoard();
        }

        if (_logged)
        {
            if (BillingProcessor::Instance()->IsScoresUpdated())
            {
                UpdateBoard();
            }
        }

        return GameState::Highscores;
    }

    void HighscoresStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void HighscoresStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void HighscoresStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void HighscoresStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
        }
    }

    void HighscoresStateProcessor::Hide()
    {
        _document->Hide();
    }

    void HighscoresStateProcessor::Show()
    {
        _document->Show();
        _logged = BillingProcessor::Instance()->IsLoggedGoogle();
        if (_logged)
        {
            BillingProcessor::Instance()->SyncScores();
        }
        UpdateBoard();
    }

    void HighscoresStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "back")
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
        }
        else if (control->GetName() == "googleplay")
        {
            BillingProcessor::Instance()->ShowLeaderboard();
        }
        else if (control->GetName() == "googleplay2")
        {
            BillingProcessor::Instance()->LogInGoogle();
        }
        else if (control->GetName() == "weekly")
        {
            _isWeekly = true;
            UpdateBoard();
        } else if (control->GetName() == "alltime")
        {
            _isWeekly = false;
            UpdateBoard();
        }
    }

    void HighscoresStateProcessor::UpdateBoard()
    {
        using namespace std;

        if (!BillingProcessor::Instance()->IsLoggedGoogle())
        {
            _document->GetControlByName("scorePanel")->SetVisibleRecursive(false);
            _document->GetControlByName("loginPanel")->SetVisibleRecursive(true);
            _document->GetControlByName("scorePlayer2")->SetText(to_string(Scores::Instance()->GetBestArcadeScore()));
        } else {
            _document->GetControlByName("scorePanel")->SetVisibleRecursive(true);
            _document->GetControlByName("loginPanel")->SetVisibleRecursive(false);

            _document->GetControlByName("scorestype")->SetText(_isWeekly ? "Weekly scores" : "All time scores");
            auto scores = BillingProcessor::Instance()->GetScores(_isWeekly);
            int i;
            for (i = 0; i < scores.size(); i++)
            {
                if (i == 5 && scores[i].place < 6)
                    break;

                auto suffix = to_string(i+1);
                auto nameControl = _document->GetControlByName("name" + suffix);
                auto scoreControl = _document->GetControlByName("score" + suffix);
                nameControl->SetVisible(true);
                nameControl->SetText(to_string(scores[i].place) + ". " + scores[i].name);

                scoreControl->SetVisible(true);
                if (scores[i].score == -1)
                {
                    scores[i].score = Scores::Instance()->GetBestArcadeScore();
                }
                scoreControl->SetText(to_string(scores[i].score));
            }
            _document->GetControlByName("dots")->SetVisible(i > 5);
            for (; i < 6; i++)
            {
                auto suffix = to_string(i+1);
                _document->GetControlByName("name" + suffix)->SetVisible(false);
                _document->GetControlByName("score" + suffix)->SetVisible(false);
            }

            if (scores.size() == 0)
            {
                auto nameControl = _document->GetControlByName("name6");
                auto scoreControl = _document->GetControlByName("score6");
                nameControl->SetVisible(true);
                nameControl->SetText("Player");

                scoreControl->SetVisible(true);
                scoreControl->SetText(to_string(Scores::Instance()->GetBestArcadeScore()));
            }

        }
    }


}