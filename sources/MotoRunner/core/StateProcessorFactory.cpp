#include "StateProcessorFactory.h"
using namespace CoreEngine;

StateProcessorFactory* StateProcessorFactory::_instance = nullptr;

StateProcessorFactory::StateProcessorFactory()
{
	_processorList.clear();
}

StateProcessorFactory* StateProcessorFactory::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new StateProcessorFactory();
	}

	return _instance;
}

IStateProcessor* StateProcessorFactory::GetProcessor(GameState::State state)
{
	if (_processorList.find(state) != _processorList.end())
	{
		return _processorList[state];
	} else {
		return nullptr;
	}
}

void StateProcessorFactory::RegisterProcessor(IStateProcessor* processor, GameState::State state)
{
	_processorList[state] = processor;
}

