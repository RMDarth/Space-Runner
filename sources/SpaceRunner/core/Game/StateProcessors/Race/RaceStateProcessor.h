#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
#include "Game/StateProcessors/Race/Space/Space.h"
#include <vector>
#include <memory>

class Camera;
class ParticleSystem;
class Sound;

class ModelDrawable;
class SceneSector;

namespace Ogre
{
	class ParticleSystem;
}

namespace CoreEngine
{
	class Explosion;

	class RaceStateProcessor : public IStateProcessor, public IEventHandler
	{
		Camera* _camera;
		
		UPtr<ControlDocument> _document;
		UPtr<Space> _space;
		UPtr<SceneSector> _sector;
		UPtr<SceneSector> _shipSector;
		UPtr<SceneSector> _shieldSector;
		UPtr<SceneSector> _shieldEffectSector;

		UPtr<ModelDrawable> _ship;
		UPtr<ModelDrawable> _shipShield;

		Ogre::ParticleSystem * _engineFire[2];
		Ogre::ParticleSystem * _shieldEffect;


		UPtr<Explosion> _explosion;

		// sounds
		UPtr<Sound> _bombSound;
		UPtr<Sound> _shootSound;
		UPtr<Sound> _impactSound;
		UPtr<Sound> _collectSound;
		UPtr<Sound> _successSound;
		bool _soundsLoaded;

		int _score;
		int _lives;

		float _totalTime;
		int _frames;
		
		float _speed;
		float _speedAccel;
		float _speedMax;

		float _angleHorizontal;
		const float _angleHorizontalMax = 0.7f;

		float _pos;
		
		float _presetPos[3];
		int _currentPosID;
		int _targetPosID;
		int _nextTargetPosID;
		float _posChangingStarted;
		const float _posChangingTime = 0.4f;

		bool _invincibility;
		float _invincibilityStart;
		const float _invincibilityTime = 3.0f;

		bool _shield;
		float _shieldStart;
		const float _shieldTime = 1.5f;

		float _shootingStarted;
		const float _shootingTime = 1.5f;

		float _startSlideX;
        float _startSlideY;

		bool _init;

	public:
		RaceStateProcessor();

		void Init();
		virtual GameState::State Update(float time);

		virtual void OnMouseDown(int x, int y);
		virtual void OnMouseUp(int x, int y);
		virtual void OnMouseMove(int x, int y);
		virtual void OnKeyPressed(OIS::KeyCode key);
		virtual void OnKeyReleased(OIS::KeyCode key);

		virtual void Hide();
		virtual void Show();
		virtual bool IsOverlapping() { return false; }

		// IEventProcessor interface //
		virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

	private:
		void InitSound();
		void InitSpaceShip();
		void GenerateLevel();
		void PreloadModels();
		void UpdateTurn();
		void StartExplosion();
		
		bool IsGameFinished();

		void SetLightAndCamera(float deltaTime);
		void UpdateHUD();
	};
}