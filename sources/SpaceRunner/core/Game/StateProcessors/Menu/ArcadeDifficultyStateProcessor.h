#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

class SceneSector;

namespace CoreEngine
{
	class ArcadeDifficultyStateProcessor : public IStateProcessor, public IEventHandler
	{
		ControlDocument * _document;
		ControlDocument * _loading;
		int _changeState;

	public:
		ArcadeDifficultyStateProcessor();
		~ArcadeDifficultyStateProcessor();

		virtual GameState::State Update(float time);

		virtual void OnMouseDown(int x, int y);
		virtual void OnMouseUp(int x, int y);
		virtual void OnMouseMove(int x, int y);
		virtual void OnKeyPressed(OIS::KeyCode key);

		virtual void Hide();
		virtual void Show();
		virtual bool IsOverlapping() { return false; }

		// IEventProcessor interface //
		virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);
	};
}