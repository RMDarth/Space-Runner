#pragma once

namespace CoreEngine
{
	class Config
	{
		bool _soundEnabled;
		bool _musicEnabled;
		int _selectedModel;
		int _selectedQuality;
		bool _signedGoogle;
		bool _hiddenEnabled;
		bool _showFPS;
		int _language;

		bool _achievements[20];
		int _achievementData[20];
		bool _firstRun;

		static Config* _instance;

		void CreateDefault();

		Config();
	public:
		static Config* Instance();

		void Load();
		void Save();

		void SetSoundEnabled(bool enabled);
		bool IsSoundEnabled();

		void SetHiddenBallsEnabled(bool enabled);
		bool IsHiddenBallsEnabled();

		void SetMusicEnabled(bool enabled);
		bool IsMusicEnabled();

		void SetSignedGoogle(bool value);
		bool IsSignedGoogle();

		void SetAchievementCompleted(int id);
		bool IsAchievementCompleted(int id);

		void SetAchievementData(int id, int data);
		int GetAchievementData(int id);

		void SetSelectedModel(int id);
		int GetSelectedModel();

		void SetSelectedQuality(int id);
		int GetSelectedQuality();

		void SetShowFPS(bool value);
		bool IsShowFPS();

		void SetLanguage(int id);
		int GetLanguage();

		bool IsFirstRun();
	};
}