#include "LevelManager.h"

namespace CoreEngine
{
	LevelManager* LevelManager::_instance = nullptr;

	LevelManager::LevelManager()
	{
		_level = 0;
		_isPuzzle = false;
		_isStarted = false;
		_isVictory = false;
		_isEasterEgg = false;
		_isEndless = false;
		_time = -1;
		_score = 0;
	}

	LevelManager* LevelManager::Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new LevelManager();
		}
		return _instance;
	}

	int LevelManager::GetLevelNum()
	{
		return _level;
	}

	int LevelManager::GetWorld()
	{
		return _world;
	}

	int LevelManager::GetScore()
	{
		return _score;
	}

	int LevelManager::GetTime()
	{
		return _time;
	}

	std::string LevelManager::GetHelp()
	{
		return _help;
	}

	bool LevelManager::IsPuzzle()
	{
		return _isPuzzle;
	}

	bool LevelManager::IsStarted()
	{
		return _isStarted;
	}

	bool LevelManager::IsVictory()
	{
		return _isVictory;
	}
	
	bool LevelManager::IsEasterEgg()
	{
		return _isEasterEgg;
	}

	bool LevelManager::IsEndless()
	{
		return _isEndless;
	}

	bool LevelManager::ContainsHelp()
	{
		return !_help.empty();
	}

	void LevelManager::SetLevelNum(int num)
	{
		_level = num;
	}

	void LevelManager::SetWorld(int world)
	{
		_world = world;
	}

	void LevelManager::SetScore(int score)
	{
		_score = score;
	}

	void LevelManager::SetTime(int time)
	{
		_time = time;
	}

	void LevelManager::SetIsPuzzle(bool isPuzzle)
	{
		_isPuzzle = isPuzzle;
	}

	void LevelManager::SetStarted(bool started)
	{
		_isStarted = started;
	}
	
	void LevelManager::SetVictory(bool victory)
	{
		_isVictory = victory;
	}

	void LevelManager::SetEasterEgg(bool easterEgg)
	{
		_isEasterEgg = easterEgg;
	}

	void LevelManager::SetEndless(bool endless)
	{
		_isEndless = endless;
	}

	void LevelManager::SetHelp(std::string text)
	{
		_help = text;
	}
}

