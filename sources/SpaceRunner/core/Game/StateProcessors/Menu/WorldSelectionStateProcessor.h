#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
#include "SliderProcessor.h"
#define WORLD_NUM 5

namespace Ogre
{
	class Rectangle2D;
}
class SceneSector;

namespace CoreEngine
{
	class WorldSelectionStateProcessor : public SliderProcessor, public IStateProcessor, public IEventHandler
	{
		Ogre::Rectangle2D** rects;
		Ogre::Rectangle2D** lockRects;
		bool _unlocked[2];
		SceneSector** sectors;

		ControlDocument * _document;

		int _stars;
		int _prevObject;
		float _totalTime;
		bool _bought;

	protected:
		virtual void SetObjectPosition(int object, float left, float top, float right, float bottom);
		virtual void SetCurrentObject(int object);

	public:
		WorldSelectionStateProcessor();
		~WorldSelectionStateProcessor();

		// IStateProcessor interface //
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

	private:
		void CreateMaterials();
		void CreateWorlds();
		void SetLightAndCamera();
		void UpdateUnlockText();
	};
}
