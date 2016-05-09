#pragma once
#include "Game/GameState.h"
#include "Basic.h"
#include <OIS/OIS.h>

namespace CoreEngine
{
	class IInputProcessor
	{
	public:
		virtual void OnMouseDown(int x, int y) { };
		virtual void OnMouseUp(int x, int y) { };
		virtual void OnMouseDoubleClick(int x, int y) { };
		virtual void OnKeyPressed(OIS::KeyCode key) { };

		// PC only
		virtual void OnMouseMove(int x, int y) { };
		virtual void OnKeyReleased(OIS::KeyCode key) { };
	};

	class IStateProcessor : public IInputProcessor
	{
	public:
		virtual GameState::State Update(float time) = 0;
		virtual ~IStateProcessor() { }

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual bool IsOverlapping() = 0;
	};
}