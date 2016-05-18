#include <Game/SkinManager.h>
#include "PauseStateProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"

namespace CoreEngine
{

	PauseStateProcessor::PauseStateProcessor()
	{
		_document = new ControlDocument("GUI/pausemenu.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	PauseStateProcessor::~PauseStateProcessor()
	{
	}

	GameState::State PauseStateProcessor::Update(float time)
	{
		return GameState::Pause;
	}
	
	void PauseStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void PauseStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void PauseStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void PauseStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::Level);
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


	void PauseStateProcessor::Hide()
	{
		_document->Hide();
	}

	void PauseStateProcessor::Show()
	{
		_document->Show();
	}

	void PauseStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "continue")
		{
			Game::Instance()->ChangeState(GameState::Level);
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
			LevelManager::Instance()->SetStarted(false);
			if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
			{
				Game::Instance()->ChangeState(GameState::MainMenu);
			}
			else {
				Game::Instance()->ChangeState(GameState::Storyboard);
			}
		}
	}
}