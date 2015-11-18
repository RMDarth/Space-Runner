#define _CRT_SECURE_NO_WARNINGS
#include "ArcadeDifficultyStateProcessor.h"
#include "Game.h"
#include "LevelManager.h"
#include "Scores.h"
#include "Basic.h"
#include <string>
#include <sstream>

namespace CoreEngine
{

	ArcadeDifficultyStateProcessor::ArcadeDifficultyStateProcessor()
	{
		_document = new ControlDocument("GUI/arcade.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();

		_loading = new ControlDocument("GUI/loading.xml");
		_loading->Hide();

		_changeState = 0;
		
		_document->GetControlByName("1")->SetDefaultMaterial("levelx1.png", true);
	}


	ArcadeDifficultyStateProcessor::~ArcadeDifficultyStateProcessor()
	{
		delete _document;
	}

	GameState::State ArcadeDifficultyStateProcessor::Update(float time)
	{
		if (_changeState == 1)
			_changeState = 2;
		else if (_changeState ==  2)
		{
			_changeState = 0;
			return GameState::Level;
		}
		return GameState::DifficultySelect;
	}
	
	void ArcadeDifficultyStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void ArcadeDifficultyStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void ArcadeDifficultyStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void ArcadeDifficultyStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void ArcadeDifficultyStateProcessor::Hide()
	{
		_document->Hide();
		_loading->Hide();
	}

	void ArcadeDifficultyStateProcessor::Show()
	{
		_document->Show();
		char name[100];
		_changeState = false;
		
		for (int i = 1; i <= 7; i++)
		{
			sprintf(name, "%d", i);

			if (Scores::Instance()->GetBestArcadeScore(i) > 0)
				_document->GetControlByName(name)->SetDefaultMaterial("levelx1.png");
			else
				_document->GetControlByName(name)->SetDefaultMaterial("levelx0.png");
		}
	}

	void ArcadeDifficultyStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
		else if (control->GetName() != "panel") 
		{
			LevelManager::Instance()->SetIsPuzzle(false);
			LevelManager::Instance()->SetLevelNum(stoi(control->GetName()));
			LevelManager::Instance()->SetStarted(false);
			_changeState = 1;
			_document->Hide();
			_loading->Show();
		}
	}
}