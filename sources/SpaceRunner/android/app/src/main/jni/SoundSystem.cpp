#include "SoundSystem.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <fstream>
#include "OpenSL/OSLSound.h"
#include "OpenSL/OSLWav.h"
#include "OpenSL/OSLMp3.h"
#include "OpenSL/OSLContext.h"

AAssetManager *mgr;
SoundSystem* SoundSystem::_instance = nullptr;

Sound::Sound(OSLSound *sound)
{
	_sound = sound;
}

Sound::Sound(int id, struct android_app* state)
{
	_sound = nullptr;
	_state = state;
	_id = id;
}

void Sound::Play()
{
	if (_sound != nullptr)
		_sound->play();
	else
	{
		ANativeActivity* activity = _state->activity;
		JavaVM* jvm = _state->activity->vm;
		JNIEnv* env = NULL;
		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
		jint res = jvm->AttachCurrentThread(&env, NULL);

		if (res == JNI_ERR)
		{
			// Failed to retrieve JVM environment
			return;
		}

		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, "PlaySound", "(I)V");
		env->CallVoidMethod(activity->clazz, methodID, _id);
		jvm->DetachCurrentThread();
	}
}

Sound::~Sound()
{
	if (_sound != nullptr)
		delete _sound;
}

void Sound::Stop()
{
	_sound->pause();
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
	_bgmStarted = false;
}

bool SoundSystem::Init(AAssetManager* manager, struct android_app* state)
{
	_manager = manager;
	_state = state;
	mgr = _manager;

	_context =  new OSLContext(manager);

	_initialized = true;
	return true;
}


Sound* SoundSystem::CreateSound(char* filename, bool loop)
{
	if (loop)
	{
		auto type = OSLSound::BUFFER;
		if (loop)
			type = OSLSound::AASSET;
		auto sound = new OSLWav(_context, type);

		sound->load(filename);
		sound->setLoop(loop);

		return new Sound(sound);
	} else {
		// Get the android application's activity.
		ANativeActivity* activity = _state->activity;
		JavaVM* jvm = _state->activity->vm;
		JNIEnv* env = NULL;
		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
		jint res = jvm->AttachCurrentThread(&env, NULL);

		jstring jStr = env->NewStringUTF(filename);

		if (res == JNI_ERR)
		{
			return nullptr;
		}

		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, "LoadSound", "(Ljava/lang/String;)I");
		int result = env->CallIntMethod(activity->clazz, methodID, jStr);
		jvm->DetachCurrentThread();

		return new Sound(result, _state);
	}
}

Sound* SoundSystem::CreateMusic(char* filename, bool loop)
{
	OSLSound* sound = new OSLMp3(_context);

	sound->load(filename);
	sound->setLoop(loop);

	return new Sound(sound);
}

bool SoundSystem::IsLoaded()
{
	return _initialized;
}

SoundSystem::~SoundSystem()
{
	if (_bgm != nullptr)
		delete _bgm;

	if (_context != nullptr)
		delete _context;
}

void SoundSystem::InitBackgroundMusic(char* filename)
{
	_bgm = CreateMusic(filename, true);
}

void SoundSystem::StartBackgroundMusic()
{
	if (!_bgmStarted)
		_bgm->Play();

	_bgmStarted = true;
}

void SoundSystem::StopBackgroundMusic()
{
	if (_bgmStarted)
		_bgm->Stop();

	_bgmStarted = false;
}
