#include <Game/StateProcessors/Race/PrefabManager.h>
#include <Game/StateProcessors/Race/LevelFileManager.h>
#include <Game/StateProcessors/Level/LevelManager.h>
#include "Space.h"
#include "SpaceDust.h"
#include "Fence.h"
#include "Asteroid.h"
#include "AsteroidProcessor.h"
#include "Mine.h"
#include "Cruiser.h"
#include "BlasterBurst.h"
#include "Missile.h"
#include "Explosion.h"
#include "Sparks.h"
#include "EnergyOrb.h"
#include "Barrier.h"
#include "Boss.h"
#include "BigBoss.h"

using namespace std;

namespace CoreEngine
{
    Space::Space()
    {
        _lastObstacleCreated = 0;
        _spaceDust = make_unique<SpaceDust>("SpaceDust");
        _fence = make_unique<Fence>();
        _totalTime = 0;
        _missedOrbs = 0;
        LevelManager::Instance()->SetMissed(_missedOrbs);
        _lastObstaclePos = 0;
    }

    Space::~Space()
    {

    }

    void Space::GenerateSpace()
    {
        for (int i = 0; i < ASTEROID_NUM; i++)
        {
            float posX = 0;
            while (posX > -BLOCK_SIZE*15.0f && posX < BLOCK_SIZE*15.0f)
            {
                posX = (rand() % (int)(BLOCK_SIZE * 50.0f * 10)) / 10.0f - BLOCK_SIZE * 25.0f;
            }
            auto posY = (rand() % (int)(BLOCK_SIZE * 40.0f * 10)) / 10.0f - BLOCK_SIZE * 20.0f;

            int num = rand() % 6 + 1;
            auto asteroid = make_shared<Asteroid>(Vector3(-i*BLOCK_SIZE * 10, posY, posX), Asteroid::getAsteroidName(num), 0.0f, 90.0, false);
            std::stringstream ss;
            ss << "asteroid" << num << "_Dark";
            asteroid->SetMaterial(ss.str());
            _backgroundAsteroidList.push_back(asteroid);
        }

        if (LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
        {
            auto levelNum = LevelManager::Instance()->GetLevelNum();
            _currentLevel = unique_ptr<Level>(LevelFileManager::Instance()->LoadLevel(levelNum));
            RenderProcessor::Instance()->SetSkybox(levelNum % SKYBOX_NUM + 1);
        }
        else if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
        {
            _currentLevelDifficulty = LevelDifficulty::Easy;
            GenerateLevel(LevelDifficulty::Easy, 15);
            RenderProcessor::Instance()->SetSkybox(rand() % SKYBOX_NUM + 1);
        } else
        {
            _currentLevelDifficulty = LevelDifficulty::Hard;
            auto levelNum = LevelManager::Instance()->GetLevelNum();
            _currentLevel = unique_ptr<Level>(LevelFileManager::Instance()->LoadChallenge(levelNum));
            RenderProcessor::Instance()->SetSkybox(rand() % SKYBOX_NUM + 1);
        }

        //LevelFileManager::Instance()->SaveLevel(_currentLevel.get(), 11);
    }

    void Space::GenerateLevel(LevelDifficulty difficulty, int count)
    {
        vector<ObstacleType> obstacleList;
        vector<PrefabInfo> prefabList;
        for (int i = 0; i < count; i++)
        {
            //obstacleList.push_back(ObstacleType::BigBoss); continue;
            int type = rand() % 18;
            if (type < 2)
            {
                obstacleList.push_back(ObstacleType::EnergyBarrier);
            }
            else if (type < 4)
            {
                obstacleList.push_back(ObstacleType::Mine);
            }
            else if (type < 6)
            {
                obstacleList.push_back(ObstacleType::EnemyCruiser);
            }
            else if (type < 8)
            {
                obstacleList.push_back(ObstacleType::EnergyOrb);
            }
            else if (type < 10)
            {
                string difficultyStr;
                switch (difficulty)
                {
                    case LevelDifficulty::Easy:
                        difficultyStr = "easy";
                        break;
                    case LevelDifficulty::Normal:
                        difficultyStr = "normal";
                        break;
                    case LevelDifficulty::Hard:
                        difficultyStr = "hard";
                        break;
                }

                int num = (int)PrefabManager::Instance()->getPrefabList(difficultyStr).size();
                obstacleList.push_back(ObstacleType::Prefab);
                PrefabInfo prefabInfo { difficultyStr, rand() % num + 1 };
                prefabList.push_back(std::move(prefabInfo));
            }
            else
            {
                obstacleList.push_back(ObstacleType::AsteroidsPack);
            }
        }
        if (_currentLevelDifficulty == LevelDifficulty::Normal)
            obstacleList.push_back(ObstacleType::BigBoss);
        else if (_currentLevelDifficulty == LevelDifficulty::Hard)
            obstacleList.push_back(rand() % 2 ? ObstacleType::Boss : ObstacleType::BigBoss);

        _currentLevel = make_unique<Level>();
        _currentLevel->currentObstacle = 0;
        _currentLevel->currentPrefab = 0;
        _currentLevel->obstacleList = obstacleList;
        _currentLevel->prefabList = prefabList;
        _currentLevel->skyboxId = 0;
        _currentLevel->difficulty = difficulty;
        if (_currentLevelDifficulty == LevelDifficulty::Normal)
            _currentLevel->bossDifficulty = static_cast<BossInterface::Difficulty>(rand() % 2);
        else
            _currentLevel->bossDifficulty = static_cast<BossInterface::Difficulty>(rand() % 3);

        _currentLevel->energyToComplete = 100;
    }

    void Space::Update(float time, float speed)
    {
        _totalTime += time * speed;
        float roadOffset = time * speed * 5.0f;

        _spaceDust->Update(time, roadOffset);
        _fence->Update(time, roadOffset);

        AddObstacles(_totalTime);
        UpdateShots(time, roadOffset);

        for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::TryReset, placeholders::_1));
        for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));

        for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::Update, placeholders::_1, time, roadOffset));
        _asteroidList.erase(remove_if(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::IsDone, placeholders::_1)), _asteroidList.end());

        for_each(_missileList.begin(), _missileList.end(), bind(&Missile::Update, placeholders::_1, time, roadOffset));
        _missileList.erase(remove_if(_missileList.begin(), _missileList.end(), bind(&Missile::IsDone, placeholders::_1)), _missileList.end());

        for_each(_mineList.begin(), _mineList.end(), bind(&Mine::Update, placeholders::_1, time, roadOffset));
        _mineList.erase(remove_if(_mineList.begin(), _mineList.end(), bind(&Mine::IsDone, placeholders::_1)), _mineList.end());

        for_each(_cruiserList.begin(), _cruiserList.end(), bind(&Cruiser::Update, placeholders::_1, time, roadOffset));
        _cruiserList.erase(remove_if(_cruiserList.begin(), _cruiserList.end(), bind(&Cruiser::IsDone, placeholders::_1)), _cruiserList.end());

        for_each(_explosionList.begin(), _explosionList.end(), bind(&Explosion::Update, placeholders::_1, time, roadOffset));
        _explosionList.erase(remove_if(_explosionList.begin(), _explosionList.end(), bind(&Explosion::IsDone, placeholders::_1)), _explosionList.end());

        for_each(_sparksList.begin(), _sparksList.end(), bind(&Sparks::Update, placeholders::_1, time, roadOffset));
        _sparksList.erase(remove_if(_sparksList.begin(), _sparksList.end(), bind(&Sparks::IsDone, placeholders::_1)), _sparksList.end());

        for_each(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::Update, placeholders::_1, time, roadOffset));
        auto missedOrbs = count_if(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::Passed, placeholders::_1));
        if (missedOrbs > 0)
        {
            _missedOrbs += missedOrbs;
            LevelManager::Instance()->SetMissed(_missedOrbs);
        }
        _orbList.erase(remove_if(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::IsDone, placeholders::_1)), _orbList.end());

        for_each(_barrierList.begin(), _barrierList.end(), bind(&Barrier::Update, placeholders::_1, time, roadOffset));
        _barrierList.erase(remove_if(_barrierList.begin(), _barrierList.end(), bind(&Barrier::IsDone, placeholders::_1)), _barrierList.end());

        if (_boss)
        {
            _boss->Update(time, roadOffset);
            if (_boss->IsDone())
                _boss.reset();
        }
    }

    void Space::AddObstacles(float totalTime)
    {
        if (!_currentObstacle || (totalTime - _currentObstacle->timeStarted) > _currentObstacle->timeLength )
        {
            // create new obstacle
            _currentLevel->currentObstacle++;
            if (_currentLevel->currentObstacle == _currentLevel->obstacleList.size())
            {
                _currentLevel->currentObstacle = 0;
                if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
                {
                    if (_currentLevelDifficulty != LevelDifficulty::Hard)
                        _currentLevelDifficulty = static_cast<LevelDifficulty>((int)_currentLevelDifficulty + 1);
                    int count = 30;
                    if (_currentLevelDifficulty == LevelDifficulty::Hard)
                        count = rand() % 15 + 30;

                    GenerateLevel(_currentLevelDifficulty, count);
                }
            }

            switch (_currentLevel->obstacleList[_currentLevel->currentObstacle])
            {
            case ObstacleType::AsteroidsPack:
                {
                    int count = rand() % 5 + 2;
                    _lastObstacleCreated = totalTime;

                    _currentObstacle = make_unique<Obstacle>(
                        ObstacleType::AsteroidsPack,
                        count * 4 + 1,
                        totalTime,
                        count);

                    _asteroidProcessor = make_shared<AsteroidProcessor>(_asteroidList, _lastObstacleCreated, _currentLevel->difficulty);
                }
                break;

            case ObstacleType::Mine:
                _currentObstacle = make_unique<Obstacle>(
                    ObstacleType::Mine,
                    25,
                    totalTime,
                    0);
                break;

            case ObstacleType::EnemyCruiser:
                _currentObstacle = make_unique<Obstacle>(
                        ObstacleType::EnemyCruiser,
                        25,
                        totalTime,
                        0);
                break;

            case ObstacleType::EnergyOrb:
                _lastObstaclePos = rand() % 3;
                _lastPosChange = totalTime;
                _currentObstacle = make_unique<Obstacle>(
                    ObstacleType::EnergyOrb,
                    20,
                    totalTime,
                    0);

                break;

            case ObstacleType::EnergyBarrier:
                _currentObstacle = make_unique<Obstacle>(
                    ObstacleType::EnergyBarrier,
                    15,
                    totalTime,
                    0);
                break;

            case ObstacleType::Prefab:
                {
                    _lastObstaclePos = 0;
                    _lastPosChange = -1;
                    const auto& currentPrefabInfo = _currentLevel->prefabList[_currentLevel->currentPrefab];

                    _currentLevel->currentPrefab++;
                    if (_currentLevel->currentPrefab >= _currentLevel->prefabList.size())
                        _currentLevel->currentPrefab = 0;

                    _currentPrefab = PrefabManager::Instance()->getPrefabList(currentPrefabInfo.category).at(currentPrefabInfo.prefabNum - 1);
                    _currentPrefab->setInverted(_currentPrefab->isMirrorable() && rand()%2 == 0);

                    _currentObstacle = make_unique<Obstacle>(
                            ObstacleType::Prefab,
                            _currentPrefab->getRowList().size() * 4 + 1,
                            totalTime,
                            0);
                    break;
                }

            case ObstacleType::Boss:
                _currentObstacle = make_unique<Obstacle>(
                        ObstacleType::Boss,
                        5000,
                        totalTime,
                        0);
                break;

            case ObstacleType::BigBoss:
                _currentObstacle = make_unique<Obstacle>(
                        ObstacleType::BigBoss,
                        5000,
                        totalTime,
                        0);
                break;
            }
        }
        else
        {
            switch (_currentObstacle->type)
            {
                case ObstacleType::AsteroidsPack:
                    ProcessAsteroids(totalTime);
                    break;

                case ObstacleType::EnergyOrb:
                    ProcessEnergyOrbs(totalTime);
                    break;

                case ObstacleType::Mine:
                    ProcessMine(totalTime);
                    break;

                case ObstacleType::EnemyCruiser:
                    ProcessEnemyCruiser(totalTime);
                    break;

                case ObstacleType::EnergyBarrier:
                    ProcessEnergyBarrier(totalTime);
                    break;

                case ObstacleType::Prefab:
                    ProcessPrefabs(totalTime);
                    break;

                case ObstacleType::Boss:
                    ProcessBoss(totalTime);
                    break;

                case ObstacleType::BigBoss:
                    ProcessBigBoss(totalTime);
                    break;
            }
        }
    }

    void Space::AddShot(Vector3 pos, float speed)
    {
        auto shot = make_shared<BlasterBurst>(pos, "BlasterShotMaterial", speed);
        _shotList.push_back(shot);
    }

    void Space::PreloadModels()
    {
        Vector3 zero;

        for (auto i = 1; i <= 6; i++)
        {
            auto * asteroid = new Asteroid(zero, Asteroid::getAsteroidName(i), 0, 0);
            delete asteroid;
        }
        auto* mine = new Mine(zero, 0);
        delete mine;

        auto* cruiser = new Cruiser(zero, 0);
        delete cruiser;

        auto* missile = new Missile(zero, 0);
        delete missile;

        auto* boss = new Boss(zero);
        delete boss;

        auto* bigBoss = new BigBoss(zero);
        delete bigBoss;

        auto * blasterBurst = new BlasterBurst(zero, "BlasterShotMaterial", 80);
        blasterBurst->Destroy();
        blasterBurst->SetVisible(false);
        _shotList.push_back(shared_ptr<BlasterBurst>(blasterBurst));

        auto * sparks = new Sparks(zero);
        sparks->Destroy();
        sparks->SetVisible(false);
        _sparksList.push_back(shared_ptr<Sparks>(sparks));

        auto * orb = new EnergyOrb(zero);
        orb->Destroy();
        orb->SetVisible(false);
        _orbList.push_back(shared_ptr<EnergyOrb>(orb));

        auto * explosion = new Explosion(zero);
        explosion->Destroy();
        explosion->SetVisible(false);
        _explosionList.push_back(shared_ptr<Explosion>(explosion));

        auto * barrier = new Barrier(zero);
        barrier->Destroy();
        barrier->SetVisible(false);
        _barrierList.push_back(shared_ptr<Barrier>(barrier));
    }

    void Space::RegisterShotEvent(EventCallback callback)
    {
        _shotCallback = callback;
    }

    void Space::RegisterBossEvent(BossCallback callback)
    {
        _bossCallback = callback;
    }

    Level* Space::GetCurrentLevel()
    {
        return _currentLevel.get();
    }

    void Space::ProcessAsteroids(float totalTime)
    {
        _asteroidProcessor->Process(totalTime);
    }

    void Space::ProcessMine(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 9)
        {
            int posIndex = rand() % 3;
            float pos = presetPos[posIndex];

            auto mine = make_shared<Mine>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), 0.2f);
            _mineList.push_back(mine);

            _lastObstacleCreated = totalTime + 10;
        }
    }

    void Space::ProcessEnemyCruiser(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 9)
        {
            float pos = presetPos[1];

            auto cruiser = make_shared<Cruiser>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos), 0.0f, 2.0f);
            _cruiserList.push_back(cruiser);

            bool posUsed[3] = { false, false, false };

            int count;
            if (_currentLevel->difficulty == LevelDifficulty::Easy)
                count = rand() % 2;
            else if (_currentLevel->difficulty == LevelDifficulty::Normal)
                count = rand() % 2 + 1;
            else
                count = rand() % 3 + 1;

            for (int i = 0; i < count; i++)
            {
                int posIndex;
                do
                {
                    posIndex = rand() % 3;
                } while (posUsed[posIndex]);
                posUsed[posIndex] = true;
                auto missilePos = presetPos[posIndex];

                auto missile = make_shared<Missile>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, missilePos), -12.0f - rand()%6 , 0.25f);
                _missileList.push_back(missile);
            }

            _lastObstacleCreated = totalTime + 10;
        }
    }


    void Space::ProcessEnergyOrbs(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 2.0f)
        {
            float pos;
            if (rand() % 3 == 0 && totalTime - _lastPosChange > 10)
            {
                if (_lastObstaclePos == 0
                    || (rand() % 2 == 0 && _lastObstaclePos != 2))
                {
                    pos = presetPos[_lastObstaclePos];
                    pos += (presetPos[_lastObstaclePos + 1] - presetPos[_lastObstaclePos]) * 0.5f;
                    _lastObstaclePos++;
                }
                else
                {
                    pos = presetPos[_lastObstaclePos];
                    pos -= (presetPos[_lastObstaclePos] - presetPos[_lastObstaclePos - 1]) * 0.5f;
                    _lastObstaclePos--;
                }

                _lastPosChange = totalTime;
            }
            else
            {
                pos = presetPos[_lastObstaclePos];
            }

            _orbList.push_back(make_shared<EnergyOrb>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, pos)));
            _lastObstacleCreated = totalTime;
        }
    }

    void Space::ProcessEnergyBarrier(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 9)
        {
            auto barrier = make_shared<Barrier>(Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, 0));
            _barrierList.push_back(barrier);

            _lastObstacleCreated = totalTime + 6;
        }
    }

    void Space::ProcessPrefabs(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 4)
        {
            if (_lastObstaclePos >= _currentPrefab->getRowList().size())
                return;

            const auto& row = _currentPrefab->getRowList()[_lastObstaclePos];
            _lastObstaclePos++;

            for (auto i = 0; i < 3; i++)
            {
                Vector3 pos(-ASTEROID_NUM * BLOCK_SIZE, 0, presetPos[2-i]);
                if (_currentPrefab->isInverted())
                    pos = Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, presetPos[i]);

                switch (row.objects[i].type)
                {
                    case SpaceObjectType::Asteroid:
                    {
                        auto asteroid = make_shared<Asteroid>(
                                pos,
                                Asteroid::getAsteroidName(rand() % 6 + 1),
                                row.objects[i].speed,
                                5.0f);
                        _asteroidList.push_back(std::move(asteroid));
                        break;
                    }
                    case SpaceObjectType::Mine:
                    {
                        auto mine = make_shared<Mine>(pos, row.objects[i].speed);
                        _mineList.push_back(std::move(mine));
                        break;
                    }
                    case SpaceObjectType::EnemyCruiser:
                        _cruiserList.push_back(make_shared<Cruiser>(pos, row.objects[i].speed, 2.0f));
                        break;
                    case SpaceObjectType::EnergyOrb:
                        _orbList.push_back(make_shared<EnergyOrb>(pos));
                        _lastPosChange = _totalTime;
                        break;
                    case SpaceObjectType::Barrier:
                        _barrierList.push_back(make_shared<Barrier>(pos));
                        break;
                    case SpaceObjectType::Missile:
                        _missileList.push_back(make_shared<Missile>(pos, -12.0f - rand()%6 , 0.25f));
                        break;
                    case SpaceObjectType::None:
                        break;
                }
            }

            _lastObstacleCreated = totalTime;
        }
        else if (totalTime > 3 && totalTime - _lastObstacleCreated > 2)
        {
            ProcessPrefabEnergyOrbs(totalTime);
        }
    }

    void Space::ProcessPrefabEnergyOrbs(float totalTime)
    {
        // Adding interim energy orbs
        if (_lastPosChange > 0 && totalTime - _lastPosChange > 2)
        {
            const auto& row = _currentPrefab->getRowList()[_lastObstaclePos - 1];
            for (auto i = 0; i < 3; i++)
            {
                int id = 2 - i;
                if (_currentPrefab->isInverted())
                    id = i;
                Vector3 pos(-ASTEROID_NUM * BLOCK_SIZE, 0, presetPos[id]);

                if (row.objects[i].type == SpaceObjectType::EnergyOrb)
                {
                    if (_lastObstaclePos == _currentPrefab->getRowList().size())
                    {
                        // for last row add in the same spot
                        _orbList.push_back(make_shared<EnergyOrb>(pos));
                    } else {
                        // for other cases create way to next one
                        const auto& nextrow = _currentPrefab->getRowList()[_lastObstaclePos];
                        bool found = false;
                        for (auto r = 0; r < 3; r++)
                        {
                            int idNext = 2 - r;
                            if (_currentPrefab->isInverted())
                                idNext = r;

                            if (nextrow.objects[r].type == SpaceObjectType::EnergyOrb)
                            {
                                found = true;
                                if (id < idNext)
                                {
                                    _orbList.push_back(make_shared<EnergyOrb>(Vector3(pos.x, pos.y, pos.z - (presetPos[id] - presetPos[idNext])*0.5f)));
                                    break;
                                }
                                else if (id == idNext)
                                {
                                    _orbList.push_back(make_shared<EnergyOrb>(pos));
                                    break;
                                }
                                else
                                {
                                    _orbList.push_back(make_shared<EnergyOrb>(Vector3(pos.x, pos.y, pos.z + (presetPos[idNext] - presetPos[id])*0.5f)));
                                    break;
                                }
                            }
                        }
                        if (!found)
                        {
                            _orbList.push_back(make_shared<EnergyOrb>(pos));
                        }
                    }
                }
            }
            _lastPosChange = totalTime;
        }
    }

    void Space::ProcessBoss(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 9 && !_boss)
        {
            _boss = make_shared<Boss>(
                    Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, presetPos[1]),
                    50,
                    _currentLevel->bossDifficulty);
            _bossCallback(50, 50);

            _lastObstacleCreated = totalTime + 10;
        }

        float period = _boss && _boss->GetDifficulty() != Boss::Difficulty::Hard ? 1.5f : 0.4f;
        if (_boss && totalTime - _lastObstacleCreated > period * 10)
        {
            Vector3 missilePos = _boss->getPos();
            missilePos.x += 0.1f;

            float speed;
            if (_boss->GetDifficulty() == BigBoss::Difficulty::Easy)
            {
                speed = 0;
            } else {
                speed = -18.0f - rand()%6;
            }

            auto missile = make_shared<Missile>(missilePos, speed, 0.25f);
            _missileList.push_back(missile);
            _lastObstacleCreated = totalTime;
        }
    }

    void Space::ProcessBigBoss(float totalTime)
    {
        if (totalTime > 3 && totalTime - _lastObstacleCreated > 9 && !_boss)
        {
            _boss = make_shared<BigBoss>(
                    Vector3(-ASTEROID_NUM * BLOCK_SIZE, 0, presetPos[1]),
                    50,
                    _currentLevel->bossDifficulty);
            _bossCallback(50, 50);

            _lastObstacleCreated = totalTime + 10;
        }

        float period = _boss && _boss->GetDifficulty() == BigBoss::Difficulty::Easy ? 1.5f : 1.0f;
        if (_boss && totalTime - _lastObstacleCreated > period * 10)
        {
            float speed;
            if (_boss->GetDifficulty() == BigBoss::Difficulty::Easy)
            {
                speed = 15;
            } else {
                speed = -10;
            }

            if (_boss->GetDifficulty() == BigBoss::Difficulty::Hard)
            {
                int rocketPos = rand() % 4 + 3;
                for (auto r = 0; r < 3; r++)
                {
                    if (rocketPos & (1<<r))
                    {
                        Vector3 missilePos = _boss->getPos();
                        missilePos.z = presetPos[r];
                        missilePos.x += 0.1f;
                        auto missile = make_shared<Missile>(missilePos, speed, 0.25f);
                        _missileList.push_back(missile);
                    }
                }
            }
            else
            {
                Vector3 missilePos = _boss->getPos();
                missilePos.z = presetPos[rand() % 3];
                missilePos.x += 0.1f;

                auto missile = make_shared<Missile>(missilePos, speed, 0.25f);
                _missileList.push_back(missile);
            }
            _lastObstacleCreated = totalTime;
        }
    }

    void Space::UpdateShots(float time, float roadOffset)
    {
        for_each(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::Update, placeholders::_1, time, roadOffset));

        for (auto& shot : _shotList)
        {
            for (auto& mine : _mineList)
            {
                if (!mine->IsDone() && shot->IsIntersected(mine.get()))
                {
                    shot->Destroy();
                    mine->Destroy();
                    _explosionList.push_back(make_shared<Explosion>(mine->getPos()));
                    if (_shotCallback)
                        _shotCallback(SpaceObjectType::Mine);
                }
            }

            for (auto& cruiser : _cruiserList)
            {
                if (!cruiser->IsDone() && shot->IsIntersected(cruiser.get()))
                {
                    shot->Destroy();
                    cruiser->Destroy();
                    //_explosionList.push_back(make_shared<Explosion>(cruiser->getPos(), 3.0f));
                    for (float offset = presetPos[0]; offset <= presetPos[2] + 1; offset += presetPos[2]/2)
                    {
                        _explosionList.push_back(make_shared<Explosion>(Vector3(cruiser->getPos().x, cruiser->getPos().y, offset), true));
                    }

                    if (_shotCallback)
                        _shotCallback(SpaceObjectType::EnemyCruiser);
                }
            }

            for (auto& asteroid : _asteroidList)
            {
                if (shot->IsIntersected(asteroid.get()))
                {
                    shot->Destroy();
                    _sparksList.push_back(make_shared<Sparks>(shot->getPos()));
                    if (_shotCallback)
                        _shotCallback(SpaceObjectType::Asteroid);
                }
            }

            if (_boss && shot->IsIntersected(_boss.get()))
            {
                shot->Destroy();
                _boss->Hit();
                if (!_boss->IsDone())
                {
                    _sparksList.push_back(make_shared<Sparks>(shot->getPos(), -1.0f));
                } else {
                    _boss->Destroy();
                    if (_boss->GetType() == Boss::Type::Small)
                    {
                        _explosionList.push_back(make_shared<Explosion>(_boss->getPos()));
                    } else {
                        for (float offset = presetPos[0]; offset <= presetPos[2] + 1; offset += presetPos[2]/2)
                        {
                            _explosionList.push_back(make_shared<Explosion>(Vector3(_boss->getPos().x, _boss->getPos().y, offset), true));
                        }
                    }
                    _currentObstacle.reset();
                }

                if (_bossCallback)
                    _bossCallback(_boss->GetLives(), 50);
            }

            for (auto& explosion : _explosionList)
            {
                if (shot->IsIntersected(explosion.get()))
                {
                    shot->Destroy();
                }
            }
        }

        _shotList.erase(remove_if(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::IsDone, placeholders::_1)), _shotList.end());
    }

    void Space::DestroyEverything(bool near)
    {
        for (auto& asteroid : _asteroidList)
        {
            if (near && asteroid->getPos().x < -90)
                continue;
            asteroid->Destroy();
            _explosionList.push_back(make_shared<Explosion>(asteroid->getPos()));
        }
        for (auto& mine : _mineList)
        {
            if (near && mine->getPos().x < -90)
                continue;

            mine->Destroy();
            _explosionList.push_back(make_shared<Explosion>(mine->getPos()));
        }

        for (auto& cruiser : _cruiserList)
        {
            if (near && cruiser->getPos().x < -90)
                continue;

            cruiser->Destroy();
            for (float offset = presetPos[0]; offset <= presetPos[2] + 1; offset += presetPos[2]/2)
            {
                _explosionList.push_back(make_shared<Explosion>(Vector3(cruiser->getPos().x, cruiser->getPos().y, offset)));
            }
        }

        for_each(_shotList.begin(), _shotList.end(), bind(&SpaceObject::Destroy, placeholders::_1));
        for_each(_missileList.begin(), _missileList.end(), bind(&SpaceObject::Destroy, placeholders::_1));
        //for_each(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::SetVisible, placeholders::_1, visible));
        //for_each(_explosionList.begin(), _explosionList.end(), bind(&Explosion::SetVisible, placeholders::_1, visible));

        for (auto& barrier : _barrierList)
        {
            if (near && barrier->getPos().x < -90)
                continue;

            barrier->Destroy();
            _explosionList.push_back(make_shared<Explosion>(Vector3(barrier->getPos().x, barrier->getPos().y, presetPos[0] - presetPos[2]/2)));
            _explosionList.push_back(make_shared<Explosion>(Vector3(barrier->getPos().x, barrier->getPos().y, presetPos[2] + presetPos[2]/2)));
        }

        if (_boss && near)
        {
            for (auto i = 0; i < 12; i++)
                _boss->Hit();

            if (!_boss->IsDone())
            {
                _sparksList.push_back(make_shared<Sparks>(_boss->getPos(), -1.0f));
            } else {
                _boss->Destroy();
                if (_boss->GetType() == Boss::Type::Small)
                {
                    _explosionList.push_back(make_shared<Explosion>(_boss->getPos()));
                } else {
                    for (float offset = presetPos[0]; offset <= presetPos[2] + 1; offset += presetPos[2]/2)
                    {
                        _explosionList.push_back(make_shared<Explosion>(Vector3(_boss->getPos().x, _boss->getPos().y, offset)));
                    }
                }
                _currentObstacle.reset();
            }

            if (_bossCallback)
                _bossCallback(_boss->GetLives(), 50);
        }

        //for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
    }


    void Space::SetVisible(bool visible)
    {
        for_each(_asteroidList.begin(), _asteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
        for_each(_mineList.begin(), _mineList.end(), bind(&Mine::SetVisible, placeholders::_1, visible));
        for_each(_cruiserList.begin(), _cruiserList.end(), bind(&Cruiser::SetVisible, placeholders::_1, visible));
        for_each(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::SetVisible, placeholders::_1, visible));
        for_each(_missileList.begin(), _missileList.end(), bind(&Missile::SetVisible, placeholders::_1, visible));
        for_each(_orbList.begin(), _orbList.end(), bind(&EnergyOrb::SetVisible, placeholders::_1, visible));
        for_each(_explosionList.begin(), _explosionList.end(), bind(&Explosion::SetVisible, placeholders::_1, visible));
        for_each(_barrierList.begin(), _barrierList.end(), bind(&Barrier::SetVisible, placeholders::_1, visible));
        for_each(_backgroundAsteroidList.begin(), _backgroundAsteroidList.end(), bind(&Asteroid::SetVisible, placeholders::_1, visible));
        if (_boss)
            _boss->SetVisible(visible);

        _fence->SetVisible(visible);
        _spaceDust->SetVisible(visible);
    }

    SpaceObjectType Space::IsIntersected(float turn)
    {
        for (auto i = _asteroidList.begin(); i != _asteroidList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                (*i)->Destroy();
                return SpaceObjectType::Asteroid;
            }
        }

        for (auto i = _mineList.begin(); i != _mineList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                (*i)->Destroy();
                return SpaceObjectType::Mine;
            }
        }

        for (auto i = _cruiserList.begin(); i != _cruiserList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                (*i)->Destroy();
                return SpaceObjectType::EnemyCruiser;
            }
        }

        for (auto i = _missileList.begin(); i != _missileList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                (*i)->Destroy();
                (*i)->SetVisible(false);
                return SpaceObjectType::Missile;
            }
        }

        for (auto i = _orbList.begin(); i != _orbList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                (*i)->Destroy();
                return SpaceObjectType::EnergyOrb;
            }
        }

        for (auto i = _barrierList.begin(); i != _barrierList.end(); i++)
        {
            if ((*i)->IsIntersected(turn))
            {
                return SpaceObjectType::Barrier;
            }
        }
        return SpaceObjectType::None;
    }

    int Space::GetMissedOrbsCount()
    {
        return _missedOrbs;
    }



}