#include "AsteroidProcessor.h"
#include "Asteroid.h"

namespace CoreEngine
{
    AsteroidProcessor::AsteroidProcessor(PtrList<Asteroid>& asteroidList, float& lastObstacleCreated, LevelDifficulty difficulty)
        : _asteroidList(asteroidList)
        , _lastObstacleCreated(lastObstacleCreated)
        , _difficulty(difficulty)
    {
    }

    void AsteroidProcessor::Process(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 4)
        {
            bool posUsed[3] = { false, false, false };

            int count = rand() % 2 + 1;
            int8_t curPoses = 0;

            if (_lastRows.empty())
                count = 0;

            for (int i = 0; i < count; i++)
            {
                int posIndex;
                do{
                    posIndex = rand() % 3;
                } while (posUsed[posIndex]);

                posUsed[posIndex] = true;
                curPoses |= 1 << posIndex;

                // Hard check
                if (i == 1 && CountCentralDoubleBlocks() == 2 && curPoses != _lastRows.front())
                    break;

                // Normal check
                if (_difficulty <= LevelDifficulty::Normal && !isNormalMovePossible(curPoses))
                    break;

                float deviation = (rand() % 10 - 5.0f) / 10.0f;
                float pos = presetPos[posIndex] + deviation;

                auto * asteroid = new Asteroid(
                        Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos),
                        Asteroid::getAsteroidName(rand() % 6 + 1),
                        0.0f,
                        5.0f);
                _asteroidList.push_back(std::shared_ptr<Asteroid>(asteroid));
            }

            _lastRows.push_front(curPoses);
            if (_lastRows.size() > 4)
                _lastRows.pop_back();

            _lastObstacleCreated = totalTime;
        }
    }

    int AsteroidProcessor::CountCentralDoubleBlocks()
    {
        int count = 0;

        for (auto i = 0; i < _lastRows.size(); i++)
        {
            int aNum = 0;
            for (auto r = 0; r < 3; r++)
                if (_lastRows[i] & (1 << r)) aNum++;

            if (aNum < 2 || !(_lastRows[i] & (1 << 1)))
                break;

            if (count == 0)
            {
                count++;
            }
            else if (_lastRows[i] != _lastRows[i-1])
            {
                count++;
            } else
            {
                break;
            }
        }
        return count;
    }

    bool AsteroidProcessor::isNormalMovePossible(int8_t poses)
    {
        return true;
    }


}