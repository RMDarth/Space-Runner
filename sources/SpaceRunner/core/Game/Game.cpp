#include "Game.h"
#include "StateProcessorFactory.h"
#include "SoundSystem.h"
#include "BillingProcessor.h"
#include "Config.h"
#include "SkinManager.h"
#include "Game/StateProcessors/Menu/MenuStateProcessor.h"
#include "Game/StateProcessors/Menu/PauseStateProcessor.h"
#include "Game/StateProcessors/Menu/ScoreStateProcessor.h"
#include "Game/StateProcessors/Menu/StoryboardStateProcessor.h"
#include "Game/StateProcessors/Menu/ChallengesStateProcessor.h"
#include "Game/StateProcessors/Menu/MessageStateProcessor.h"
#include "Game/StateProcessors/Menu/StoreStateProcessor.h"
#include "Game/StateProcessors/Menu/SelectShipStateProcessor.h"
#include "Game/StateProcessors/Menu/CreditsStateProcessor.h"
#include "Game/StateProcessors/Menu/AchievementsStateProcessor.h"
#include "Game/StateProcessors/Menu/SaveMeStateProcessor.h"
#include "Game/StateProcessors/Race/RaceStateProcessor.h"
using namespace CoreEngine;

Game* Game::_game = NULL;

Game* Game::Instance()
{
    if (_game == NULL)
    {
        _game = new Game();
    }
    return _game;
}

Game::Game()
{
    MenuStateProcessor * menu = new MenuStateProcessor();
    RaceStateProcessor * race = new RaceStateProcessor();
    PauseStateProcessor * pause = new PauseStateProcessor();
    SaveMeStateProcessor * saveme = new SaveMeStateProcessor();
    SelectShipStateProcessor * selectShip = new SelectShipStateProcessor();
    ChallengesStateProcessor * challenges = new ChallengesStateProcessor();

#if OGRE_PLATFORM == OGRE_PLATFORM_WINRT || defined(CHINA_SHOP)
    AchievementsStateProcessor *achievement = new AchievementsStateProcessor();
#endif
    ScoreStateProcessor * score = new ScoreStateProcessor();
    StoryboardStateProcessor * storyboard = new StoryboardStateProcessor();

    MessageStateProcessor* message = new MessageStateProcessor();
#if OGRE_PLATFORM != OGRE_PLATFORM_WINRT
    StoreStateProcessor* store = new StoreStateProcessor();
#endif
    CreditsStateProcessor *credits = new CreditsStateProcessor();

    race->Hide();
    storyboard->Hide();

    StateProcessorFactory::Instance()->RegisterProcessor(race, GameState::Level);
    StateProcessorFactory::Instance()->RegisterProcessor(menu, GameState::MainMenu);
    StateProcessorFactory::Instance()->RegisterProcessor(pause, GameState::Pause);
    StateProcessorFactory::Instance()->RegisterProcessor(saveme, GameState::SaveMe);
    StateProcessorFactory::Instance()->RegisterProcessor(selectShip, GameState::ShipSelect);
    StateProcessorFactory::Instance()->RegisterProcessor(score, GameState::Score);

#if OGRE_PLATFORM == OGRE_PLATFORM_WINRT || defined(CHINA_SHOP)
    StateProcessorFactory::Instance()->RegisterProcessor(achievement, GameState::Achievements);
#endif
    StateProcessorFactory::Instance()->RegisterProcessor(storyboard, GameState::Storyboard);
    StateProcessorFactory::Instance()->RegisterProcessor(challenges, GameState::Challenges);
    StateProcessorFactory::Instance()->RegisterProcessor(message, GameState::Message);
#if OGRE_PLATFORM != OGRE_PLATFORM_WINRT
    StateProcessorFactory::Instance()->RegisterProcessor(store, GameState::Store);
#endif
    StateProcessorFactory::Instance()->RegisterProcessor(credits, GameState::Credits);

    _state = GameState::MainMenu;

#ifdef __ANDROID_API__
    SoundSystem::Instance()->InitBackgroundMusic("Sound/Music.ogg");
#else
    SoundSystem::Instance()->InitBackgroundMusic("Sound/Music.wav");
#endif

    if (Config::Instance()->IsSignedGoogle())
    {
        BillingProcessor::Instance()->LogInGoogle();
    }

    menu->Show();

    if (Config::Instance()->IsMusicEnabled())
    {
        SoundSystem::Instance()->StartBackgroundMusic();
    }
    SkinManager::Instance()->SetSkinID(Config::Instance()->GetSelectedModel());
    //SkinManager::Instance()->SetSkinID(0);

}

void Game::Update(float time)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    auto state = processor->Update(time);

    if (state != _state)
        ChangeState(state);
}


bool Game::OnMouseMove(int x, int y, float time)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnMouseMove(x, y);

    return false;
}

bool Game::OnMouseDown(int x, int y)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnMouseDown(x, y);

    return false;
}

bool Game::OnMouseUp(int x, int y)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnMouseUp(x, y);

    return false;
}

bool Game::OnMouseDoubleClick(int x, int y)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnMouseDoubleClick(x, y);

    return false;
}

void Game::OnKeyPressed(OIS::KeyCode key)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnKeyPressed(key);
}

void Game::OnKeyReleased(OIS::KeyCode key)
{
    IStateProcessor* processor = StateProcessorFactory::Instance()->GetProcessor(_state);
    processor->OnKeyReleased(key);
}

void Game::ChangeState(GameState::State state)
{
    if (!StateProcessorFactory::Instance()->GetProcessor(state)->IsOverlapping())
    {
        StateProcessorFactory::Instance()->GetProcessor(_state)->Hide();

        for (auto i = _overlappedStateList.begin(); i != _overlappedStateList.end(); i++)
            StateProcessorFactory::Instance()->GetProcessor(*i)->Hide();
        _overlappedStateList.clear();
    } else {
        _overlappedStateList.push_back(_state);
    }

    _state = state;
    StateProcessorFactory::Instance()->GetProcessor(_state)->Show();
}

GameState::State Game::GetState()
{
    return _state;
}

