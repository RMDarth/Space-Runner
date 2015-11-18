#pragma once
#include "IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"
#include <memory>
using namespace std;

class Sound;

namespace CoreEngine
{
	class ScoreStateProcessor : public IStateProcessor,	public IEventHandler
	{
		int _score;
		float _currentScore;
		int _time;
		float _totalTime;
		float _span;
		float _achievementTime;
		int _stars;

		int _restartX;
		int _restartY;

		ControlDocument * _document;
		ControlDocument * _achievementWindow;

		vector<std::string> _achievementName;

		// sounds
		shared_ptr<Sound> _starSound;
		bool _soundsLoaded;
		int _soundsPlayed;

		void InitSound();
		void HideContinue();

		void UpdateAchievements();
		void ShowAchievement(int id);
		void InitAchievementNames();

	public:
		ScoreStateProcessor();
		~ScoreStateProcessor();

		void SetScore(int score, int time, float span, int stars);

		virtual GameState::State Update(float time);
		
		virtual void OnMouseDown(int x, int y);
		virtual void OnMouseUp(int x, int y);
		virtual void OnMouseMove(int x, int y);
		virtual void OnKeyPressed(OIS::KeyCode key);

		virtual void Hide();
		virtual void Show();
		virtual bool IsOverlapping() { return true; }

		// IEventProcessor interface //
		virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

		
	};
}

