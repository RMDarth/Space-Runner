#include "PrecompileHeaders.h"
#include "OgreApplication.h"
#include "RenderProcessor.h"
#include "Game/Game.h"
#include "Input.h"
#include "Game/Config.h"
#include "SoundSystem.h"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;
using namespace CoreEngine;

void msleep(unsigned long milisec)
{
    struct timespec req={0};
    req.tv_sec=0;
    req.tv_nsec=milisec * 1000000L;
    nanosleep(&req,&req);
}

int main()
{
	OgreApplication::Instance()->Init();
	Config::Instance()->Load();
	bool soundEnabled = SoundSystem::Instance()->Init();
	if (!soundEnabled)
		Config::Instance()->SetSoundEnabled(false);

	Game *game = Game::Instance();
	Input *input = new Input(OgreApplication::Instance()->GetWindow());
	input->AddListener((IInputListener*)game);

	high_resolution_clock::time_point oldtime = high_resolution_clock::now();
	high_resolution_clock::time_point time;
	float deltaTime;

	while (!OgreApplication::Instance()->IsExit())
	{
	    msleep(5);
		high_resolution_clock::time_point time = high_resolution_clock::now();
		duration<float> time_span = duration_cast<duration<float>>(time - oldtime);
		deltaTime = time_span.count();

		if (deltaTime > 0)
		{
			RenderProcessor::Instance()->Update(deltaTime);
			input->Update(deltaTime);
			game->Update(deltaTime);
			OgreApplication::Instance()->LoopStep(deltaTime);

			oldtime = time;
		}
	}

	SoundSystem::Instance()->Dispose();

	return 0;
}
