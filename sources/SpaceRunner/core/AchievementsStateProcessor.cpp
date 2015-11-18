#include "AchievementsStateProcessor.h"
#include "LevelManager.h"
#include "Game.h"
#include "Config.h"
#include "Basic.h"
#include <string>
#include <sstream>
#include <climits>
#include <iomanip>

using namespace std;

namespace CoreEngine
{
	AchievementsStateProcessor::AchievementsStateProcessor()
	{
		_document = new ControlDocument("GUI/achievementslist.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();

		_message = new ControlDocument("GUI/message.xml");
		_message->SetMouseUpHandler(this);
		_message->Hide();

		_messageShown = false;
		InitAchievementMessages();
	}


	AchievementsStateProcessor::~AchievementsStateProcessor()
	{
		delete _document;
	}

	void AchievementsStateProcessor::InitAchievementMessages()
	{
		_achievementMessages.push_back("Newcomer\nComplete any puzzle level");
		_achievementMessages.push_back("Bubble shooter\nComplete arcade level");
		_achievementMessages.push_back("Level smasher\nCompete 5 levels");
		_achievementMessages.push_back("Farm conqueror\nFinish levels campaign");
		_achievementMessages.push_back("Superbubble\nFinish 6th difficulty level");
		_achievementMessages.push_back("Star collector\nGet 15 stars");
		_achievementMessages.push_back("Star shooter\nCollect 40 stars!");
		_achievementMessages.push_back("Super star\nCollect 100 stars!");
		_achievementMessages.push_back("Puzzle lover\nComplete 40 levels");
		_achievementMessages.push_back("Puzzle hero\nComplete all levels");
		_achievementMessages.push_back("Arcade smasher\nFinish all arcade difficulty levels (except endless)");
		_achievementMessages.push_back("Endless destroyer\nDon't lose in endless mode for 5 minutes");
		_achievementMessages.push_back("Hard worker\nFinish 25 levels with 3 stars");
		_achievementMessages.push_back("Deep diver\nComplete 60 levels with 3 stars");
		_achievementMessages.push_back("Mission Accomplished\nComplete 100 levels with 3 stars");
		_achievementMessages.push_back("Superhuman\nComplete all achievements");

		/*_achievementMessages.push_back("新来的人\n完成的水平，任何难题");
		_achievementMessages.push_back("泡沫射手\n完整的骑楼水平");
		_achievementMessages.push_back("水平加速器\n比赛的5个层次");
		_achievementMessages.push_back("征服者\n完整的水平运动");
		_achievementMessages.push_back("泡泡\n完成第六个难度级别");
		_achievementMessages.push_back("星收集器\n获得15颗星");
		_achievementMessages.push_back("明星射手\n收集40颗星星");
		_achievementMessages.push_back("超级明星\n收集100颗星星");
		_achievementMessages.push_back("拼图的情人\n完成40关");
		_achievementMessages.push_back("拼图英雄\n完成所有的水平");
		_achievementMessages.push_back("游戏加速器\n完成所有的游戏难度（除了无尽的）");
		_achievementMessages.push_back("无尽的驱逐舰\n不要迷失在无尽的模式为5分钟");
		_achievementMessages.push_back("努力工作的人\n3颗星，完成25级");
		_achievementMessages.push_back("深潜水\n完整的60级与3星");
		_achievementMessages.push_back("任务完成\n完整的100级与3星");
		_achievementMessages.push_back("超人\n完成所有成就");*/
	}

	GameState::State AchievementsStateProcessor::Update(float time)
	{
		return GameState::Achievements;
	}

	void AchievementsStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void AchievementsStateProcessor::OnMouseUp(int x, int y)
	{
		if (!_messageShown) _document->OnMouseUp(x, y);
		else _message->OnMouseUp(x, y);
	}

	void AchievementsStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void AchievementsStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void AchievementsStateProcessor::Hide()
	{
		_document->Hide();
		_message->Hide();
		_messageShown = false;
	}

	void AchievementsStateProcessor::Show()
	{
		for (int i = 0; i < 16; i++)
		{
			stringstream ss;
			ss << "bg" << (i + 1);
			string bgControlName = ss.str();
			if (Config::Instance()->IsAchievementCompleted(i))
			{
				_document->GetControlByName(bgControlName)->SetDefaultMaterial("achievment_button1.png");
			}
			else {
				_document->GetControlByName(bgControlName)->SetDefaultMaterial("achievment_button0.png");
			}
		}

		_document->Show();
		_messageShown = false;
	}

	void AchievementsStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName().substr(0, 2) == "bg")
		{
			_document->Hide();
			int aNum = stoi(control->GetName().substr(2));
			_message->GetControlByName("text")->SetText(_achievementMessages[aNum-1]);
			_message->Show();
			_messageShown = true;
		} else if (_messageShown && type == EventType::MouseUp)
		{
			_document->Show();
			_message->Hide();
			_messageShown = false;
		}
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
			return;
		}
	}
}