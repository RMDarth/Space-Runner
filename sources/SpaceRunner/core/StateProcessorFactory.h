#pragma once
#include "IStateProcessor.h"
#include "GameState.h"
#include <map>

namespace CoreEngine
{
	class StateProcessorFactory
	{
	private:
		std::map<GameState::State, IStateProcessor*> _processorList; 

		static StateProcessorFactory* _instance;
		StateProcessorFactory();

	public:
		static StateProcessorFactory* Instance();

		IStateProcessor* GetProcessor(GameState::State state);
		void RegisterProcessor(IStateProcessor * processor, GameState::State state);
	};

}

