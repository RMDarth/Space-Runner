#include "StoreStateProcessor.h"
#include "Game/Game.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "BillingProcessor.h"
#include "Game/Scores.h"
#include "Basic.h"
#include <string>
#include <iomanip>
#include <Game/Config.h>

using namespace std;

namespace CoreEngine
{
	StoreStateProcessor::StoreStateProcessor()
	{
		_document = new ControlDocument("GUI/store.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();

        _bonusesState = false;
	}


	StoreStateProcessor::~StoreStateProcessor()
	{
		delete _document;
	}

	GameState::State StoreStateProcessor::Update(float time)
	{
        _lastStoreCheckTime += time;
		UpdateButtons();

        if (_lastStoreCheckTime > 0.7f)
        {
            _lastStoreCheckTime = 0;
            auto boughtItem = BillingProcessor::Instance()->GetBoughtItem();
            if (boughtItem != -1)
            {
                int energy = 0;
                switch (boughtItem)
                {
                    case 0:
                        energy = 5000;
                        break;
                    case 1:
                        energy = 12500;
                        break;
                    case 2:
                        energy = 50000;
                        break;
                    case 3:
                        energy = 75000;
                        break;
                    case 4:
                        energy = 100000;
                        break;
                }
                Scores::Instance()->UpdateTotalEnergy(energy);
                UpdateButtons();
            }
        }

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

		_document->GetControlByName("bonusesPanel")->SetVisibleRecursive(false);
		_document->GetControlByName("energyPanel")->SetVisibleRecursive(true);
        _bonusesState = false;

		UpdateButtons();
	}

	void StoreStateProcessor::UpdateButtons()
	{
        _document->GetControlByName("bank")->SetText(to_string(Scores::Instance()->GetTotalEnergy()));

        if (_bonusesState)
        {
            _document->GetControlByName("bombcount")->SetText(
                    "You have: " + to_string(Config::Instance()->GetBombCount()));
            _document->GetControlByName("bombcount2")->SetText(
                    "You have: " + to_string(Config::Instance()->GetBombCount()));
            _document->GetControlByName("resurrectcount")->SetText(
                    "You have: " + to_string(Config::Instance()->GetResurrectCount()));
            _document->GetControlByName("resurrectcount2")->SetText(
                    "You have: " + to_string(Config::Instance()->GetResurrectCount()));

            bool isMultiplierBought = Config::Instance()->IsMultiplierBought();
            _document->GetControlByName("boughtmultiplier")->SetVisible(isMultiplierBought);
            _document->GetControlByName("buymultiplier")->SetVisible(!isMultiplierBought);
        }
	}

	void StoreStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
		else if (control->GetName() == "bonuses")
		{
			_document->GetControlByName("bonusesPanel")->SetVisibleRecursive(true);
			_document->GetControlByName("energyPanel")->SetVisibleRecursive(false);
            _bonusesState = true;
            UpdateButtons();
		}
		else if (control->GetName() == "energy")
		{
			_document->GetControlByName("bonusesPanel")->SetVisibleRecursive(false);
			_document->GetControlByName("energyPanel")->SetVisibleRecursive(true);
            _bonusesState = false;
            UpdateButtons();
		}
        else if (!control->GetName().compare(0, 13, "buyenergypack"))
        {
            int packId = control->GetName()[13] - '0';
            BillingProcessor::Instance()->BuyEnergy(packId - 1);

        } else if (control->GetName() == "buybomb1")
        {
            int totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 1200)
            {
                Scores::Instance()->UpdateTotalEnergy(-1200);
                Config::Instance()->SetBombCount(Config::Instance()->GetBombCount() + 1);
                UpdateButtons();
            }
        } else if (control->GetName() == "buybomb5")
        {
            int totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 5000)
            {
                Scores::Instance()->UpdateTotalEnergy(-5000);
                Config::Instance()->SetBombCount(Config::Instance()->GetBombCount() + 5);
                UpdateButtons();
            }
        } else if (control->GetName() == "buymultiplier")
        {
            int totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 4000)
            {
                Scores::Instance()->UpdateTotalEnergy(-4000);
                Config::Instance()->SetMultiplierBought();
                UpdateButtons();
            }
        } else if (control->GetName() == "buyresurrect1")
        {
            int totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 5000)
            {
                Scores::Instance()->UpdateTotalEnergy(-5000);
                Config::Instance()->SetResurrectCount(Config::Instance()->GetResurrectCount() + 1);
                UpdateButtons();
            }
        } else if (control->GetName() == "buyresurrect5")
        {
            int totalEnergy = Scores::Instance()->GetTotalEnergy();
            if (totalEnergy >= 20000)
            {
                Scores::Instance()->UpdateTotalEnergy(-20000);
                Config::Instance()->SetResurrectCount(Config::Instance()->GetResurrectCount() + 5);
                UpdateButtons();
            }
        }
	}
}