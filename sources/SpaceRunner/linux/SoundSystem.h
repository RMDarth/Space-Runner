#pragma once

class SoundSystem;

class Sound
{
	friend class SoundSystem;

	unsigned int _id;

	Sound(int id);
public:
	void Play();
	void Stop();
};

class SoundSystem
{
	bool _initialized;
	static SoundSystem* _instance;

	Sound* _bgm;

	SoundSystem();
public:
	~SoundSystem();
	void Dispose();
	bool Init();
	static SoundSystem* Instance();
	bool IsLoaded();

	Sound* CreateSound(const char* filename, bool loop = false);

	void InitBackgroundMusic(const char* filename);
	void StartBackgroundMusic();
	void StopBackgroundMusic();
};
