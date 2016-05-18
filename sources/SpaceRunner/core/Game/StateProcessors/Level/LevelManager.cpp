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

		_time = -1;
		_score = 0;
		_missed = 0;
		_lives = 2;
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

	int LevelManager::GetScore()
	{
		return _score;
	}

	int LevelManager::GetTime()
	{
		return _time;
	}

	bool LevelManager::IsStarted()
	{
		return _isStarted;
	}

	bool LevelManager::IsVictory()
	{
		return _isVictory;
	}

	void LevelManager::SetLevelNum(int num)
	{
		_level = num;
	}

	void LevelManager::SetScore(int score)
	{
		_score = score;
	}

	void LevelManager::SetTime(int time)
	{
		_time = time;
	}

	void LevelManager::SetStarted(bool started)
	{
		_isStarted = started;
	}
	
	void LevelManager::SetVictory(bool victory)
	{
		_isVictory = victory;
	}

	int LevelManager::GetMissed()
	{
		return _missed;
	}

	void LevelManager::SetMissed(int missed)
	{
		_missed = missed;
	}

	int LevelManager::GetLives()
	{
		return _lives;
	}

	void LevelManager::SetLives(int lives)
	{
		_lives = lives;
	}

	void LevelManager::DecreaseLive()
	{
		_lives--;
	}

	void LevelManager::IncreaseLive()
	{
		_lives++;
	}

	void LevelManager::ResetMovies()
	{
		_isMoviesLeft = true;
	}

	void LevelManager::DecreaseMovies()
	{
		_isMoviesLeft = false;
	}

	bool LevelManager::IsMoviesLeft()
	{
		return _isMoviesLeft;
	}

	LevelType LevelManager::GetLevelType()
	{
		return _levelType;
	}

	void LevelManager::SetLevelTypes(LevelType levelType)
	{
		_levelType = levelType;
	}


}

