#include "SoundSystem.h"
#include "windows.h"
#include "OpenAL/Framework.h"

SoundSystem* SoundSystem::_instance = nullptr;

Sound::Sound(int id, int source)
{
	_id = id;
	_source = source;
}

void Sound::Play()
{
	alSourcei(_source, AL_BUFFER, _id);
	alSourcePlay(_source);
}

void Sound::Stop()
{
	alSourceStop(_source);
}

SoundSystem* SoundSystem::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new SoundSystem();
	}
	return _instance;
}


SoundSystem::SoundSystem()
{
	_bgm = nullptr;
	_initialized = false;
}

bool SoundSystem::Init()
{
	ALFWInit();

	if (!ALFWInitOpenAL())
	{
		ALFWShutdown();
		return false;
	}
	else {
		_initialized = true;
		return true;
	}
}


Sound* SoundSystem::CreateSound(char* filename, bool loop)
{
	ALuint      uiBuffer;
	alGenBuffers(1, &uiBuffer);
	if (!ALFWLoadWaveToBuffer(filename, uiBuffer))
		return nullptr;

	ALuint      uiSource;
	alGenSources(1, &uiSource);
	alSourcei(uiSource, AL_BUFFER, uiBuffer);
	if (loop)
		alSourcei(uiSource, AL_LOOPING, AL_TRUE);

	auto sound = new Sound(uiBuffer, uiSource);

	return sound;
}

bool SoundSystem::IsLoaded()
{
	return _initialized;
}

SoundSystem::~SoundSystem()
{
	if (_bgm != nullptr)
		delete _bgm;

	ALFWShutdownOpenAL();
	ALFWShutdown();
}

void SoundSystem::InitBackgroundMusic(char* filename)
{
	_bgm = CreateSound(filename, true);
}

void SoundSystem::StartBackgroundMusic()
{
	if (_bgm != nullptr)
		_bgm->Play();
}

void SoundSystem::StopBackgroundMusic()
{
	if (_bgm != nullptr)
		_bgm->Stop();
}