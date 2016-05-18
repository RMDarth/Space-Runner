#include "Scores.h"
#include "FileSystem.h"
#include "Game/StateProcessors/Race/LevelFileManager.h"
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

bool Scores::UpdatePuzzleScore(int level, int missedEnergy)
{
    level--;
    bool result = false;
    if (bestPuzzleMisses[level] > missedEnergy)
    {
        result = true;
        bestPuzzleMisses[level] = missedEnergy;

        bestPuzzleStars[level] = 1;
        if (missedEnergy <= averageMissingEnergy[level])
        {
            bestPuzzleStars[level] = 2;
        }
        if (missedEnergy <= minMissingEnergy[level])
        {
            bestPuzzleStars[level] = 3;
        }
    }

    if (result)
        Save();

    return result;
}

bool Scores::UpdateArcadeScore(int time, int score)
{
    bool result = false;
    if (bestArcadeScore < score)
    {
        result = true;
        bestArcadeScore = score;
    }

    if (bestArcadeTime < time)
    {
        result = true;
        bestArcadeTime = time;
    }

    if (result)
        Save();

    return result;
}

void Scores::UpdateTotalEnergy(int collected)
{
    totalEnergy += collected;
    Save();
}

void Scores::Reset()
{
    CreateDefault();
}

int Scores::GetTotalEnergy()
{
    return totalEnergy;
}

int Scores::GetBestArcadeScore()
{
    return bestArcadeScore;
}

int Scores::GetBestArcadeTime()
{
    return bestArcadeTime;
}

int Scores::GetBestStars(int level)
{
    return bestPuzzleStars[level-1];
}

int Scores::GetStars(int level, int missedEnergy)
{
    if (missedEnergy <= minMissingEnergy[level-1])
        return 3;
    if (missedEnergy <= averageMissingEnergy[level-1])
        return 2;
    return 1;
}

int Scores::GetBestPuzzleMissingEnergy(int level)
{
    return bestPuzzleMisses[level - 1];
}

void Scores::SetChallengeCompleted(int num)
{
    challengeCompleted[num - 1] = true;
    Save();
}

bool Scores::IsChallengeCompleted(int num)
{
    return challengeCompleted[num - 1];
}

int getDaysTillNow()
{
    time_t timer;
    struct tm y2k = {0};
    double seconds;

    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 110; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time(&timer);  /* get current time; same as: timer = time(NULL)  */

    return (int)difftime(timer,mktime(&y2k)) / (60 * 60 * 24);
}

void Scores::SetDailyChallengeCompleted(int num)
{
    dailyChallengeLastCompleted[num] = getDaysTillNow();
    Save();
}

bool Scores::IsDailyChallengeCompleted(int id)
{
    int days = getDaysTillNow();
    return dailyChallengeLastCompleted[id] >= days;
}

void Scores::Load()
{
    SecureFileInputRef* scoresFile = new SecureFileInputRef("scores.dat");
    if (!scoresFile->IsOpened())
    {
        delete scoresFile;
        CreateDefault();

        scoresFile = new SecureFileInputRef("scores.dat");
    }
    totalEnergy = scoresFile->ReadInt();
    bestArcadeTime = scoresFile->ReadInt();
    bestArcadeScore = scoresFile->ReadInt();
    for (int i = 0; i < LEVELNUM; i++)
    {
        bestPuzzleMisses[i] = scoresFile->ReadInt();
        bestPuzzleStars[i] = scoresFile->ReadInt();
    }

    for (int i = 0; i < CHALLENGENUM; i++)
    {
        challengeCompleted[i] = scoresFile->ReadInt() == 1;
    }

    dailyChallengeLastCompleted[0] = scoresFile->ReadInt();
    dailyChallengeLastCompleted[1] = scoresFile->ReadInt();

    scoresFile->Close();

    delete scoresFile;
}

void Scores::LoadLevelInfo()
{
    for (int i = 0; i < LEVELNUM; i++)
    {
        auto level = CoreEngine::LevelFileManager::Instance()->LoadLevel(i + 1);
        minMissingEnergy[i] = level->minMissingEnergy;
        averageMissingEnergy[i] = level->avgMissingEnergy;

        delete level;
    }
}

void Scores::Save()
{
    SecureFileOutputRef scoresFile("scores.dat");
    scoresFile.WriteInt(totalEnergy);
    scoresFile.WriteInt(bestArcadeTime);
    scoresFile.WriteInt(bestArcadeScore);
    for (int i = 0; i < LEVELNUM; i++)
    {
        scoresFile.WriteInt(bestPuzzleMisses[i]);
        scoresFile.WriteInt(bestPuzzleStars[i]);
    }

    for (int i = 0; i < CHALLENGENUM; i++)
    {
        scoresFile.WriteInt(challengeCompleted[i] ? 1 : 0);
    }

    scoresFile.WriteInt(dailyChallengeLastCompleted[0]);
    scoresFile.WriteInt(dailyChallengeLastCompleted[1]);

    scoresFile.Close();
}

void Scores::CreateDefault()
{
    for (int i = 0; i < LEVELNUM; i++)
    {
        bestPuzzleMisses[i] = INT_MAX;
        bestPuzzleStars[i] = 0;
    }
    for (auto i = 0; i < CHALLENGENUM; i++)
    {
        challengeCompleted[i] = false;
    }
    dailyChallengeLastCompleted[0] = 0;
    dailyChallengeLastCompleted[1] = 0;

    bestArcadeScore = 0;
    bestArcadeTime = 0;
    totalEnergy = 0;
    Save();
}









