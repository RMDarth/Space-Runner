#include "HighScoresStateProcessor.h"
#include "Game.h"
#include "LevelManager.h"
#include "BillingProcessor.h"
#include "Scores.h"
#include "Basic.h"
#include "Config.h"
#include <string>
#include <sstream>
#include <climits>
#include <iomanip>

namespace CoreEngine
{
	HighScoresStateProcessor::HighScoresStateProcessor()
	{
		_document = new ControlDocument("GUI/highscores.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	HighScoresStateProcessor::~HighScoresStateProcessor()
	{
		delete _document;
	}

	GameState::State HighScoresStateProcessor::Update(float time)
	{
		return GameState::HighScores;
	}

	void HighScoresStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void HighScoresStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void HighScoresStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void HighScoresStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void HighScoresStateProcessor::Hide()
	{
		_document->Hide();
	}

	void HighScoresStateProcessor::Show()
	{
		_document->Show();

		auto scores = Scores::Instance();
		std::stringstream stream;
		for (int i = 0; i < 7; i++)
		{
			int bestScore = scores->GetBestArcadeScore(i + 1);
			int bestTime = scores->GetBestArcadeTime(i + 1);

			stream.str("");
			stream << "score" << (i + 1);
			auto scoreControl = _document->GetControlByName(stream.str());
			stream.str("");
			if (bestScore > 0)
			{
				stream << bestScore;
			}
			else {
				stream << "-";
			}

			scoreControl->SetText(stream.str());

			stream.str("");
			stream << "time" << (i + 1);
			auto timeControl = _document->GetControlByName(stream.str());
			stream.str("");
			if (bestTime < INT_MAX)
			{
				stream << (int)(bestTime / 60) << std::string(":") << std::setfill('0') << std::setw(2) << bestTime % 60;
			}
			else {
				stream << "-";
			}

			timeControl->SetText(stream.str());
		}
	}

	void HighScoresStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
		if (control->GetName() == "leaderboard")
		{
			BillingProcessor::Instance()->UpdateEndlessScore(LevelManager::Instance()->GetScore());
			BillingProcessor::Instance()->ShowLeaderboard();
		}
		if (control->GetName() == "header1")
		{
			static int counter = 0;
			counter++;
			if (counter > 5)
			{
				LevelManager::Instance()->SetEasterEgg(!LevelManager::Instance()->IsEasterEgg());
				counter = 0;
			}
		}
	}
}