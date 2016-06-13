#include <BillingProcessor.h>
#include "Config.h"
#include "FileSystem.h"

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
            //CreateDefault();
            _firstRun = true;
            return;

            configFile = new SecureFileInputRef("config.dat");
        }

        _soundEnabled = (configFile->ReadInt() == 1);
        _musicEnabled = (configFile->ReadInt() == 1);
        _signedGoogle = (configFile->ReadInt() == 1);
        _scoreSubmitted = configFile->ReadInt();
        _selectedModel = configFile->ReadInt();

        if (_selectedModel < 0 || _selectedModel > 10)
            _selectedModel = 0;

        for (int i = 0; i < 5; i++)
        {
            _modelBought[i] = (configFile->ReadInt() == 1);
        }

        _multiplierBought = (configFile->ReadInt() == 1);
        _bombCount = configFile->ReadInt();
        _resurrectCount = configFile->ReadInt();

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
        arcadeFile.WriteInt(_scoreSubmitted);
        arcadeFile.WriteInt(_selectedModel);

        for (int i = 0; i < 5; i++)
        {
            arcadeFile.WriteInt(_modelBought[i] ? 1 : 0);
        }

        arcadeFile.WriteInt(_multiplierBought ? 1 : 0);
        arcadeFile.WriteInt(_bombCount);
        arcadeFile.WriteInt(_resurrectCount);

        for (int i = 0; i < 20; i++)
        {
            arcadeFile.WriteInt(_achievements[i] ? 1 : 0);
            arcadeFile.WriteInt(_achievementData[i]);
        }

        arcadeFile.Close();

        BillingProcessor::Instance()->RequestBackup();
    }

    Config::Config()
    {
        _soundEnabled = true;
        _musicEnabled = true;
        _signedGoogle = false;
        _showFPS = false;
        _selectedModel = 0;
        _language = 0;
        _firstRun = false;
        _scoreSubmitted = 0;

        _multiplierBought = false;
        _bombCount = 0;
        _resurrectCount = 0;

        for (int i = 1; i < 5; i++)
            _modelBought[i] = false;
        _modelBought[0] = true;

        for (int i = 0; i < 20; i++)
            _achievements[i] = false;
        for (int i = 0; i < 20; i++)
            _achievementData[i] = 0;
    }

    void Config::CreateDefault()
    {
        SecureFileOutputRef configFile("config.dat");
        configFile.WriteInt(1); // sound
        configFile.WriteInt(1); // music
        configFile.WriteInt(0); // signed google
        configFile.WriteInt(0); // score submitted
        configFile.WriteInt(0); // model

        configFile.WriteInt(1); // first model bought
        for (int i = 1; i < 5; i++)
        {
            configFile.WriteInt(0); // models are not bought
        }

        configFile.WriteInt(0); // multiplier not bought;
        configFile.WriteInt(0); // bomb count;
        configFile.WriteInt(0); // resurrect count;

        for (int i = 0; i < 20; i++)
        {
            configFile.WriteInt(0); // achievement unlocked
            configFile.WriteInt(0); // achievement data
        }

        configFile.Close();
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

    bool Config::IsModelBought(int id)
    {
        return _modelBought[id];
    }

    void Config::SetModelBought(int id)
    {
        _modelBought[id] = true;
        Save();
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

    int Config::GetBombCount()
    {
        return _bombCount;
    }

    void Config::SetBombCount(int count)
    {
        _bombCount = count;
        Save();
    }

    int Config::GetResurrectCount()
    {
        return _resurrectCount;
    }

    void Config::SetResurrectCount(int count)
    {
        _resurrectCount = count;
        Save();
    }

    bool Config::IsMultiplierBought()
    {
        return _multiplierBought;
    }

    void Config::SetMultiplierBought()
    {
        _multiplierBought = true;
        Save();
    }

    int Config::GetScoreSubmitted()
    {
        return _scoreSubmitted;
    }

    void Config::SetScoreSubmitted(int value)
    {
        if (_scoreSubmitted < value || value == 0)
        {
            _scoreSubmitted = value;
        }
        Save();
    }
}