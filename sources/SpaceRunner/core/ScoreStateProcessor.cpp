#include "ScoreStateProcessor.h"
#include "BillingProcessor.h"
#include "WorldSelectionStateProcessor.h"
#include "LevelManager.h"
#include "SoundSystem.h"
#include "Scores.h"
#include "Game.h"
#include "Config.h"
#include <sstream>
#include <iomanip>
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
		_achievementName.push_back("Bubble Shooter");
		_achievementName.push_back("Level Smasher");
		_achievementName.push_back("Conqueror");
		_achievementName.push_back("Unstoppable");
		_achievementName.push_back("Star Collector");
		_achievementName.push_back("Star Shooter");
		_achievementName.push_back("Super Star");
		_achievementName.push_back("Puzzle Lover");
		_achievementName.push_back("Puzzle Hero");
		_achievementName.push_back("Arcade Smasher");
		_achievementName.push_back("Endless Destroyer");
		_achievementName.push_back("Hard Worker");
		_achievementName.push_back("Deep Diver");
		_achievementName.push_back("Mission Accomplished");
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
			if (LevelManager::Instance()->IsPuzzle())
			{
				Game::Instance()->ChangeState(GameState::LevelSelect);
			}
			else {
				Game::Instance()->ChangeState(GameState::DifficultySelect);
			}
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
		bool highscore;

		_document->Show();
		_achievementWindow->Hide();
		_soundsPlayed = 0;
		//_achievementTime = 0;

		
		if (levelManager->IsVictory() || levelManager->IsEndless())
		{
			if (levelManager->IsVictory())
			{
				_document->GetControlByName("result")->SetText("Victory");
				//_document->GetControlByName("result")->SetText("成功");

				// reset position of restart button
				auto restartBtn = _document->GetControlByName("restart");
				restartBtn->SetPos(_restartX, _restartY);
				restartBtn->SetVisible(true);
			}
			else
			{
				_document->GetControlByName("result")->SetText("Game over");
				//_document->GetControlByName("result")->SetText("游戏结束"); 
				HideContinue();
			}

			if (levelManager->IsPuzzle())
			{
				highscore = scores->UpdatePuzzleScore(levelManager->GetLevelNum(), levelManager->GetTime(), levelManager->GetScore());

				stars = scores->GetStars(levelManager->GetLevelNum(), levelManager->GetTime());
			} else {
				highscore = scores->UpdateArcadeScore(levelManager->GetLevelNum(), levelManager->GetTime(), levelManager->GetScore());

				if (highscore) 
					stars = 3;
				else
					stars = 2;
			}

			UpdateAchievements();

			if (highscore && levelManager->IsEndless())
				BillingProcessor::Instance()->UpdateEndlessScore(levelManager->GetScore());

			if (highscore)
			{
				_document->GetControlByName("highscore")->SetVisible(true);
			} else {
				_document->GetControlByName("highscore")->SetVisible(false);
			}
		} else {
			_document->GetControlByName("result")->SetText("Game over");
			//_document->GetControlByName("result")->SetText("游戏结束");
			_document->GetControlByName("highscore")->SetVisible(false);
			HideContinue();

			stars = 0;
		}

		stringstream ss;

		if (LevelManager::Instance()->IsPuzzle())
			ss << "Level " << LevelManager::Instance()->GetWorld() + 1 << "-" << ((LevelManager::Instance()->GetLevelNum() - 1) % 20) + 1;
		else
			ss << "Difficulty " << LevelManager::Instance()->GetLevelNum();

	/*	if (LevelManager::Instance()->IsPuzzle())
			ss << "级别 " << LevelManager::Instance()->GetWorld() + 1 << "-" << ((LevelManager::Instance()->GetLevelNum() - 1) % 20) + 1;
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
		if (soundSystem->IsLoaded() && _soundsLoaded == false)
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
		bool isPuzzle = LevelManager::Instance()->IsPuzzle();
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
			if (!isPuzzle && isVictory)
			{
				ShowAchievement(1);
				Config::Instance()->SetAchievementCompleted(1);
				achievementsCompleted++;
			}
		} else { achievementsCompleted++; }

		if (!Config::Instance()->IsAchievementCompleted(9))
		{
			int levelNum = 0;
			for (i = 0; i < LEVELNUM; i++)
			{
				if (Scores::Instance()->GetBestStars(i + 1) > 0)
					levelNum++;
			}
			if (levelNum >= 5)
			{
				ShowAchievement(2);
				Config::Instance()->SetAchievementCompleted(2);
				achievementsCompleted++;
			}
			if (levelNum >= 40)
			{
				ShowAchievement(8);
				Config::Instance()->SetAchievementCompleted(8);
				achievementsCompleted++;
			}
			if (levelNum >= 100)
			{
				ShowAchievement(9);
				Config::Instance()->SetAchievementCompleted(9);
				achievementsCompleted++;
			}
			Config::Instance()->SetAchievementData(2, levelNum);
			Config::Instance()->SetAchievementData(8, levelNum);
			Config::Instance()->SetAchievementData(9, levelNum);
		}
		else
		{
			achievementsCompleted += 3;
		}

		if (!Config::Instance()->IsAchievementCompleted(3))
		{
			bool found = false;
			for (i = 0; i < WORLD_NUM; i++)
			{
				for (r = i * 20; r < (i + 1) * 20; r++)
				{
					if (Scores::Instance()->GetBestStars(r + 1) == 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					ShowAchievement(3);
					Config::Instance()->SetAchievementCompleted(3);
					achievementsCompleted++;
					break;
				}
				found = false;
			}
		} else { achievementsCompleted++; }

		if (!Config::Instance()->IsAchievementCompleted(4))
		{
			if (!isPuzzle && isVictory && LevelManager::Instance()->GetLevelNum() == 6)
			{
				ShowAchievement(4);
				Config::Instance()->SetAchievementCompleted(4);
				achievementsCompleted++;
			}
			else if (Scores::Instance()->GetBestArcadeScore(6) > 0)
			{
				ShowAchievement(4);
				Config::Instance()->SetAchievementCompleted(4);
				achievementsCompleted++;
			}	
		}
		else { achievementsCompleted++; }

		if (!Config::Instance()->IsAchievementCompleted(7))
		{
			int stars = 0;
			for (i = 0; i < LEVELNUM; i++)
			{
				stars += Scores::Instance()->GetBestStars(i + 1);
			}
			if (stars >= 15)
			{
				ShowAchievement(5);
				Config::Instance()->SetAchievementCompleted(5);
				achievementsCompleted++;
			}
			if (stars >= 40)
			{
				ShowAchievement(6);
				Config::Instance()->SetAchievementCompleted(6);
				achievementsCompleted++;
			}
			if (stars >= 100)
			{
				ShowAchievement(7);
				Config::Instance()->SetAchievementCompleted(7);
				achievementsCompleted++;
			}
			Config::Instance()->SetAchievementData(5, stars);
			Config::Instance()->SetAchievementData(6, stars);
			Config::Instance()->SetAchievementData(7, stars);
		}
		else {
			achievementsCompleted += 3;
		}

		if (!Config::Instance()->IsAchievementCompleted(10))
		{
			int levelNum = 0;
			for (i = 0; i < 6; i++)
			{
				if (Scores::Instance()->GetBestArcadeScore(i + 1) > 0)
					levelNum++;
			}
			if (levelNum == 6)
			{
				ShowAchievement(10);
				Config::Instance()->SetAchievementCompleted(10);
				achievementsCompleted++;
			}
			Config::Instance()->SetAchievementData(10, levelNum);
		}
		else { achievementsCompleted++; }

		if (!Config::Instance()->IsAchievementCompleted(11))
		{
			if (Scores::Instance()->GetBestArcadeTime(7) >= 300)
			{
				ShowAchievement(11);
				Config::Instance()->SetAchievementCompleted(11);
				achievementsCompleted++;
			}
		}
		else { achievementsCompleted++; }

		if (!Config::Instance()->IsAchievementCompleted(14))
		{
			int levelNum = 0;
			for (i = 0; i < LEVELNUM; i++)
			{
				if (Scores::Instance()->GetBestStars(i + 1) == 3)
					levelNum++;
			}
			if (levelNum >= 25)
			{
				ShowAchievement(12);
				Config::Instance()->SetAchievementCompleted(12);
				achievementsCompleted++;
			}
			if (levelNum >= 60)
			{
				ShowAchievement(13);
				Config::Instance()->SetAchievementCompleted(13);
				achievementsCompleted++;
			}
			if (levelNum >= 100)
			{
				ShowAchievement(14);
				Config::Instance()->SetAchievementCompleted(14);
				achievementsCompleted++;
			}
			Config::Instance()->SetAchievementData(12, levelNum);
			Config::Instance()->SetAchievementData(13, levelNum);
			Config::Instance()->SetAchievementData(14, levelNum);
		}
		else
		{
			achievementsCompleted += 3;
		}

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
			if (LevelManager::Instance()->IsPuzzle())
			{
				int level = LevelManager::Instance()->GetLevelNum();
				if (level % 20 != 0)
				{
					level++;
					LevelManager::Instance()->SetLevelNum(level);
					Game::Instance()->ChangeState(GameState::Level);
				}
				else {
					Game::Instance()->ChangeState(GameState::LevelSelect);
				}
			} else {
				int level = LevelManager::Instance()->GetLevelNum();
				if (level < 6) level++;
				LevelManager::Instance()->SetLevelNum(level);
				Game::Instance()->ChangeState(GameState::Level);
			}
		}
		if (control->GetName() == "restart")
		{
			LevelManager::Instance()->SetStarted(false);
			Game::Instance()->ChangeState(GameState::Level);
		}
		if (control->GetName() == "tomenu")
		{
			if (LevelManager::Instance()->IsPuzzle())
			{
				Game::Instance()->ChangeState(GameState::LevelSelect);
			}
			else {
				Game::Instance()->ChangeState(GameState::DifficultySelect);
			}
		}
	}
}