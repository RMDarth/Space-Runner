#include "OgreApplication.h"
#include "RaceStateProcessor.h"
#include "RenderProcessor.h"
#include "SoundSystem.h"
#include "Game/Game.h"
#include "Game/Config.h"
#include "Game/StateProcessors/Level/LevelManager.h"

#include "Game/StateProcessors/Race/Space/SpaceDust.h"
#include "Game/StateProcessors/Race/Space/Fence.h"
#include "Game/StateProcessors/Race/Space/Explosion.h"

#include "PrefabManager.h"

#include "Render/Drawables/ModelDrawable.h"
#include "Render/Drawables/RectDrawable.h"

#include <iomanip>
#include <Game/SkinManager.h>
#include <BillingProcessor.h>
#include <Game/Scores.h>

using namespace std;

#define CAMERA_SPEED 0.35f
#define PI 3.1415f

namespace CoreEngine
{
    namespace
    {
        bool between(float x, float y, float mid)
        {
            if (x > mid && y < mid)
                return true;
            if (x < mid && y > mid)
                return true;
            if (x == mid || y == mid)
                return true;
            return false;
        }
    } // anon namespace

    RaceStateProcessor::RaceStateProcessor()
    {
        _soundsLoaded = false;
        InitSound();

        PreloadModels();

        _document = make_unique<ControlDocument>("GUI/HUD.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        int c_x, c_y, c_width, c_height;
        RenderProcessor::Instance()->GetViewport()->getActualDimensions(c_x, c_y, c_width, c_height);
        _minSlideX = c_width * 0.01f;
        _minSlideY = c_height * 0.01f;

        _init = false;
    }

    void RaceStateProcessor::Init()
    {
        _space = make_unique<Space>();
        _space->GenerateSpace();
        _space->RegisterShotEvent(
            [this](SpaceObjectType type)
            {
                if (Config::Instance()->IsSoundEnabled())
                {
                    if (type == SpaceObjectType::Mine || type == SpaceObjectType::EnemyCruiser)
                        _bombSound->Play();
                    else
                    {
                        if (_totalTime - _impactSoundTime > 0.01f)
                        {
                            _impactSound->Play();
                            _impactSoundTime = _totalTime;
                        }
                    }
                }
                if (type == SpaceObjectType::Mine || type == SpaceObjectType::EnemyCruiser)
                    _score += 3 * _multiplier;
            });

        _space->RegisterBossEvent(
                [this](int lives, int maxlives)
                {
                    UpdateBossLifePanel(lives, maxlives);
                }
        );

        if (!_sector)
            InitSpaceShip();
        else
            ResetSpaceShip();

        if (_bigBomb)
        {
            _sector->GetNode()->removeChild(_bombSector->GetNode());
            _bigBomb.reset();
        }

        _totalTime = 0;
        _score = 0;
        _frames = 0;
        _speed = 1.0f;
        _speedAccel = 5.0f;

        InitMaxSpeed();

        _pos = 0;
        _angleHorizontal = 0;
        _invincibility = false;
        _shield = false;
        _shieldQueued = false;
        _sparks = false;
        _shootingStarted = 0;
        _shieldStart = 0;

        _presetPos[0] = -BLOCK_SIZE * 1.1f;
        _presetPos[1] = 0;
        _presetPos[2] = BLOCK_SIZE * 1.1f;

        _explosion.reset();

        _currentPosID = _targetPosID = _nextTargetPosID = 1;

        _camera = RenderProcessor::Instance()->GetCamera();

        _document->GetControlByName("bosslifebar")->SetDefaultMaterial("red.png", true);

        _init = true;
    }

    void RaceStateProcessor::ResetSpaceShip()
    {
        auto * sceneNode = _sector->GetNode();
        auto * sceneManager = RenderProcessor::Instance()->GetSceneManager();

        // update ship
        float scale = SkinManager::Instance()->GetShipScale();
        _sector->GetNode()->setScale(scale, scale, scale);
        _ship.reset();
        _ship = make_unique<ModelDrawable>(_shipSector.get(), SkinManager::Instance()->GetShipModelName());

        // update shield
        _shipShield = make_unique<ModelDrawable>(_shieldSector.get(), SkinManager::Instance()->GetShipModelName());
        _shipShield->SetMaterial("ShieldMaterial");
        _shipShield->SetScale(1.2f);
        _shieldEffectSector->GetNode()->setPosition(VectorToOgre(SkinManager::Instance()->GetShieldOffset()));

        // update sparks
        _sparksSector->GetNode()->setPosition(-1.65f/scale,0,0);

        // clean old engines
        sceneNode->removeChild(_engineFire[0]->getParentNode());
        _engineFire[0]->detachFromParent();
        sceneManager->destroyParticleSystem(_engineFire[0]);
        if (_engineFire[1])
        {
            sceneNode->removeChild(_engineFire[1]->getParentNode());
            _engineFire[1]->detachFromParent();
            sceneManager->destroyParticleSystem(_engineFire[1]);
        }

        // Create new engines
        auto sceneNodeChild = sceneManager->createSceneNode();
        sceneNodeChild->setPosition(VectorToOgre(SkinManager::Instance()->GetEngineOffset(0)));
        sceneNode->addChild(sceneNodeChild);

        _engineFire[0] = sceneManager->createParticleSystem("EngineFire", "Engine");
        sceneNodeChild->attachObject(_engineFire[0]);

        if (SkinManager::Instance()->GetEngineCount() > 1)
        {
            sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setPosition(VectorToOgre(SkinManager::Instance()->GetEngineOffset(1)));
            sceneNode->addChild(sceneNodeChild);

            _engineFire[1] = sceneManager->createParticleSystem("EngineFire2", "Engine");
            sceneNodeChild->attachObject(_engineFire[1]);
        } else {
            _engineFire[1] = nullptr;
        }
    }

    void RaceStateProcessor::InitSpaceShip()
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = make_unique<SceneSector>(sceneNode);
        float scale = SkinManager::Instance()->GetShipScale();
        sceneNode->setScale(scale, scale, scale);

        auto sceneNodeShip = sceneManager->createSceneNode();
        sceneNode->addChild(sceneNodeShip);
        _shipSector = make_unique<SceneSector>(sceneNodeShip);
        _ship = make_unique<ModelDrawable>(_shipSector.get(), SkinManager::Instance()->GetShipModelName());

        auto sceneNodeShieldModel = sceneManager->createSceneNode();
        //sceneNode->addChild(sceneNodeShieldModel);
        _shieldSector = make_unique<SceneSector>(sceneNodeShieldModel);
        _shipShield = make_unique<ModelDrawable>(_shieldSector.get(), SkinManager::Instance()->GetShipModelName());
        _shipShield->SetMaterial("ShieldMaterial");
        _shipShield->SetScale(1.2f);

        auto sceneNodeChild = sceneManager->createSceneNode();
        sceneNodeChild->setPosition(VectorToOgre(SkinManager::Instance()->GetEngineOffset(0)));
        sceneNode->addChild(sceneNodeChild);

        _engineFire[0] = sceneManager->createParticleSystem("EngineFire", "Engine");
        sceneNodeChild->attachObject(_engineFire[0]);

        if (SkinManager::Instance()->GetEngineCount() > 1)
        {
            sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setPosition(VectorToOgre(SkinManager::Instance()->GetEngineOffset(1)));
            sceneNode->addChild(sceneNodeChild);

            _engineFire[1] = sceneManager->createParticleSystem("EngineFire2", "Engine");
            sceneNodeChild->attachObject(_engineFire[1]);
        } else {
            _engineFire[1] = nullptr;
        }

        auto sceneNodeShieldEffect = sceneManager->createSceneNode();
        _shieldEffectSector = make_unique<SceneSector>(sceneNodeShieldEffect);
        sceneNodeShieldEffect->setPosition(VectorToOgre(SkinManager::Instance()->GetShieldOffset()));
        sceneNodeShieldEffect->setInheritScale(false);
        sceneNodeShieldEffect->setScale(5.0f, 5.0f, 5.0f);

        _shieldEffect = sceneManager->createParticleSystem("ShieldEffect", "ShieldAnim");
        _shieldEffect->setKeepParticlesInLocalSpace(true);
        sceneNodeShieldEffect->attachObject(_shieldEffect);

        auto sceneNodeSparks = sceneManager->createSceneNode();
        _sparksSector = make_unique<SceneSector>(sceneNodeSparks);
        sceneNodeSparks->setInheritScale(false);
        sceneNodeSparks->setScale(2.0f, 2.0f, 2.0f);
        _sparksSector->GetNode()->setPosition(-1.65f/scale,0,0);

        _sparksEffect = sceneManager->createParticleSystem("ShieldSparks", "ShotSparksReverse");
        _sparksEffect->setKeepParticlesInLocalSpace(true);
        sceneNodeSparks->attachObject(_sparksEffect);
    }

    void RaceStateProcessor::InitMaxSpeed()
    {
        switch(_space->GetCurrentLevel()->difficulty)
        {
            case LevelDifficulty::Easy:
                _speedMax = 8.0f;
                break;
            case LevelDifficulty::Normal:
                _speedMax = 10.0f;
                break;
            case LevelDifficulty::Hard:
                _speedMax = 11.0f;
                break;
        }
    }

    void RaceStateProcessor::InitSound()
    {
        auto soundSystem = SoundSystem::Instance();
        if (soundSystem->IsLoaded() && !_soundsLoaded)
        {
            _shootSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/LaserSound2.wav"));
            _impactSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/ImpactSound.wav"));
            _collectSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/CollectSound.wav"));

#ifdef __ANDROID_API__
            _bombSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/BombSound.ogg"));
            _superBombSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/SuperBombSound.ogg"));
            _successSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/SuccessSound.ogg"));
            _shieldSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/Shield.ogg"));
#else
            _bombSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/BombSound.wav"));
            _superBombSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/SuperBombSound.wav"));
            _successSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/SuccessSound.wav"));
            _shieldSound = unique_ptr<Sound>(soundSystem->CreateSound("Sound/Shield.wav"));
#endif
            _soundsLoaded = true;
        }
    }

    void RaceStateProcessor::PreloadModels()
    {
        _space = make_unique<Space>();
        _space->PreloadModels();
        _space->SetVisible(false);

        PrefabManager::Instance()->LoadPrefabs();
    }

    void RaceStateProcessor::UpdateTurn()
    {
        if (_currentPosID != _targetPosID)
        {
            auto curTime = _totalTime - _posChangingStarted;
            if (curTime > _posChangingTime)
            {
                _pos = _presetPos[_targetPosID];
                _currentPosID = _targetPosID;
                _angleHorizontal = 0;
                if (_nextTargetPosID != _targetPosID)
                {
                    _targetPosID = _nextTargetPosID;
                    _posChangingStarted = _totalTime;
                }
                return;
            }

            auto scaleTime = curTime * PI / _posChangingTime;

            auto k = 0.5f * (sin(scaleTime - PI*0.5f) + 1);
            _pos = _presetPos[_currentPosID] + k*(_presetPos[_targetPosID] - _presetPos[_currentPosID]);

            auto sign = _currentPosID < _targetPosID ? 1.0f : -1.0f;
            _angleHorizontal = sin(scaleTime) * _angleHorizontalMax * sign;
        }
    }

    GameState::State RaceStateProcessor::Update(float time)
    {
        _totalTime += time;
        _frames++;

        SetLightAndCamera();
        UpdateHUD();

        _speed += _speedAccel * time;
        if (_speed > _speedMax)
            _speed = _speedMax;

        if (_invincibility)
        {
            _sector->GetNode()->setVisible(true);
            if (_totalTime > _invincibilityStart + _invincibilityTime)
            {
                _invincibility = false;
                _ship->SetVisible(true);
            }
            else
            {
                auto invisTime = _totalTime - _invincibilityStart;
                auto koef = static_cast<int>(invisTime * 10);
                _ship->SetVisible(koef % 2 != 0);
            }
        }

        if (_shield)
        {
            if (_totalTime > _shieldStart + _shieldTime)
            {
                _shield = false;
                _sector->GetNode()->removeChild(_shieldSector->GetNode());
                _sector->GetNode()->removeChild(_shieldEffectSector->GetNode());
                if (_shieldQueued)
                {
                    OnKeyPressed(OIS::KC_DOWN);
                    _shieldQueued = false;
                }
            }
        }

        if (_sparks)
        {
            if (_totalTime > _sparksStart + _sparksTime)
            {
                _sparks = false;
                _sector->GetNode()->removeChild(_sparksSector->GetNode());
            }
        }

        if (_explosion)
        {
            _explosion->Update(time, _speed * time * 5.0f);
            if (_explosion->IsDone())
            {
                _explosion.reset();

                if (LevelManager::Instance()->GetLevelNum() == 1 && LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
                {
                    LevelManager::Instance()->SetStarted(false);
                    LevelManager::Instance()->SetLives(SkinManager::Instance()->LivesCount());
                    Show();
                    return Game::Instance()->GetState();
                }

                _invincibility = true;
                _invincibilityStart = _totalTime;
                _speedAccel = 5.0f;
                //_sector->GetNode()->setVisible(true);
            }
        }
        else
        {
            UpdateTurn();
        }

        if (_bigBomb)
            UpdateBomb(time * 5);

        _space->Update(time, _speed);
        _sector->GetNode()->setPosition(10, 0, _pos);

        auto angle = Ogre::Quaternion(Ogre::Radian(_angleHorizontal), Ogre::Vector3(1, 0, 0));
        _sector->GetNode()->setOrientation(angle);

        auto intersectedObject = _space->IsIntersected(_pos);
        if (!_invincibility && !_explosion && intersectedObject != SpaceObjectType::None && intersectedObject != SpaceObjectType::EnergyOrb)
        {
            if (intersectedObject == SpaceObjectType::Missile && _shield)
            {
                if (!_sparks)
                {
                    _sector->GetNode()->addChild(_sparksSector->GetNode());
                    _sparksSector->GetNode()->setVisible(true);
                    _sparksEffect->clear();
                    _sparks = true;
                    _sparksStart = _totalTime;
                }
            }
            else if (intersectedObject == SpaceObjectType::Barrier && _shield)
            {
                // TODO: play sound of dodging?

            }
            else if (intersectedObject == SpaceObjectType::Mine && _shield && Config::Instance()->GetSelectedModel() == 4)
            {
                if (!_sparks)
                {
                    _sector->GetNode()->addChild(_sparksSector->GetNode());
                    _sparksSector->GetNode()->setVisible(true);
                    _sparksEffect->clear();
                    _sparks = true;
                    _sparksStart = _totalTime;
                }
            }
            else
            {
                _speed = 0.0f;
                _speedAccel = 0.0f;
                StartExplosion();
                _sector->GetNode()->setVisible(false);

                LevelManager::Instance()->DecreaseLive();
            }
        }

        if (intersectedObject == SpaceObjectType::EnergyOrb)
        {
            if (Config::Instance()->IsSoundEnabled())
                _collectSound->Play();

            _score += 1 * _multiplier;
        }

        if (IsGameFinished())
        {
            _document->Hide();

            if (LevelManager::Instance()->IsVictory()
                || LevelManager::Instance()->GetLevelType() == LevelType::Challenge)
                return GameState::Score;
            else
                return GameState::SaveMe;
        }

        if (LevelManager::Instance()->GetLevelType() == LevelType::Rush
            && _space->GetCurrentLevel()->currentObstacle == 0)
        {
            InitMaxSpeed();
        }

        return Game::Instance()->GetState();
    }

    void RaceStateProcessor::StartExplosion()
    {
        _explosion = make_unique<Explosion>(VectorFromOgre(_sector->GetNode()->getPosition()));
        if (Config::Instance()->IsSoundEnabled())
            _bombSound->Play();
    }

    bool RaceStateProcessor::IsGameFinished()
    {
        if (LevelManager::Instance()->GetLives() < 0 && _invincibility)
        {
            LevelManager::Instance()->SetScore(_score);
            LevelManager::Instance()->SetTime((int)_totalTime);
            LevelManager::Instance()->SetVictory(false);

            return true;
        }

        if (LevelManager::Instance()->GetLevelType() != LevelType::Rush
                && _space->GetCurrentLevel()->currentObstacle == (int)_space->GetCurrentLevel()->obstacleList.size() - 1)
        {
            if (Config::Instance()->IsSoundEnabled())
                _successSound->Play();
            if (LevelManager::Instance()->GetLevelType() == LevelType::Challenge
                && !Scores::Instance()->IsChallengeCompleted(LevelManager::Instance()->GetLevelNum()))
                _score += 300;

            LevelManager::Instance()->SetScore(_score);
            LevelManager::Instance()->SetTime((int)_totalTime);
            LevelManager::Instance()->SetVictory(true);
            return true;
        }

        return false;
    }

    void RaceStateProcessor::UpdateHUD()
    {
        stringstream stream;
        stream << (int)_totalTime / 60 << ":" << setfill('0') << setw(2) << (int)_totalTime % 60;
        static auto timeControl = _document->GetControlByName("time");
        timeControl->SetText(stream.str());

        static auto scoreControl = _document->GetControlByName("score");
        scoreControl->SetText(to_string(_score));

        //static auto nameControl = _document->GetControlByName("name");
        //nameControl->SetText(to_string(_speed));

        //static auto missingControl = _document->GetControlByName("missing");
        //missingControl->SetText(to_string(_space->GetMissedOrbsCount()));

        static auto bombControl = _document->GetControlByName("bombcount");
        bombControl->SetText(to_string(Config::Instance()->GetBombCount()));

        static auto livesControl = _document->GetControlByName("shipcount");
        if (LevelManager::Instance()->GetLives() >= 0)
        {
            livesControl->SetText(to_string(LevelManager::Instance()->GetLives()));
        }

        //if (Config::Instance()->IsShowFPS())
        {
            float fps = OgreApplication::Instance()->GetWindow()->getAverageFPS();

            stream.str("");
            stream << (int)(fps);
            static auto fpsControl = _document->GetControlByName("FPS");
            fpsControl->SetText(stream.str());
        }

        if (LevelManager::Instance()->GetLevelNum() == 1 && LevelManager::Instance()->GetLevelType() == LevelType::Puzzle)
            UpdateHelp();
    }

    void RaceStateProcessor::UpdateHelp()
    {
        if (_explosion)
            return;

        static auto helpControl = _document->GetControlByName("help");

        helpControl->SetVisible(false);
        if (_totalTime > 2.5 * 1.375 && _totalTime < 4.5 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowleft.png", true);
        }
        if (_totalTime > 6 * 1.375 && _totalTime < 7.5 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowright.png", true);
        }
        if (_totalTime > 9 * 1.375 && _totalTime < 11 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowup.png", true);
        }
        if (_totalTime > 13.5 * 1.375 && _totalTime < 15 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowdown.png", true);
        }
        if (_totalTime > 16.5 * 1.375 && _totalTime < 18 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowright.png", true);
        }
        if (_totalTime > 16.5 * 1.375 && _totalTime < 18 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("arrowright.png", true);
        }

        if (_totalTime > 18.5 * 1.375 && _totalTime < 21 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("energyhelp.png", true);
        }

        if (_totalTime > 22.5 * 1.375 && _totalTime < 24 * 1.375)
        {
            helpControl->SetVisible(true);
            helpControl->SetDefaultMaterial("bombhelp.png", true);
        }
    }

    void RaceStateProcessor::InitBomb()
    {
        if (Config::Instance()->IsSoundEnabled())
        {
            _bombSound->Play();
            _superBombSound->Play();
        }

        if (_bigBomb)
        {
            _sector->GetNode()->removeChild(_bombSector->GetNode());
            _bigBomb.reset();
        }
        _space->DestroyEverything(true);
        auto sceneNodeBomb = RenderProcessor::Instance()->GetSceneManager()->createSceneNode();
        _sector->GetNode()->addChild(sceneNodeBomb);
        sceneNodeBomb->setInheritOrientation(false);
        sceneNodeBomb->setInheritScale(false);
        _bombSector = make_unique<SceneSector>(sceneNodeBomb);
        std::vector<Vector3> pointList;
        pointList.push_back(Vector3(-5.0f, 0, -5.0f));
        pointList.push_back(Vector3(5.0f, 0, -5.0f));
        pointList.push_back(Vector3(5.0f, 0,  5.0f));
        pointList.push_back(Vector3(-5.0f, 0, 5.0f));
        _bigBomb = make_unique<RectDrawable>(_bombSector.get(), "ExplosionRingMaterial", pointList);
        _bombScale = 0.1f;

        if (!Config::Instance()->IsAchievementCompleted(11))
        {
            Config::Instance()->SetAchievementCompleted(11);
            BillingProcessor::Instance()->SyncAchievements();
        }
    }

    void RaceStateProcessor::UpdateBomb(float time)
    {
        _bombScale += time;

        _bombSector->GetNode()->setScale(_bombScale*_bombScale, _bombScale*_bombScale, _bombScale*_bombScale);

        if (_bombScale > 5)
        {
            if (_bombScale < 5.2 && Config::Instance()->IsSoundEnabled())
                _bombSound->Play();
            _space->DestroyEverything();
        }
        if (_bombScale > 10)
        {
            _sector->GetNode()->removeChild(_bombSector->GetNode());
            _bigBomb.reset();
        }
    }

    void RaceStateProcessor::OnMouseDown(int x, int y)
    {
        if (_document->OnMouseDown(x, y))
            return;

        _startSlideX = x;
        _startSlideY = y;
    }

    void RaceStateProcessor::OnMouseUp(int x, int y)
    {
        if (_document->OnMouseUp(x, y))
            return;

        if (abs(x - _startSlideX) < _minSlideX && abs(y - _startSlideY) < _minSlideY)
        {
            return;
        }

        if (abs(x - _startSlideX) > abs(y - _startSlideY))
        {
            if (_startSlideX > x)
                OnKeyPressed(OIS::KC_LEFT);
            else if (_startSlideX < x)
                OnKeyPressed(OIS::KC_RIGHT);
        } else {
            if (_startSlideY > y)
                OnKeyPressed(OIS::KC_UP);
            else if (_startSlideY < y)
                OnKeyPressed(OIS::KC_DOWN);
        }
    }

    void RaceStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void RaceStateProcessor::OnMouseDoubleClick(int /*x*/, int /*y*/)
    {
        if (LevelManager::Instance()->GetLevelType() == LevelType::Puzzle && LevelManager::Instance()->GetLevelNum() == 1)
        {
            InitBomb();
        } else if (Config::Instance()->GetBombCount() > 0 && LevelManager::Instance()->GetLevelType() != LevelType::Challenge)
        {
            InitBomb();
            Config::Instance()->SetBombCount(Config::Instance()->GetBombCount() - 1);
        }
    }


    void RaceStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::Pause);
        }
        if (key == OIS::KC_UP)
        {
            if (_totalTime - _shootingStarted > SkinManager::Instance()->ShootingCooldown() && !_explosion)
            {
                for (auto i = 0; i < 4; i++)
                {
                    _space->AddShot(Vector3(12 - 3.0f*i, 0, _pos - 0.4f), 80);
                    _space->AddShot(Vector3(12 - 3.0f*i, 0, _pos + 0.4f), 80);
                }
                if (Config::Instance()->IsSoundEnabled())
                    _shootSound->Play();
                _shootingStarted = _totalTime;
            }
        }
        if (key == OIS::KC_DOWN)
        {
            if (_totalTime - _shieldStart > _shieldTime && !_explosion)
            {
                _shield = true;
                _shieldStart = _totalTime;

                _sector->GetNode()->addChild(_shieldSector->GetNode());
                _sector->GetNode()->addChild(_shieldEffectSector->GetNode());
                _shieldSector->GetNode()->setVisible(true);
                _shieldEffectSector->GetNode()->setVisible(true);
                _shieldEffect->clear();

                if (Config::Instance()->IsSoundEnabled())
                {
                    _shieldSound->Play();
                }
                //if (Config::Instance()->IsSoundEnabled())
                //	_shootSound->Play();
            } else if (!_explosion)
            {
                if (SkinManager::Instance()->ShieldQueue())
                    _shieldQueued = true;
            }
        }
        if (key == OIS::KC_LEFT)
        {
            if (_currentPosID == _targetPosID && _targetPosID < 2)
            {
                _targetPosID++;
                _nextTargetPosID = _targetPosID;
                _posChangingStarted = _totalTime;
            }
            else
            {
                if (_targetPosID < 2)
                    _nextTargetPosID++;
            }
        }
        if (key == OIS::KC_RIGHT)
        {
            if (_currentPosID == _targetPosID && _targetPosID > 0)
            {
                _targetPosID--;
                _nextTargetPosID = _targetPosID;
                _posChangingStarted = _totalTime;
            }
            else
            {
                if (_targetPosID > 0)
                    _nextTargetPosID--;
            }
        }
        if (key == OIS::KC_SPACE)
        {
            if (Config::Instance()->GetBombCount() > 0)
            {
                InitBomb();
                Config::Instance()->SetBombCount(Config::Instance()->GetBombCount() - 1);
            }
        }
    }


    void RaceStateProcessor::OnKeyReleased(OIS::KeyCode key)
    {
        if (key == OIS::KC_UP || key == OIS::KC_DOWN)
        {
            //_speedAccel = 0.0f;
        }
        if (key == OIS::KC_LEFT || key == OIS::KC_RIGHT)
        {

        }
    }

    void RaceStateProcessor::SetLightAndCamera()
    {
        auto _cameraRadius = 21.0f;

        Vector3 pos = Vector3(
            _cameraRadius,
            6.0f,
            _pos);
        _camera->SetPosition(pos);
        _camera->SetTarget(Vector3(0, 0, _pos));

        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += 2.0f;
        light->SetPosition(pos);
    }

    void RaceStateProcessor::Hide()
    {
        if (_init)
        {
            if (_space)
            {
                if (_shield)
                {
                    _sector->GetNode()->removeChild(_shieldSector->GetNode());
                    _sector->GetNode()->removeChild(_shieldEffectSector->GetNode());
                    _shield = false;
                    _shieldQueued = false;
                }
                _space->SetVisible(false);
            }
        }
        if (_sector)
            _sector->GetNode()->setVisible(false);

        //for_each(particleList.begin(), particleList.end(), bind(&ParticleSystem::Update, placeholders::_1, 10.0f));
        _document->Hide();
    }

    void RaceStateProcessor::Show()
    {
        bool wasStarted = LevelManager::Instance()->IsStarted();
        if (!wasStarted)
        {
            Init();
            LevelManager::Instance()->SetStarted(true);
            LevelManager::Instance()->SetVictory(false);
        }

        if (_space)
        {
            _space->SetVisible(true);
        }
        _sector->GetNode()->setVisible(true);

        _document->Show();

        if (Config::Instance()->IsShowFPS())
        {
            _document->GetControlByName("FPStext")->SetVisible(true);
            _document->GetControlByName("FPS")->SetVisible(true);
        }
        else {
            _document->GetControlByName("FPStext")->SetVisible(false);
            _document->GetControlByName("FPS")->SetVisible(false);
        }

        static auto nameControl = _document->GetControlByName("name");
        switch (LevelManager::Instance()->GetLevelType())
        {
            case LevelType::Puzzle:
                nameControl->SetText("Level " +to_string(LevelManager::Instance()->GetLevelNum()));
                break;
            case LevelType::Rush:
                nameControl->SetText("");
                break;
            case LevelType::Challenge:
                nameControl->SetText("Challenge " +to_string(LevelManager::Instance()->GetLevelNum()));
                break;
        }

        _multiplier = Config::Instance()->IsMultiplierBought() ? 2 : 1;

        _document->GetControlByName("help")->SetVisible(false);

        _document->GetControlByName("bosslifepanel")->SetVisible(false);
        _document->GetControlByName("bosslifebar")->SetVisible(false);
    }


    void RaceStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "pause")
        {
            //for_each(particleList.begin(), particleList.end(), bind(&ParticleSystem::Update, placeholders::_1, 10.0f));
            Game::Instance()->ChangeState(GameState::Pause);
        }
    }

    void RaceStateProcessor::UpdateBossLifePanel(int lives, int maxlives)
    {
        if (Config::Instance()->IsSoundEnabled() && lives < maxlives)
        {
            if (lives > 0)
                _impactSound->Play();
            else
                _bombSound->Play();
        }

        static auto bossLifePanel = _document->GetControlByName("bosslifepanel");
        static auto bossLifeBar = _document->GetControlByName("bosslifebar");

        if (lives <= 0)
        {
            bossLifePanel->SetVisible(false);
            bossLifeBar->SetVisible(false);
        } else {
            bossLifePanel->SetVisible(true);

            int baseWidth = bossLifePanel->GetWidth();

            int height = bossLifeBar->GetHeight();
            float k = (float)lives / (float)maxlives;

            if (k < 0.25f)
                bossLifeBar->SetDefaultMaterial("red.png", false);
            else
                bossLifeBar->SetDefaultMaterial("green.png", false);

            int width = (int)(k * baseWidth);
            bossLifeBar->SetSize(width, height);
            bossLifeBar->SetVisible(true);
        }
    }

}