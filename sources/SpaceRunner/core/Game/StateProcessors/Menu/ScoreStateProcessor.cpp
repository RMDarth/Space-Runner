#include "ScoreStateProcessor.h"
#include "BillingProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "SoundSystem.h"
#include "Game/Scores.h"
#include "Game/Game.h"
#include "Game/Config.h"
#include <sstream>
#include <iomanip>
#include <OgreLog.h>
#include <OgreLogManager.h>
#include <Game/SkinManager.h>

using namespace std;

namespace CoreEngine
{

    ScoreStateProcessor::ScoreStateProcessor()
    {
        _currentScore = 0;
        _score = 0;
        _totalTime = 0;
        _stars = 1;
        _span = 3.0f;
        _soundsLoaded = false;
        _achievementTime = 0;

        _document = new ControlDocument("GUI/score.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _achievementWindow = new ControlDocument("GUI/achievement.xml");
        _achievementWindow->Hide();


        _document->GetControlByName("restart")->GetPos(_restartX, _restartY);
        _document->GetControlByName("panel")->SetDefaultMaterial("windows/gameover1.png");
        _document->GetControlByName("panel")->SetDefaultMaterial("windows/gameover2.png");
        _document->GetControlByName("panel")->SetDefaultMaterial("windows/gameover3.png");
        _document->GetControlByName("panel")->SetDefaultMaterial("windows/gameover0.png");

        InitSound();
        InitAchievementNames();
    }

    void ScoreStateProcessor::InitAchievementNames()
    {
        _achievementName.push_back("Newcomer");
        _achievementName.push_back("Space pilot");
        _achievementName.push_back("Energizer");
        _achievementName.push_back("Conqueror");
        _achievementName.push_back("Star Hero");
        _achievementName.push_back("Space Virtuoso");
        _achievementName.push_back("Mission Accomplished");
        _achievementName.push_back("Unstoppable");
        _achievementName.push_back("Immortal");
        _achievementName.push_back("Elite pilot");
        _achievementName.push_back("Merchant");
        _achievementName.push_back("Bomberman");
        _achievementName.push_back("Collector");
        _achievementName.push_back("Fleet Commander");
        _achievementName.push_back("Star Ace");
        _achievementName.push_back("Superhuman");

        /*_achievementName.push_back("新来的人");
        _achievementName.push_back("泡沫射手");
        _achievementName.push_back("水平加速器");
        _achievementName.push_back("征服者");
        _achievementName.push_back("泡泡");
        _achievementName.push_back("星收集器");
        _achievementName.push_back("明星射手");
        _achievementName.push_back("超级明星");
        _achievementName.push_back("拼图的情人");
        _achievementName.push_back("拼图英雄");
        _achievementName.push_back("游戏加速器");
        _achievementName.push_back("无尽的驱逐舰");
        _achievementName.push_back("努力工作的人");
        _achievementName.push_back("深潜水");
        _achievementName.push_back("任务完成");
        _achievementName.push_back("超人");*/
    }

    ScoreStateProcessor::~ScoreStateProcessor()
    {
        delete _document;
    }

    void ScoreStateProcessor::SetScore(int score, int time, float span, int stars)
    {
        _score = score;
        _time = time;
        _currentScore = 0;
        _totalTime = 0;

        _span = span;
        _stars = stars;
    }

    GameState::State ScoreStateProcessor::Update(float time)
    {
        _totalTime += time;
        _currentScore += ((float)_score / _span) * time;
        if (_currentScore > _score)
            _currentScore = (float)_score;

        if (_achievementTime > 0)
        {
            _achievementTime -= time;
            if (_achievementTime < 0)
            {
                _achievementWindow->Hide();
            }
        }

        stringstream stream;

        auto control = _document->GetControlByName("time");
        stream << "Time: " << (int)_time / 60 << ":" << setfill('0') << setw(2) << (int)_time % 60;
        //stream << "时间 " << (int)_time / 60 << ":" << setfill('0') << setw(2) << (int)_time % 60;
        control->SetText(stream.str());

        control = _document->GetControlByName("score");
        stream.str("");
        //stream << "得分 " << (int)_currentScore;
        stream << "Score: " << (int)_currentScore;
        control->SetText(stream.str());

        float deltaTime = _span / _stars;

        if (_totalTime > deltaTime && _stars > 0)
        {
            _document->GetControlByName("panel")->SetMaterial("windows/gameover1.png");
            if (_soundsPlayed < 1)
            {
                _soundsPlayed = 1;
                if (Config::Instance()->IsSoundEnabled())
                    _starSound->Play();
            }
        }
        if (_totalTime > deltaTime * 2 && _stars > 1)
        {
            _document->GetControlByName("panel")->SetMaterial("windows/gameover2.png");
            if (_soundsPlayed < 2)
            {
                _soundsPlayed = 2;
                if (Config::Instance()->IsSoundEnabled())
                    _starSound->Play();
            }
        }
        if (_totalTime > deltaTime * 3 && _stars > 2)
        {
            _document->GetControlByName("panel")->SetMaterial("windows/gameover3.png");
            if (_soundsPlayed < 3)
            {
                _soundsPlayed = 3;
                if (Config::Instance()->IsSoundEnabled())
                    _starSound->Play();
            }
        }

        return GameState::Score;
    }

    void ScoreStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void ScoreStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void ScoreStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void ScoreStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            LevelManager::Instance()->SetStarted(false);
            if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
            {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
            else {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
        }
        if (key == OIS::KC_R)
        {
            LevelManager::Instance()->SetStarted(false);
            LevelManager::Instance()->SetLives(
                    LevelManager::Instance()->GetLevelType() == LevelType::Challenge ? 0 : SkinManager::Instance()->LivesCount());
            LevelManager::Instance()->ResetMovies();
            Game::Instance()->ChangeState(GameState::Level);
        }
    }


    void ScoreStateProcessor::Hide()
    {
        _document->Hide();
        _achievementWindow->Hide();
    }

    void ScoreStateProcessor::Show()
    {
        if (!_soundsLoaded)
            InitSound();

        auto levelManager = LevelManager::Instance();

        auto scores = Scores::Instance();
        int stars;
        bool highscore = false;
        bool daily = false;

        _document->Show();
        _achievementWindow->Hide();
        _soundsPlayed = 0;
        //_achievementTime = 0;


        if (levelManager->IsVictory())
        {
            _document->GetControlByName("result")->SetText("Victory");
            //_document->GetControlByName("result")->SetText("成功");

            // reset position of restart button
            if (levelManager->GetLevelType() == LevelType::Rush)
            {
                HideContinue();
            } else
            {
                auto restartBtn = _document->GetControlByName("restart");
                restartBtn->SetPos(_restartX, _restartY);
                restartBtn->SetVisible(true);
            }

            Scores::Instance()->UpdateTotalEnergy(levelManager->GetScore());

            if (levelManager->GetLevelType() == LevelType::Puzzle)
            {
                highscore = scores->UpdatePuzzleScore(levelManager->GetLevelNum(), levelManager->GetMissed());

                stars = scores->GetStars(levelManager->GetLevelNum(), levelManager->GetMissed());
            } else if (levelManager->GetLevelType() == LevelType::Rush)
            {
                if (levelManager->GetScore() >= 150)
                {
                    if (!Scores::Instance()->IsDailyChallengeCompleted(0))
                    {
                        levelManager->SetScore(levelManager->GetScore() + 150);
                        Scores::Instance()->UpdateTotalEnergy(150);
                        daily = true;
                    }

                    Scores::Instance()->SetDailyChallengeCompleted(0);
                }
                if (levelManager->GetTime() >= 240)
                {
                    if (!Scores::Instance()->IsDailyChallengeCompleted(1))
                    {
                        levelManager->SetScore(levelManager->GetScore() + 150);
                        Scores::Instance()->UpdateTotalEnergy(150);
                        daily = true;
                    }
                    Scores::Instance()->SetDailyChallengeCompleted(1);
                }

                highscore = scores->UpdateArcadeScore(levelManager->GetTime(), levelManager->GetScore());

                if (highscore)
                    stars = 3;
                else
                    stars = 2;
            } else {
                stars = 3;
                scores->SetChallengeCompleted(levelManager->GetLevelNum());
                // TODO: Show image with sign "Challenge bonus: 300 energy"
            }

            UpdateAchievements();

            if (levelManager->GetLevelType() == LevelType::Rush)
            {
                if (BillingProcessor::Instance()->IsLoggedGoogle())
                    BillingProcessor::Instance()->UpdateScore(levelManager->GetScore());
                else
                    Config::Instance()->SetScoreSubmitted(levelManager->GetScore());
            }

            _document->GetControlByName("highscore")->SetVisible(highscore);
            _document->GetControlByName("daily")->SetVisible(daily);

        } else {
            _document->GetControlByName("result")->SetText("Game over");
            //_document->GetControlByName("result")->SetText("游戏结束");
            _document->GetControlByName("highscore")->SetVisible(false);
            _document->GetControlByName("daily")->SetVisible(false);
            HideContinue();

            stars = 0;
        }

        stringstream ss;

        if (LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
            ss << "Level " << LevelManager::Instance()->GetLevelNum();
        else if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
            ss << "Single game";
        else
            ss << "Challenge " << LevelManager::Instance()->GetLevelNum();

    /*	if (LevelManager::Instance()->IsPuzzle())
            ss << "级别 " << LevelManager::Instance()->GetLevelNum();
        else
            ss << "困难 " << LevelManager::Instance()->GetLevelNum();*/

        _document->GetControlByName("levelname")->SetText(ss.str());

        SetScore(levelManager->GetScore(), levelManager->GetTime(), 1.5f, stars);
    }

    void ScoreStateProcessor::HideContinue()
    {
        int x, y;
        auto continueBtn = _document->GetControlByName("continue");
        auto restartBtn = _document->GetControlByName("restart");
        continueBtn->SetVisible(false);
        continueBtn->GetPos(x, y);
        restartBtn->SetPos(x, y);
        restartBtn->SetVisible(true);

    }


    void ScoreStateProcessor::InitSound()
    {
        auto soundSystem = SoundSystem::Instance();
        if (soundSystem->IsLoaded() && !_soundsLoaded)
        {
#ifdef __ANDROID_API__
            _starSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/Bell.wav"));
#else
            _starSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/Bell.wav"));
#endif
            _soundsLoaded = true;
        }
    }

    void ScoreStateProcessor::UpdateAchievements()
    {
        bool isPuzzle = LevelManager::Instance()->GetLevelType() == LevelType::Puzzle;
        bool isVictory = LevelManager::Instance()->IsVictory();
        int i, r;
        int achievementsCompleted = 0;

        if (!Config::Instance()->IsAchievementCompleted(0))
        {
            if (isPuzzle && isVictory)
            {
                ShowAchievement(0);
                Config::Instance()->SetAchievementCompleted(0);
                achievementsCompleted++;
            }
        } else { achievementsCompleted++; }

        if (!Config::Instance()->IsAchievementCompleted(1))
        {
            if (isPuzzle && isVictory && LevelManager::Instance()->GetLevelNum() > 1)
            {
                ShowAchievement(1);
                Config::Instance()->SetAchievementCompleted(1);
                achievementsCompleted++;
            }
        } else { achievementsCompleted++; }

        if (!Config::Instance()->IsAchievementCompleted(6))
        {
            int levelNum = 0;
            int levelNumMax = 0;
            for (i = 0; i < LEVELNUM; i++)
            {
                auto stars = Scores::Instance()->GetBestStars(i + 1);
                if (stars > 0)
                    levelNum++;
                if (stars == 3)
                    levelNumMax++;

            }
            if (levelNum >= 5)
            {
                ShowAchievement(3);
                Config::Instance()->SetAchievementCompleted(3);
                achievementsCompleted++;
            }
            if (levelNum >= 11)
            {
                ShowAchievement(4);
                Config::Instance()->SetAchievementCompleted(4);
                achievementsCompleted++;
            }
            if (levelNumMax >= 5)
            {
                ShowAchievement(5);
                Config::Instance()->SetAchievementCompleted(5);
                achievementsCompleted++;
            }
            if (levelNumMax >= 11)
            {
                ShowAchievement(6);
                Config::Instance()->SetAchievementCompleted(6);
                achievementsCompleted++;
            }
            Config::Instance()->SetAchievementData(3, levelNum);
            Config::Instance()->SetAchievementData(4, levelNum);
            Config::Instance()->SetAchievementData(5, levelNumMax);
            Config::Instance()->SetAchievementData(6, levelNumMax);
        } else {
            achievementsCompleted += 4;
        }

        if (!Config::Instance()->IsAchievementCompleted(8))
        {
            auto energy = LevelManager::Instance()->GetScore();
            if (energy >= 500)
            {
                ShowAchievement(7);
                Config::Instance()->SetAchievementCompleted(7);
                achievementsCompleted++;
            }
            if (energy >= 1000)
            {
                ShowAchievement(8);
                Config::Instance()->SetAchievementCompleted(8);
                achievementsCompleted++;
            }
        }
        else {
            achievementsCompleted += 2;
        }

        if (!Config::Instance()->IsAchievementCompleted(9))
        {
            // TODO: check if top-10 score (weekly or alltime)
        }
        else { achievementsCompleted++; }

        if (Config::Instance()->IsAchievementCompleted(11)) // use bomb
        {
            achievementsCompleted++;
        }

        if (!Config::Instance()->IsAchievementCompleted(12))
        {
            auto totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 300)
            {
                ShowAchievement(2);
                Config::Instance()->SetAchievementCompleted(2);
                achievementsCompleted++;
            }
            if (totalEnergy >= 10000)
            {
                ShowAchievement(12);
                Config::Instance()->SetAchievementCompleted(12);
                achievementsCompleted++;
            }
        } else {
            achievementsCompleted += 2;
        }

        if (!Config::Instance()->IsAchievementCompleted(13))
        {
            int shipsBought = 0;
            for (auto i = 0; i < 5; i++)
            {
                if (Config::Instance()->IsModelBought(i))
                    shipsBought++;
            }
            if (shipsBought > 1)
            {
                ShowAchievement(10);
                Config::Instance()->SetAchievementCompleted(10);
                achievementsCompleted++;
            }
            if (shipsBought >= 5)
            {
                ShowAchievement(13);
                Config::Instance()->SetAchievementCompleted(13);
                achievementsCompleted++;
            }

            Config::Instance()->SetAchievementData(13, shipsBought);
        } else {
            achievementsCompleted += 2;
        }

        if (!Config::Instance()->IsAchievementCompleted(14))
        {
            auto challengesCompleted = 0;
            for (auto i = 1; i <= 6; i++)
            {
                if (Scores::Instance()->IsChallengeCompleted(i))
                    challengesCompleted++;
            }

            if (challengesCompleted >= 6)
            {
                ShowAchievement(14);
                Config::Instance()->SetAchievementCompleted(14);
                achievementsCompleted++;
            }

            Config::Instance()->SetAchievementData(15, achievementsCompleted);
        }
        else { achievementsCompleted++; }

        if (!Config::Instance()->IsAchievementCompleted(15))
        {
            if (achievementsCompleted >= 15)
            {
                ShowAchievement(15);
                Config::Instance()->SetAchievementCompleted(15);
            }
            Config::Instance()->SetAchievementData(15, achievementsCompleted);
        }

        BillingProcessor::Instance()->SyncAchievements();
    }

    void ScoreStateProcessor::ShowAchievement(int id)
    {
        if (!BillingProcessor::Instance()->IsLoggedGoogle())
        {
            if (!Config::Instance()->IsAchievementCompleted(id))
            {
                _achievementWindow->GetControlByName("achievement")->SetText(_achievementName[id]);
                _achievementTime = 3.0f;
                _achievementWindow->Show();
            }
        }
    }

    void ScoreStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "continue")
        {
            LevelManager::Instance()->SetStarted(false);
            if (LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
            {
                int level = LevelManager::Instance()->GetLevelNum();
                if (level % 20 != 0)
                {
                    level++;
                    LevelManager::Instance()->SetLevelNum(level);
                    LevelManager::Instance()->SetLives(SkinManager::Instance()->LivesCount());
                    LevelManager::Instance()->ResetMovies();
                    Game::Instance()->ChangeState(GameState::Level);
                }
                else {
                    Game::Instance()->ChangeState(GameState::Storyboard);
                }
            } else if (LevelManager::Instance()->GetLevelType() == LevelType::Rush) {
                // TODO: This button should show scores
                Game::Instance()->ChangeState(GameState::Level);
            }
            else
            {
                _document->Hide();
                Game::Instance()->ChangeState(GameState::Challenges);
            }
        }
        if (control->GetName() == "restart")
        {
            LevelManager::Instance()->SetStarted(false);
            LevelManager::Instance()->SetLives(
                    LevelManager::Instance()->GetLevelType() == LevelType::Challenge ? 0 : SkinManager::Instance()->LivesCount());
            LevelManager::Instance()->ResetMovies();
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (control->GetName() == "tomenu")
        {
            if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
            {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
            else if (LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
            {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
            else
            {
                _document->Hide();
                Game::Instance()->ChangeState(GameState::Challenges);
            }
        }
    }
}