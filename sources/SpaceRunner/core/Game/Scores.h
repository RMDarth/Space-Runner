#pragma once
#define LEVELNUM 11
#define CHALLENGENUM 6

class Scores
{
    static Scores* _instance;

    int minMissingEnergy[LEVELNUM];
    int averageMissingEnergy[LEVELNUM];

    int bestPuzzleStars[LEVELNUM];
    int bestPuzzleMisses[LEVELNUM];

    bool challengeCompleted[CHALLENGENUM];
    int dailyChallengeLastCompleted[2];

    int bestArcadeTime;
    int bestArcadeScore;

    int totalEnergy;

public:
    static Scores* Instance();

    // returns true if new high scores
    bool UpdatePuzzleScore(int level, int missedEnergy);
    bool UpdateArcadeScore(int time, int score);
    void SetChallengeCompleted(int num);
    void SetDailyChallengeCompleted(int num);
    void UpdateTotalEnergy(int collected);
    void Reset();

    int GetTotalEnergy();
    int GetBestArcadeScore();
    int GetBestArcadeTime();
    int GetBestStars(int level);
    int GetStars(int level, int missedEnergy);
    int GetBestPuzzleMissingEnergy(int level);
    bool IsChallengeCompleted(int num);
    bool IsDailyChallengeCompleted(int id);

private:
    Scores();

    void LoadLevelInfo();
    void Load();
    void Save();

    void CreateDefault();
};

