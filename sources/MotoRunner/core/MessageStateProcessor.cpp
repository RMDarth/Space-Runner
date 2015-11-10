#include "MessageStateProcessor.h"
#include "LevelManager.h"
#include "Game.h"

namespace CoreEngine
{

	MessageStateProcessor::MessageStateProcessor()
	{
		_document = new ControlDocument("GUI/message.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	MessageStateProcessor::~MessageStateProcessor()
	{
	}

	GameState::State MessageStateProcessor::Update(float time)
	{
		return GameState::Message;
	}

	void MessageStateProcessor::OnMouseDown(int x, int y)
	{
		_document->OnMouseDown(x, y);
	}

	void MessageStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void MessageStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void MessageStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::Level);
		}
	}


	void MessageStateProcessor::Hide()
	{
		_document->Hide();
	}

	void MessageStateProcessor::Show()
	{
		_document->GetControlByName("text")->SetText(LevelManager::Instance()->GetHelp());

		_document->Show();
	}

	void MessageStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "ok" || type == EventType::MouseUp)
		{
			Game::Instance()->ChangeState(GameState::Level);
		}
	}
}