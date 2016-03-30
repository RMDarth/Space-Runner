#include "OpenAL.h"
#include <audio/wave.h>



OpenALProcessor* OpenALProcessor::_instance = nullptr;

OpenALProcessor* OpenALProcessor::Instance()
{
    if (_instance == nullptr)
    {
        _instance = new OpenALProcessor();
    }
    return _instance;
}

OpenALProcessor::OpenALProcessor()
{
	_device = alcOpenDevice(NULL);
	if (!_device) {
        return;
	}

    _context = alcCreateContext(_device, NULL);
    if (!alcMakeContextCurrent(_context))
    {
        alcCloseDevice(_device);
        _device = nullptr;
        return;
    }
}

void OpenALProcessor::Dispose()
{
    if (_device)
    {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(_context);
        alcCloseDevice(_device);
        _device = nullptr;
    }
}

OpenALProcessor::~OpenALProcessor()
{
    Dispose();
}

bool OpenALProcessor::IsLoaded()
{
    return _device != nullptr;
}

static inline ALenum to_al_format(short channels, short samples)
{
        bool stereo = (channels > 1);

        switch (samples) {
        case 16:
                if (stereo)
                        return AL_FORMAT_STEREO16;
                else
                        return AL_FORMAT_MONO16;
        case 8:
                if (stereo)
                        return AL_FORMAT_STEREO8;
                else
                        return AL_FORMAT_MONO8;
        default:
                return -1;
        }
}

unsigned int OpenALProcessor::RegisterSound(std::string sound, bool looped)
{
    ALuint source;

    alGenSources((ALuint)1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, looped ? AL_TRUE : AL_FALSE);

    WaveInfo *wave;
    char *bufferData;
    int ret;
    wave = WaveOpenFileForReading(sound.c_str());
    if (!wave) {
            return -1;
    }
    ret = WaveSeekFile(0, wave);
    if (ret) {
            return -1;
    }
    bufferData = (char*)malloc(wave->dataSize);
    if (!bufferData) {
            return -1;
    }
    ret = WaveReadFile(bufferData, wave->dataSize, wave);
    if (ret != wave->dataSize) {
            return -1;
    }

    ALuint buffer;
    alGenBuffers((ALuint)1, &buffer);

	alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
			bufferData, wave->dataSize, wave->sampleRate);

    alSourcei(source, AL_BUFFER, buffer);

    return source;
}

void OpenALProcessor::PlaySound(unsigned int id)
{
    alSourcePlay(id);
}

void OpenALProcessor::StopSound(unsigned int id)
{
    alSourceStop(id);
}
