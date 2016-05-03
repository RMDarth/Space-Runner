#pragma once
#define LEVELNUM 11

class Scores
{
    static Scores* _instance;

    int minMissingEnergy[LEVELNUM];
    int averageMissingEnergy[LEVELNUM];

    int bestPuzzleStars[LEVELNUM];
    int bestPuzzleMisses[LEVELNUM];

    int bestArcadeTime;
    int bestArcadeScore;

    int totalEnergy;

public:
    static Scores* Instance();

    // returns true if new high scores
    bool UpdatePuzzleScore(int level, int missedEnergy);
    bool UpdateArcadeScore(int time, int score);
    void UpdateTotalEnergy(int collected);
    void Reset();

    int GetTotalEnergy();
    int GetBestArcadeScore();
    int GetBestArcadeTime();
    int GetBestStars(int level);
    int GetStars(int level, int missedEnergy);
    int GetBestPuzzleMissingEnergy(int level);

private:
    Scores();

    void LoadLevelInfo();
    void Load();
    void Save();

    void CreateDefault();
};

