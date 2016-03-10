﻿#include "OgreApplication.h"
#include "RaceStateProcessor.h"
#include "RenderProcessor.h"
#include "SoundSystem.h"
#include "Game.h"
#include "Config.h"
#include "ParticleSystem.h"
#include "BillingProcessor.h"
#include "LevelManager.h"
#include "SpaceDust.h"

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

namespace CoreEngine
{
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
		_speedAccel = 0;
		_turn = 0;
		_turnSpeed = 0;

		_camera = RenderProcessor::Instance()->GetCamera();

		_init = true;
	}

	void RaceStateProcessor::InitSpaceShip()
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = make_unique<SceneSector>(sceneNode);
		//sceneNode->setPosition(-1.0f, 0.1f, 0.0f);

		//_sector->GetNode()->setDirection(Ogre::Vector3(1, 0, 0));
		_ship = make_unique<ModelDrawable>(_sector.get(), "ship.mesh");
		_ship->SetScale(5);

		auto sceneNodeChild = sceneManager->createSceneNode();
		sceneNodeChild->setPosition(0.35, 0.0, 0);
		sceneNode->addChild(sceneNodeChild);

		_engineFire = sceneManager->createParticleSystem("EngineFire", "Engine");
		_engineFire->getEmitter(0)->setParticleVelocity(3.5f);
		sceneNodeChild->attachObject(_engineFire);
		//_engineFire = make_unique<ParticleSystem>();
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
		//auto gun = new Gun(zero);
		//delete gun;
	}


	GameState::State RaceStateProcessor::Update(float time)
	{
		_totalTime += time;
		_frames++;

		SetLightAndCamera(time);
		UpdateHUD();

		_speed += _speedAccel * time;
		_turn += _turnSpeed * time;


		if (_turn > BLOCK_SIZE * 1.5f)
			_turn = BLOCK_SIZE * 1.5f;
		if (_turn < -BLOCK_SIZE * 1.5f)
			_turn = -BLOCK_SIZE * 1.5f;

		auto multiplier = _speed * 0.8f;
		if (multiplier > 1.1f) multiplier = 1.1f;
		if (multiplier < 0.8f) multiplier = 0.8f;
		_engineFire->getEmitter(0)->setParticleVelocity(3.5f * multiplier);
		_space->Update(time, _speed);
		_sector->GetNode()->setPosition(10, 0, _turn);

		if (_space->IsIntersected(_turn))
			_score++;

		/*if (IsGameLost())
		{
			for_each(particleList.begin(), particleList.end(), bind(&ParticleSystem::Update, placeholders::_1, 10.0f));
			_document->Hide();
			return GameState::Score;
		}*/

		return Game::Instance()->GetState();
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
			_speedAccel = 5.0f;
		}
		if (key == OIS::KC_DOWN)
		{
			_speedAccel = -5.0f;
		}
		if (key == OIS::KC_LEFT)
		{
			_turnSpeed = 10.0f;
		}
		if (key == OIS::KC_RIGHT)
		{
			_turnSpeed = -10.0f;
		}
	}


	void RaceStateProcessor::OnKeyReleased(OIS::KeyCode key)
	{
		if (key == OIS::KC_UP || key == OIS::KC_DOWN)
		{
			_speedAccel = 0.0f;
		}
		if (key == OIS::KC_LEFT || key == OIS::KC_RIGHT)
		{
			_turnSpeed = 0.0f;
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
			_turn);// *sin(_cameraTime * CAMERA_SPEED));
		camera->SetPosition(pos);
		camera->SetTarget(Vector3(0, 0, _turn));

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