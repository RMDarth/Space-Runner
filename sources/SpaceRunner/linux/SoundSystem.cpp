#include "SoundSystem.h"
#include "OpenAL/OpenAL.h"
#include <string>

SoundSystem* SoundSystem::_instance = nullptr;

Sound::Sound(int id)
{
	_id = id;
}

void Sound::Play()
{
    OpenALProcessor::Instance()->PlaySound(_id);
	//alSourcei(_source, AL_BUFFER, _id);
	//alSourcePlay(_source);
}

void Sound::Stop()
{
    OpenALProcessor::Instance()->StopSound(_id);
	//alSourceStop(_source);
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
    _initialized = OpenALProcessor::Instance()->IsLoaded();
    return _initialized;
}


Sound* SoundSystem::CreateSound(char* filename, bool loop)
{
    auto sound = new Sound(OpenALProcessor::Instance()->RegisterSound(std::string(filename), loop));
    return sound;
}

bool SoundSystem::IsLoaded()
{
	return _initialized;
}

void SoundSystem::Dispose()
{
    if (_initialized)
        OpenALProcessor::Instance()->Dispose();
    _initialized = false;
}

SoundSystem::~SoundSystem()
{
	if (_bgm != nullptr)
		delete _bgm;

	/*ALFWShutdownOpenAL();
	ALFWShutdown();*/
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
