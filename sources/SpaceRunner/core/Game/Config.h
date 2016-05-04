#pragma once

namespace CoreEngine
{
    class Config
    {
        bool _soundEnabled;
        bool _musicEnabled;
        int _selectedModel;
        bool _signedGoogle;
        bool _showFPS;
        int _language;

        bool _achievements[20];
        int _achievementData[20];

        bool _modelBought[5];

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

        void SetMusicEnabled(bool enabled);
        bool IsMusicEnabled();

        void SetSignedGoogle(bool value);
        bool IsSignedGoogle();

        bool IsModelBought(int id);
        void SetModelBought(int id);

        void SetAchievementCompleted(int id);
        bool IsAchievementCompleted(int id);

        void SetAchievementData(int id, int data);
        int GetAchievementData(int id);

        void SetSelectedModel(int id);
        int GetSelectedModel();

        void SetShowFPS(bool value);
        bool IsShowFPS();

        void SetLanguage(int id);
        int GetLanguage();

        bool IsFirstRun();
    };
}