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
	class RaceStateProcessor : public IStateProcessor, public IEventHandler
	{
		Camera* _camera;
		
		UPtr<ControlDocument> _document;
		UPtr<Space> _space;
		UPtr<SceneSector> _sector;
		UPtr<ModelDrawable> _ship;
		//UPtr<ParticleSystem> _engineFire;
		Ogre::ParticleSystem * _engineFire;

		// sounds
		//shared_ptr<Sound> _motorSound;
		//bool _soundsLoaded;

		int _score;

		float _totalTime;
		int _frames;
		
		float _speed;
		float _speedAccel;

		float _pos;
		float _speedHorizontal;
		float _speedAccelHorizontal;
		float _speedLimitHorizontal;

		float _angleHorizontal;
		float _angleAccelHorizontal;
		float _angleLimitHorizontal;

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
		void UpdateTurn(float time, float roadspeed);

		void SetLightAndCamera(float deltaTime);
		void UpdateHUD();
	};
}