#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "PrecompileHeaders.h"
#include "OgreApplication.h"
#include "RenderProcessor.h"
#include "SoundSystem.h"
#include "Config.h"
#include "Game.h"
#include "Input.h"
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;
using namespace std::chrono;
using namespace CoreEngine;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	OgreApplication::Instance()->Init();
	Config::Instance()->Load();

	bool soundEnabled = SoundSystem::Instance()->Init();
	if (!soundEnabled)
		Config::Instance()->SetSoundEnabled(false);
	Config::Instance()->SetLanguage(1);

	Game *game = Game::Instance();
	Input *input = new Input(OgreApplication::Instance()->GetWindow());
	input->AddListener((IInputListener*)game);

    high_resolution_clock::time_point oldtime = high_resolution_clock::now();
    high_resolution_clock::time_point time;
	float deltaTime;
	while (!OgreApplication::Instance()->IsExit())
	{
		Sleep(5);
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

	return 0;
}
