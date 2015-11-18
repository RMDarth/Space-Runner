#pragma once
#include "GameState.h"
#include "Basic.h"
#include "IInputListener.h"

namespace CoreEngine
{
	class Game : public IInputListener
	{
		static Game* _game;

		GameState::State _state;

		std::vector<GameState::State> _overlappedStateList;

	public:
		static Game* Instance();

		void Update(float time);
		
		bool OnMouseMove(int xRel, int yRel, float deltaTime);
		bool OnMouseDown(int x, int y);
		bool OnMouseUp(int x, int y);
		void OnKeyPressed(OIS::KeyCode key);
		void OnKeyReleased(OIS::KeyCode key);

		void ChangeState(GameState::State state);
		GameState::State GetState();

	private:
		Game();
	};
}

