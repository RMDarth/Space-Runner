#pragma once
#include "IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
#include "Space.h"
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
		UPtr<ModelDrawable> _ship;

		Ogre::ParticleSystem * _engineFire;

		UPtr<Explosion> _explosion;

		// sounds
		UPtr<Sound> _bombSound;
		UPtr<Sound> _shootSound;
		UPtr<Sound> _impactSound;
		bool _soundsLoaded;

		int _score;

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

		float _shootingStarted;
		const float _shootingTime = 1.5f;

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

		void SetLightAndCamera(float deltaTime);
		void UpdateHUD();
	};
}