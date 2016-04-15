#include "LevelSelectionStateProcessor.h"
#include "RenderProcessor.h"
#include "Game/Game.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Scores.h"
#include "Basic.h"
#include <string>
#include <sstream>


namespace CoreEngine
{

	LevelSelectionStateProcessor::LevelSelectionStateProcessor()
	{
		_document = new ControlDocument("GUI/levelselect.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();

		_loading = new ControlDocument("GUI/loading.xml");
		_loading->Hide();

		_changeState = 0;
	}


	LevelSelectionStateProcessor::~LevelSelectionStateProcessor()
	{
		delete _document;
	}

	GameState::State LevelSelectionStateProcessor::Update(float time)
	{
		if (_changeState == 1)
			_changeState = 2;
		else if (_changeState == 2)
		{
			_changeState = 0;
			return GameState::Level;
		}
		return GameState::Storyboard;
	}
	
	void LevelSelectionStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void LevelSelectionStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void LevelSelectionStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void LevelSelectionStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void LevelSelectionStateProcessor::Hide()
	{
		_document->Hide();
		_loading->Hide();
	}

	void LevelSelectionStateProcessor::Show()
	{
		_document->Show();

		std::stringstream stream;
		for (int i = 0; i < 20; i++)
		{
			stream.str("");
			stream << (i+1);
			auto control = _document->GetControlByName(stream.str());

			stream.str("");
			stream << Scores::Instance()->GetBestStars(i + 1 + LevelManager::Instance()->GetWorld() * 20);
			control->SetCustomAttribute("stars", stream.str());

			/* TO REMOVE
			stream.str("");
			int time = Scores::Instance()->GetBestPuzzleTime(i + 1 + LevelManager::Instance()->GetWorld() * 20);
			stream << (i + 1) << ":";
			if (time == INT_MAX)
				stream << "-";
			else
				stream << time;
		
			control->SetText(stream.str());*/
		}

		RenderProcessor::Instance()->SetSkybox(LevelManager::Instance()->GetWorld() + 1);

		std::string campaignName;
		switch (LevelManager::Instance()->GetWorld())
		{
		case 0:
			campaignName = "Factory campaign";
			break;
		case 1:
			campaignName = "Blocks campaign";
			break;
		case 2:
			campaignName = "Space campaign";
			break;
		case 3:
			campaignName = "Clouds campaign";
			break;
		case 4:
			campaignName = "Temple campaign";
			break;
		}
		/*switch (LevelManager::Instance()->GetWorld())
		{
		case 0:
			campaignName = "工厂的活动";
			break;
		case 1:
			campaignName = "块运动";
			break;
		case 2:
			campaignName = "空间运动";
			break;
		case 3:
			campaignName = "云的运动";
			break;
		case 4:
			campaignName = "庙会活动";
			break;
		}*/
		_document->GetControlByName("worldname")->SetText(campaignName);
	}

	void LevelSelectionStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
		else if (control->GetName() == "reset")
		{
			Scores::Instance()->Reset();
			Show();
		}
		else if (control->GetName() != "panel") 
		{
			LevelManager::Instance()->SetIsPuzzle(true);
			LevelManager::Instance()->SetLevelNum(LevelManager::Instance()->GetWorld() * 20 + stoi(control->GetName()));
			LevelManager::Instance()->SetStarted(false);
			_changeState = 1;
			_document->Hide();
			_loading->Show();
		}
	}
}