#pragma once
#define LEVELNUM 100
#define MAXARCADE 7

class Scores
{
	static Scores* _instance;
	
	int minPuzzleTime[LEVELNUM];
	int averagePuzzleTime[LEVELNUM];
	
	int bestPuzzleTime[LEVELNUM];
	int bestPuzzleScore[LEVELNUM];
	int bestPuzzleStars[LEVELNUM];
	
	int bestArcadeTime[MAXARCADE];
	int bestArcadeScore[MAXARCADE];

public:
	static Scores* Instance();

	// returns true if new highscore
	bool UpdatePuzzleScore(int level, int time, int score);
	bool UpdateArcadeScore(int level, int time, int score);
	void Reset();
	
	int GetBestArcadeScore(int level);
	int GetBestArcadeTime(int level);
	int GetBestStars(int level);
	int GetBestPuzzleTime(int level);
	int GetStars(int level, int time);

private:
	Scores();
	
	void LoadLevelInfo();
	void Load();
	void SaveArcade();
	void SavePuzzle();

	void CreateDefault();
};

