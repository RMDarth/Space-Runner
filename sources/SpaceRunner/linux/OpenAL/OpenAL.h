#include <AL/al.h>
#include <AL/alc.h>
#include <string>

class OpenALProcessor
{
    ALCdevice *_device;
    ALCcontext *_context;
    static OpenALProcessor * _instance;

    OpenALProcessor();
public:
    ~OpenALProcessor();
    void Dispose();

    static OpenALProcessor* Instance();

    bool IsLoaded();
    unsigned int RegisterSound(std::string sound, bool looped = false);
    void PlaySound(unsigned int id);
    void StopSound(unsigned int id);

};
