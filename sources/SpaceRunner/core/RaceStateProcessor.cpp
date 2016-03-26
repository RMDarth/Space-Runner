#include "OgreApplication.h"
#include "RaceStateProcessor.h"
#include "RenderProcessor.h"
#include "SoundSystem.h"
#include "Game.h"
#include "Config.h"
#include "ParticleSystem.h"
#include "BillingProcessor.h"
#include "LevelManager.h"

#include "SpaceDust.h"
#include "Fence.h"
#include "Asteroid.h"
#include "BlasterBurst.h"

#include "ModelDrawable.h"
#include "SceneSector.h"

#include <algorithm>
#include <functional>
using namespace std;

#define CAMERA_SPEED 0.35f
#define MAP_SIZE 40
#define MIN_BALL_SIZE_ENDLESS 20
#define FLYBALL_SPEED 20.0f
#define REVEAL_COUNT 220
#define PI 3.1415f
#define EXPLOSIONS_NUM 4

namespace CoreEngine
{
	const float RaceStateProcessor::_explosionTime[4] = { 1.0f, 2.0f, 1.5f, 2.0f };

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

	RaceStateProcessor::RaceStateProcessor()
	{
		//_soundsLoaded = false;
		//InitSound();

		PreloadModels();
		
		_document = make_unique<ControlDocument>("GUI/HUD.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();

		_init = false;
	}

	void RaceStateProcessor::Init()
	{
		_space = make_unique<Space>();

		//RenderProcessor::Instance()->SetSkybox(rand() % SKYBOX_NUM + 1);
		if (!_sector)
			InitSpaceShip();

		_totalTime = 0;
		_score = 0;
		_frames = 0;
		_speed = 1.0f;
		_speedAccel = 5.0f;
		_speedMax = 11.0f;
		_pos = 0;
		_angleHorizontal = 0;
		_invisibility = false;
		_explosionEffect[0] = nullptr;

		_presetPos[0] = -BLOCK_SIZE * 1.1f;
		_presetPos[1] = 0;
		_presetPos[2] = BLOCK_SIZE * 1.1f;

		_currentPosID = _targetPosID = _nextTargetPosID = 1;

		_camera = RenderProcessor::Instance()->GetCamera();

		_init = true;
	}

	void RaceStateProcessor::InitSpaceShip()
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = make_unique<SceneSector>(sceneNode);

		//_sector->GetNode()->setDirection(Ogre::Vector3(1, 0, 0));
		_ship = make_unique<ModelDrawable>(_sector.get(), "ship.mesh");
		_ship->SetScale(5);

		auto sceneNodeChild = sceneManager->createSceneNode();
		sceneNodeChild->setPosition(0.35f, 0.0f, 0.0f);
		sceneNode->addChild(sceneNodeChild);

		_engineFire = sceneManager->createParticleSystem("EngineFire", "Engine");
		_engineFire->getEmitter(0)->setParticleVelocity(3.5f);
		sceneNodeChild->attachObject(_engineFire);
	}

	void RaceStateProcessor::InitSound()
	{
		auto soundSystem = SoundSystem::Instance();
		/*if (soundSystem->IsLoaded() && _soundsLoaded == false)
		{
			_launchSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/LaunchSound.wav"));
			_hitSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/HitSound.wav"));
			_hitDestroySound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/HitDestroySound.wav"));
			_bombSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/BombSound.wav"));
			_successSound = shared_ptr<Sound>(soundSystem->CreateSound("Sound/SuccessSound.wav"));
			_soundsLoaded = true;
		}*/
	}

	void RaceStateProcessor::PreloadModels()
	{
		Vector3 zero;
		Vector3i zeroi;

		for (auto i = 1; i <= 6; i++)
		{
			stringstream ss;
			ss << "Asteroid" << i << "_LOD0.mesh";
			Asteroid * asteroid = new Asteroid(zero, ss.str(), 0, 0);
			delete asteroid;
		}
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		for (int i = 1; i <= EXPLOSIONS_NUM; i++)
		{
			auto sceneNodeChild = sceneManager->createSceneNode();
			sceneManager->getRootSceneNode()->addChild(sceneNodeChild);
			stringstream ss;
			ss << "Blast" << i;
			auto particleSystem = sceneManager->createParticleSystem(ss.str(), ss.str());
			sceneNodeChild->attachObject(particleSystem);
			sceneManager->getRootSceneNode()->removeChild(sceneNodeChild);
		}
		//auto gun = new Gun(zero);
		//delete gun;
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

	void RaceStateProcessor::UpdateShots(float time)
	{
		for_each(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::Update, placeholders::_1, time, time * _speed * 5.0f));
		_shotList.erase(remove_if(_shotList.begin(), _shotList.end(), bind(&BlasterBurst::IsDone, placeholders::_1)), _shotList.end());
		if (_shotList.empty())
		{
			for (auto i = 0; i < 4; i++)
			{
				auto shot = make_shared<BlasterBurst>(Vector3(10 - 3.0f*i, 0, _pos), "BlasterShotMaterial", 0, 80.0f);
				_shotList.push_back(shot);
			}
		}
	}

	GameState::State RaceStateProcessor::Update(float time)
	{
		_totalTime += time;
		_frames++;

		SetLightAndCamera(time);
		UpdateHUD();

		_speed += _speedAccel * time;
		if (_speed > _speedMax)
			_speed = _speedMax;

		if (_invisibility)
		{
			if (_totalTime > _invisibilityStart + _invisibilityTime)
			{
				_invisibility = false;
				_ship->SetVisible(true);
			}
			else 
			{
				auto invisTime = _totalTime - _invisibilityStart;
				auto koef = static_cast<int>(invisTime * 10);
				_ship->SetVisible(koef % 2 ? true : false);
			}
		}

		if (_explosionEffect[0])
		{
			for (int i = 0; i < EXPLOSIONS_NUM; i++)
			{
				_explosionEffect[i]->Update(time);
			}
			if (_explosionEffect[1]->IsFinished())
			{
				for (int i = 0; i < EXPLOSIONS_NUM; i++)
				{
					delete _explosionEffect[i];
					_explosionEffect[i] = nullptr;
				}

				_invisibility = true;
				_invisibilityStart = _totalTime;
				_speedAccel = 5.0f;
				_sector->GetNode()->setVisible(true);
			}
		}
		else 
		{
			UpdateTurn();
			UpdateShots(time);
		}

		// Engine fire strength
		auto multiplier = _speed * 0.8f;
		if (multiplier > 1.1f) multiplier = 1.1f;
		if (multiplier < 0.8f) multiplier = 0.8f;
		_engineFire->getEmitter(0)->setParticleVelocity(3.5f * multiplier);

		_space->Update(time, _speed);
		_sector->GetNode()->setPosition(10, 0, _pos);

		auto angle = Ogre::Quaternion(Ogre::Radian(_angleHorizontal), Ogre::Vector3(1, 0, 0));
		_sector->GetNode()->setOrientation(angle);

		if (!_invisibility && !_explosionEffect[0] && _space->IsIntersected(_pos))
		{
			_speed = 0.0f;
			_speedAccel = 0.0f;
			StartExplosion();
			_sector->GetNode()->setVisible(false);

			_score++;
		}
		/*if (IsGameLost())
		{
			for_each(particleList.begin(), particleList.end(), bind(&ParticleSystem::Update, placeholders::_1, 10.0f));
			_document->Hide();
			return GameState::Score;
		}*/

		return Game::Instance()->GetState();
	}

	void RaceStateProcessor::StartExplosion()
	{
		if (_explosionEffect[0])
		{
			for (int i = 0; i < EXPLOSIONS_NUM; i++)
				delete _explosionEffect[i];
		}

		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		for (auto i = 0; i < EXPLOSIONS_NUM; i++)
		{
			auto sceneNodeChild = sceneManager->createSceneNode();
			sceneNodeChild->setPosition(_sector->GetNode()->getPosition());
			sceneManager->getRootSceneNode()->addChild(sceneNodeChild);
			stringstream str;
			str << "Blast" << (i+1) << "_%d";
			string templateName = str.str();
			str.str("");
			str << "Blast" << (i+1);
			_explosionEffect[i] = new ParticleSystem(sceneNodeChild, templateName, str.str(), _explosionTime[i], EXPLOSIONS_NUM - i);

		}
	}

	
	void RaceStateProcessor::UpdateHUD()
	{
		stringstream stream;
		stream << (int)_totalTime / 60 << ":" << setfill('0') << setw(2) << (int)_totalTime % 60;
		static auto timeControl = _document->GetControlByName("time");
		timeControl->SetText(stream.str());

		stream.str("");
		stream << _score;
		static auto scoreControl = _document->GetControlByName("score");
		scoreControl->SetText(stream.str());

		stream.str("");
		stream << _speed;
		static auto nameControl = _document->GetControlByName("name");
		nameControl->SetText(stream.str());

		//if (Config::Instance()->IsShowFPS())
		{
			float fps = OgreApplication::Instance()->GetWindow()->getAverageFPS();

			stream.str("");
			stream << (int)(fps);
			static auto fpsControl = _document->GetControlByName("FPS");
			fpsControl->SetText(stream.str());
		}
	}

	void RaceStateProcessor::OnMouseDown(int x, int y)
	{
		if (_document->OnMouseDown(x, y))
			return;
	}

	void RaceStateProcessor::OnMouseUp(int x, int y)
	{
		_document->OnMouseUp(x, y);
	}

	void RaceStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);
	}

	void RaceStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::Pause);
		}
		if (key == OIS::KC_UP)
		{
			//_speedAccel = 5.0f;
		}
		if (key == OIS::KC_DOWN)
		{
			//_speedAccel = -5.0f;
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

	void RaceStateProcessor::GenerateLevel()
	{
		
	}

	void RaceStateProcessor::SetLightAndCamera(float deltaTime)
	{
		//_cameraTime += deltaTime * _direction;
		auto _cameraRadius = 21.0f;
		auto _cameraTime = 0.0f;
		auto camera = RenderProcessor::Instance()->GetCamera();
		Vector3 pos = Vector3(
			_cameraRadius, //_cameraRadius * cos(_cameraTime * CAMERA_SPEED),
			5.0f,
			_pos);// *sin(_cameraTime * CAMERA_SPEED));
		camera->SetPosition(pos);
		camera->SetTarget(Vector3(0, 0, _pos));

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
	}


	void RaceStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "pause")
		{
			//for_each(particleList.begin(), particleList.end(), bind(&ParticleSystem::Update, placeholders::_1, 10.0f));
			Game::Instance()->ChangeState(GameState::Pause);
		}
	}
}