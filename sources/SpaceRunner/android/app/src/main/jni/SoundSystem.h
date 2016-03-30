#pragma once
#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

class OSLSound;
class OSLContext;
class SoundSystem;

class Sound
{
	friend class SoundSystem;

	OSLSound * _sound;
	int _id;
	struct android_app* _state;

	Sound(OSLSound *sound);
	Sound(int id, struct android_app* state);
public:
	~Sound();
	void Play();
	void Stop();
};

class SoundSystem
{
	bool _initialized;
	static SoundSystem* _instance;

	Sound* _bgm;
	bool _bgmStarted;

	AAssetManager* _manager;
	struct android_app* _state;
	OSLContext* _context;

	SoundSystem();
public:
	~SoundSystem();
	bool Init(AAssetManager* manager, struct android_app* state);
	static SoundSystem* Instance();
	bool IsLoaded();

	Sound* CreateSound(char* filename, bool loop = false);
	Sound* CreateMusic(char* filename, bool loop = false);

	void InitBackgroundMusic(char* filename);
	void StartBackgroundMusic();
	void StopBackgroundMusic();
};
