#pragma once
#include <string>

namespace CoreEngine
{
    enum class LevelType
    {
        Puzzle,
        Rush,
        Challenge
    };

    class LevelManager
    {
        static LevelManager* _instance;

        int _level;
        int _score;
        int _missed;
        int _time;
        int _lives;

        LevelType _levelType;

        bool _isPuzzle;
        bool _isVictory;
        bool _isStarted;
        bool _isMoviesLeft;

    public:
        static LevelManager* Instance();

        int GetLevelNum();
        int GetScore();
        int GetMissed();
        int GetTime();
        int GetLives();
        LevelType GetLevelType();

        bool IsStarted();
        bool IsVictory();
        bool IsMoviesLeft();

        void SetLevelNum(int num);
        void SetScore(int score);
        void SetMissed(int missed);
        void SetTime(int time);
        void SetStarted(bool started);
        void SetVictory(bool victory);
        void SetLives(int lives);
        void SetLevelTypes(LevelType levelType);
        void DecreaseLive();
        void IncreaseLive();
        void ResetMovies();
        void DecreaseMovies();


    private:
        LevelManager();
    };
}

