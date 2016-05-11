#pragma once

#include <Game/GameDefs.h>
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

class Sound;

namespace CoreEngine
{
	class SaveMeStateProcessor : public IStateProcessor, public IEventHandler
	{
		ControlDocument * _document;
		ControlDocument * _query;

        bool _queryVisible;

		Ptr<Sound> _successSound;
		bool _soundsLoaded;

	public:
		SaveMeStateProcessor();
		~SaveMeStateProcessor();

		virtual GameState::State Update(float time);
		virtual void OnMouseDown(int x, int y);
		virtual void OnMouseUp(int x, int y);
		virtual void OnMouseMove(int x, int y);
		virtual void OnKeyPressed(OIS::KeyCode key);

		virtual void Hide();
		virtual void Show();
		virtual bool IsOverlapping() { return true; }

		// IEventProcessor interface //
		virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

	private:
		void InitSound();
	};
}

