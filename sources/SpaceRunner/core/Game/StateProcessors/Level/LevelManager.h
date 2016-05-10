#pragma once
#include <string>

namespace CoreEngine
{
    class LevelManager
    {
        static LevelManager* _instance;

        int _level;
        int _score;
        int _missed;
        int _time;
        int _lives;

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

        bool IsPuzzle();
        bool IsStarted();
        bool IsVictory();
        bool IsMoviesLeft();

        void SetLevelNum(int num);
        void SetScore(int score);
        void SetMissed(int missed);
        void SetTime(int time);
        void SetIsPuzzle(bool puzzle);
        void SetStarted(bool started);
        void SetVictory(bool victory);
        void SetLives(int lives);
        void DecreaseLive();
        void IncreaseLive();
        void ResetMovies();
        void DecreaseMovies();


    private:
        LevelManager();
    };
}

