#include "Scores.h"
#include "FileSystem.h"
#include <climits>

Scores* Scores::_instance = nullptr;

Scores::Scores()
{
	Load();
	LoadLevelInfo();
}

Scores* Scores::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new Scores();
	}
	return _instance;
}

bool Scores::UpdatePuzzleScore(int level, int time, int score)
{
	level--;
	bool result = false;
	if (bestPuzzleTime[level] > time)
	{
		result = true;
		bestPuzzleTime[level] = time;

		bestPuzzleStars[level] = 1;
		if (time <= averagePuzzleTime[level])
		{
			bestPuzzleStars[level] = 2;
		}
		if (time <= minPuzzleTime[level])
		{
			bestPuzzleStars[level] = 3;
		}
	}
	if (bestPuzzleScore[level] < score)
	{
		result = true;
		bestPuzzleScore[level] = score;
	}

	if (result)
		SavePuzzle();

	return result;
}

bool Scores::UpdateArcadeScore(int level, int time, int score)
{
	level--;
	bool result = false;
	if (bestArcadeScore[level] < score)
	{
		result = true;
		bestArcadeScore[level] = score;
	}

	if (level < 6 && bestArcadeTime[level] > time)
	{
		result = true;
		bestArcadeTime[level] = time;
	}

	// Endless
	if (level == 6 && (bestArcadeTime[level] < time || bestArcadeTime[level] == INT_MAX))
	{
		result = true;
		bestArcadeTime[level] = time;
	}

	if (result)
		SaveArcade();

	return result;
}

void Scores::Reset()
{
	CreateDefault();
}

int Scores::GetBestArcadeScore(int level)
{
	return bestArcadeScore[level-1];
}

int Scores::GetBestArcadeTime(int level)
{
	return bestArcadeTime[level-1];
}

int Scores::GetBestStars(int level)
{
	return bestPuzzleStars[level-1];
}

int Scores::GetStars(int level, int time)
{
	if (time <= minPuzzleTime[level-1])
		return 3;
	if (time <= averagePuzzleTime[level-1])
		return 2;
	return 1;
}

int Scores::GetBestPuzzleTime(int level)
{
	return bestPuzzleTime[level - 1];
}

void Scores::Load()
{
	int i;

	SecureFileInputRef *puzzlesFile = new SecureFileInputRef("puzzles.dat");
	if (!puzzlesFile->IsOpened())
	{
		delete puzzlesFile;
		CreateDefault();

		puzzlesFile = new SecureFileInputRef("puzzles.dat");
	}
	for (i = 0; i < LEVELNUM; i++)
	{
		bestPuzzleTime[i] = puzzlesFile->ReadInt();
		bestPuzzleScore[i] = puzzlesFile->ReadInt();
		bestPuzzleStars[i] = puzzlesFile->ReadInt();
	}
	puzzlesFile->Close();

	SecureFileInputRef * arcadeFile = new SecureFileInputRef("arcade.dat");
	for (i = 0; i < MAXARCADE; i++)
	{
		bestArcadeTime[i] = arcadeFile->ReadInt();
		bestArcadeScore[i] = arcadeFile->ReadInt();
	}
	arcadeFile->Close();

	delete puzzlesFile;
	delete arcadeFile;
}

void Scores::LoadLevelInfo()
{

}

void Scores::SavePuzzle()
{
	SecureFileOutputRef puzzlesFile("puzzles.dat");
	for (int i = 0; i < LEVELNUM; i++)
	{
		puzzlesFile.WriteInt(bestPuzzleTime[i]);
		puzzlesFile.WriteInt(bestPuzzleScore[i]);
		puzzlesFile.WriteInt(bestPuzzleStars[i]);
	}

	puzzlesFile.Close();
}

void Scores::SaveArcade()
{
	SecureFileOutputRef arcadeFile("arcade.dat");
	for (int i = 0; i < MAXARCADE; i++)
	{
		arcadeFile.WriteInt(bestArcadeTime[i]);
		arcadeFile.WriteInt(bestArcadeScore[i]);
	}

	arcadeFile.Close();
}

void Scores::CreateDefault()
{
	int i;
	for (i = 0; i < LEVELNUM; i++)
	{
		bestPuzzleTime[i] = INT_MAX;
		bestPuzzleScore[i] = 0;
		bestPuzzleStars[i] = 0;
	}
	SavePuzzle();

	for (i = 0; i < MAXARCADE; i++)
	{
		bestArcadeScore[i] = 0;
		bestArcadeTime[i] = INT_MAX;
	}
	bestArcadeTime[6] = 0;
	SaveArcade();
}
