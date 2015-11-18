#pragma once

class SoundSystem;

class Sound
{
	friend SoundSystem;

	int _id;
	int _source;

	Sound(int id, int source);
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
	bool Init();
	static SoundSystem* Instance();
	bool IsLoaded();

	Sound* CreateSound(char* filename, bool loop = false);

	void InitBackgroundMusic(char* filename);
	void StartBackgroundMusic();
	void StopBackgroundMusic();
};