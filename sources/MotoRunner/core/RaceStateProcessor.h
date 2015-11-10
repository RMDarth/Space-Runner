#pragma once
#include "IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
#include "Road.h"
#include "Car.h"
#include <vector>
#include <memory>
using namespace std;

class Camera;
class ParticleSystem;
class Sound;

class ModelDrawable;
class SceneSector;

namespace CoreEngine
{
	class RaceStateProcessor : public IStateProcessor, public IEventHandler
	{
		Camera* _camera;

		ControlDocument * _document;

		shared_ptr<Road> _road;

		ModelDrawable * _bike;
		SceneSector * _sector;

		// sounds
		//shared_ptr<Sound> _motorSound;
		//bool _soundsLoaded;

		int _score;
		float _speed;
		float _totalTime;
		int _frames;
		float _speedAccel;
		float _turn;
		float _turnSpeed;

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
		void InitBike();
		void GenerateLevel();
		void PreloadModels();

		void SetLightAndCamera(float deltaTime);
		void UpdateHUD();
	};
}