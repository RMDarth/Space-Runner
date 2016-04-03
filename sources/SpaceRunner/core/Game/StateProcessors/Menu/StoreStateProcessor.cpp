#include "StoreStateProcessor.h"
#include "Game/Game.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "BillingProcessor.h"
#include "Game/Scores.h"
#include "Basic.h"
#include <string>
#include <sstream>
#include <climits>
#include <iomanip>

namespace CoreEngine
{
	StoreStateProcessor::StoreStateProcessor()
	{
		_document = new ControlDocument("GUI/store.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	StoreStateProcessor::~StoreStateProcessor()
	{
		delete _document;
	}

	GameState::State StoreStateProcessor::Update(float time)
	{
		UpdateButtons();

		return GameState::Store;
	}

	void StoreStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void StoreStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void StoreStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void StoreStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}


	void StoreStateProcessor::Hide()
	{
		_document->Hide();
	}

	void StoreStateProcessor::Show()
	{
		_document->Show();

		UpdateButtons();
	}

	void StoreStateProcessor::UpdateButtons()
	{
		bool bought = BillingProcessor::Instance()->IsItemBought(MODELS_ITEM_ID);
		if (!bought)
		{
			_document->GetControlByName("buymodels")->SetVisible(true);
			_document->GetControlByName("boughtmodels")->SetVisible(false);
		}
		else {
			_document->GetControlByName("buymodels")->SetVisible(false);
			_document->GetControlByName("boughtmodels")->SetVisible(true);
		}

		bought = BillingProcessor::Instance()->IsItemBought(LEVELS_ITEM_ID);
		if (!bought)
		{
			_document->GetControlByName("buylevels")->SetVisible(true);
			_document->GetControlByName("boughtlevels")->SetVisible(false);
		}
		else {
			_document->GetControlByName("buylevels")->SetVisible(false);
			_document->GetControlByName("boughtlevels")->SetVisible(true);
		}
	}

	void StoreStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
		if (control->GetName() == "buylevels" || control->GetName() == "levelsimage")
		{
			bool bought = BillingProcessor::Instance()->IsItemBought(LEVELS_ITEM_ID);
			if (!bought)
			{
				BillingProcessor::Instance()->BuyItem(LEVELS_ITEM_ID);
			}
		}
		if (control->GetName() == "buymodels" || control->GetName() == "modelsimage")
		{
			bool bought = BillingProcessor::Instance()->IsItemBought(MODELS_ITEM_ID);
			if (!bought)
			{
				BillingProcessor::Instance()->BuyItem(MODELS_ITEM_ID);
			}
		}
	}
}