#pragma once
#include <string>

namespace CoreEngine
{
	class LevelManager
	{
		static LevelManager* _instance;

		int _level;
		int _score;
		int _time;
		int _world;
		bool _isPuzzle;
		bool _isVictory;
		bool _isStarted;
		bool _isEasterEgg;
		bool _isEndless;
		std::string _help;

	public:
		static LevelManager* Instance();

		int GetLevelNum();
		int GetScore();
		int GetTime();
		int GetWorld();
		std::string GetHelp();
		bool IsPuzzle();
		bool IsStarted();
		bool IsVictory();
		bool IsEasterEgg();
		bool IsEndless();
		bool ContainsHelp();

		void SetLevelNum(int num);
		void SetScore(int score);
		void SetTime(int time);
		void SetIsPuzzle(bool puzzle);
		void SetWorld(int world);
		void SetStarted(bool started);
		void SetVictory(bool victory);
		void SetEasterEgg(bool easter);
		void SetEndless(bool endless);
		void SetHelp(std::string text);

	private:
		LevelManager();
	};
}

