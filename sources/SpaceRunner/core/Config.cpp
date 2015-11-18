#include "Config.h"
#include "FileSystem.h"
#include "Game.h"

namespace CoreEngine
{
	Config* Config::_instance = nullptr;

	Config* Config::Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new Config();
		}
		return _instance;
	}

	void Config::Load()
	{
		SecureFileInputRef *configFile = new SecureFileInputRef("config.dat");
		if (!configFile->IsOpened())
		{
			delete configFile;
			CreateDefault();
			_firstRun = true;

			configFile = new SecureFileInputRef("config.dat");
		}

		_soundEnabled = (configFile->ReadInt() == 1);
		_musicEnabled = (configFile->ReadInt() == 1);
		_signedGoogle = (configFile->ReadInt() == 1);
		_selectedModel = configFile->ReadInt();
		_selectedQuality = configFile->ReadInt();

		if (_selectedModel < 0 || _selectedModel > 10)
			_selectedModel = 0;

		for (int i = 0; i < 20; i++)
		{
			_achievements[i] = (configFile->ReadInt() == 1);
			_achievementData[i] = configFile->ReadInt();
		}

		configFile->Close();

		delete configFile;
	}

	void Config::Save()
	{
		SecureFileOutputRef arcadeFile("config.dat");
		arcadeFile.WriteInt(_soundEnabled ? 1 : 0);
		arcadeFile.WriteInt(_musicEnabled ? 1 : 0);
		arcadeFile.WriteInt(_signedGoogle ? 1 : 0);
		arcadeFile.WriteInt(_selectedModel);
		arcadeFile.WriteInt(_selectedQuality);

		for (int i = 0; i < 20; i++)
		{
			arcadeFile.WriteInt(_achievements[i] ? 1 : 0);
			arcadeFile.WriteInt(_achievementData[i]);
		}

		arcadeFile.Close();
	}

	Config::Config()
	{
		_soundEnabled = true;
		_musicEnabled = true;
		_signedGoogle = false;
		_hiddenEnabled = true;
		_showFPS = false;
		_selectedModel = 0;
		_selectedQuality = 0;
		_language = 0;
		_firstRun = false;

		for (int i = 0; i < 20; i++)
			_achievements[i] = false;
		for (int i = 0; i < 20; i++)
			_achievementData[i] = 0;
	}

	void Config::CreateDefault()
	{
		SecureFileOutputRef arcadeFile("config.dat");
		arcadeFile.WriteInt(1); // sound
		arcadeFile.WriteInt(1); // music
		arcadeFile.WriteInt(0); // signed google
		arcadeFile.WriteInt(0); // model
		arcadeFile.WriteInt(0); // quality

		for (int i = 0; i < 20; i++)
		{
			arcadeFile.WriteInt(0); // achivement unlocked
			arcadeFile.WriteInt(0); // achivement data
		}


		arcadeFile.Close();
	}

	void Config::SetSoundEnabled(bool enabled)
	{
		_soundEnabled = enabled;
		Save();
	}

	bool Config::IsSoundEnabled()
	{
		return _soundEnabled;
	}

	void Config::SetMusicEnabled(bool enabled)
	{
		_musicEnabled = enabled;
		Save();
	}

	bool Config::IsMusicEnabled()
	{
		return _musicEnabled;
	}

	void Config::SetHiddenBallsEnabled(bool enabled)
	{
		_hiddenEnabled = enabled;
	}

	bool Config::IsHiddenBallsEnabled()
	{
		return _hiddenEnabled;
	}

	void Config::SetSignedGoogle(bool value)
	{
		_signedGoogle = value;
		Save();
	}

	bool Config::IsSignedGoogle()
	{
		return _signedGoogle;
	}

	void Config::SetSelectedModel(int id)
	{
		_selectedModel = id;
		Save();
	}

	int Config::GetSelectedModel()
	{
		return _selectedModel;
	}

	void Config::SetSelectedQuality(int quality)
	{
		_selectedQuality = quality;
		Save();
	}

	int Config::GetSelectedQuality()
	{
		return _selectedQuality;
	}

	void Config::SetAchievementCompleted(int id)
	{
		_achievements[id] = true;
		Save();
	}

	bool Config::IsAchievementCompleted(int id)
	{
		return _achievements[id];
	}

	void Config::SetAchievementData(int id, int data)
	{
		_achievementData[id] = data;
		Save();
	}

	int Config::GetAchievementData(int id)
	{
		return _achievementData[id];
	}


	void Config::SetShowFPS(bool value)
	{
		_showFPS = value;
	}

	bool Config::IsShowFPS()
	{
		return _showFPS;
	}

	void Config::SetLanguage(int id)
	{
		_language = id;
	}

	int Config::GetLanguage()
	{
		return _language;
	}

	bool Config::IsFirstRun()
	{
		return _firstRun;
	}
}