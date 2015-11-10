#include "CreditsStateProcessor.h"
#include "Game.h"
#include "LevelManager.h"
#include "BillingProcessor.h"
#include "Scores.h"
#include "Basic.h"
#include <string>
#include <sstream>
#include <climits>
#include <iomanip>

namespace CoreEngine
{
	CreditsStateProcessor::CreditsStateProcessor()
	{
		_document = new ControlDocument("GUI/credits.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	CreditsStateProcessor::~CreditsStateProcessor()
	{
		delete _document;
	}

	GameState::State CreditsStateProcessor::Update(float time)
	{
		return GameState::Credits;
	}

	void CreditsStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void CreditsStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void CreditsStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void CreditsStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void CreditsStateProcessor::Hide()
	{
		_document->Hide();
	}

	void CreditsStateProcessor::Show()
	{
		_document->Show();
	}

	void CreditsStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
			return;
		}
		if (control->GetName() == "credits1")
		{
			BillingProcessor::Instance()->OpenLink("http://turbulentsoft.com/");
		}
		if (control->GetName() == "credits2")
		{
			BillingProcessor::Instance()->OpenLink("http://turbulentsoft.com/");
		}
		if (control->GetName() == "facebook")
		{
			BillingProcessor::Instance()->OpenLink("https://www.facebook.com/bubbleshooter3d");
		}
		if (control->GetName() == "credits4" || control->GetName() == "credits7")
		{
			BillingProcessor::Instance()->OpenLink("mailto:igor.barinov@gmail.com");
		}
		if (control->GetName() == "credits6" || control->GetName() == "credits9")
		{
			BillingProcessor::Instance()->OpenLink("http://portfolios.ru/alopex");
		}
		if (control->GetName() == "credits10")
		{
			BillingProcessor::Instance()->OpenLink("https://www.facebook.com/pages/No13/576218159143568");
		}
		if (control->GetName() == "credits12")
		{
			BillingProcessor::Instance()->OpenLink("http://audiojungle.net/user/3KTrack-Exclusive/portfolio");
		}
		if (control->GetName() == "credits15")
		{
			BillingProcessor::Instance()->OpenLink("mailto:artem.goncharenko@gmail.com");
		}
		if (control->GetName() == "credits19")
		{
			BillingProcessor::Instance()->OpenLink("http://www.ogre3d.org/");
		}
	
	}
}