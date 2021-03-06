#pragma once
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
class ModelDrawable;
class SceneSector;

namespace CoreEngine
{
	class MenuStateProcessor : public IStateProcessor,  public IEventHandler
	{
		ModelDrawable* _model;

		SceneSector* _sector;
		SceneSector* _sectorShip;

		ControlDocument * _document;

		float _totalTime;
		float _signTime;
		bool _logged;
		bool _updateSlider;
		bool _firstRunDocument;
		bool _showInterstitial;

	public:
		MenuStateProcessor();
		~MenuStateProcessor();

		// IStateProcessor interface //
		virtual GameState::State Update(float time);
		virtual void OnMouseDown(int x, int y);
		virtual void OnMouseUp(int x, int y);
		virtual void OnMouseMove(int x, int y);

		virtual void Hide();
		virtual void Show();
		virtual bool IsOverlapping() { return false; }

		// IEventProcessor interface //
		virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);
	private:
		void GenerateLevel();
		void SetLightAndCamera();
		void ChangeSettingsPanelVisibility();
		void UpdateSoundIcon(Control* control);
		void UpdateMusicIcon(Control* control);
		void UpdateGooglePlayIcon(Control* control);
		void UpdateSlider();
	};
}

