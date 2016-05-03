#include <Game/Scores.h>
#include "MenuStateProcessor.h"
#include "Render/SceneSector.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"
#include "Game/Config.h"
#include "SoundSystem.h"
#include "BillingProcessor.h"

#include "OgreRectangle2D.h"

#define SIZE 2.5f
#define CAMERA_RADIUS 8
#define CAMERA_SPEED 0.35f

namespace CoreEngine
{
    MenuStateProcessor::MenuStateProcessor()
    {
        _totalTime = 0;
        _logged = false;
        GenerateLevel();

        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sectorShip = new SceneSector(sceneNode);

        _model = new ModelDrawable(_sectorShip, "ship.mesh");
        _model->SetScale(5.0);

        sceneNode->setPosition(Ogre::Vector3(0, 3, 0));

        // Engine fire
        auto sceneNodeChild = sceneManager->createSceneNode();
        sceneNodeChild->setPosition(0.24f, 0.05f, 0.13f);
        sceneNode->addChild(sceneNodeChild);

        auto engineFire1 = sceneManager->createParticleSystem("EngineFire12", "Engine");
        sceneNodeChild->attachObject(engineFire1);

        sceneNodeChild = sceneManager->createSceneNode();
        sceneNodeChild->setPosition(0.24f, 0.05f, -0.13f);
        sceneNode->addChild(sceneNodeChild);

        auto engineFire2 = sceneManager->createParticleSystem("EngineFire13", "Engine");
        sceneNodeChild->attachObject(engineFire2);

        _document = new ControlDocument("GUI/startmenu.xml");
        _document->SetMouseUpHandler(this);

        auto configpanel = _document->GetControlByName("configpanel");
        int x, y;
        configpanel->GetPos(x, y);
        y = y - (int)(configpanel->GetWidth() * 3.448f);
        configpanel->SetPos(x, y);
        configpanel->SetSize(configpanel->GetWidth(), (int)(configpanel->GetWidth() * 4.348f));
        configpanel->SetRenderOrder(0);

        x = x + (int)(configpanel->GetWidth() * 0.05f);
        auto pigbutton = _document->GetControlByName("graphics");
        pigbutton->SetPos(x, y);
        pigbutton->SetSize(pigbutton->GetWidth(), pigbutton->GetWidth());

        y = y + (int)(pigbutton->GetWidth() * 0.95f);
        auto soundbutton = _document->GetControlByName("sound");
        soundbutton->SetPos(x, y);
        soundbutton->SetSize(soundbutton->GetWidth(), soundbutton->GetWidth());

        y = y + (int)(soundbutton->GetWidth() * 0.95f);
        auto musicbutton = _document->GetControlByName("music");
        musicbutton->SetPos(x, y);
        musicbutton->SetSize(musicbutton->GetWidth(), musicbutton->GetWidth());

        y = y + (int)(soundbutton->GetWidth() * 0.95f);
        auto infobutton = _document->GetControlByName("info");
        infobutton->SetPos(x, y);
        infobutton->SetSize(infobutton->GetWidth(), infobutton->GetWidth());

        Show();

        configpanel->SetVisible(false);
        pigbutton->SetVisible(false);
        soundbutton->SetVisible(false);
        musicbutton->SetVisible(false);
        infobutton->SetVisible(false);

        UpdateSoundIcon(soundbutton.get());
        UpdateMusicIcon(musicbutton.get());
#ifdef CHINA_SHOP
        UpdateGooglePlayIcon(_document->GetControlByName("googleplay").get());
#endif
    }

    MenuStateProcessor::~MenuStateProcessor()
    {
        delete _model;
        delete _sector;
        delete _document;
    }

    GameState::State MenuStateProcessor::Update(float time)
    {
        _totalTime += time;
        SetLightAndCamera();

#ifdef __ANDROID_API__
        if (BillingProcessor::Instance()->IsLoggedGoogle())
        {
            if (!_logged)
            {
                Config::Instance()->SetSignedGoogle(true);
                UpdateGooglePlayIcon(_document->GetControlByName("googleplay").get());
                BillingProcessor::Instance()->SyncAchievements();
            }
            _logged = true;
        }
        else {
            if (_logged)
            {
                UpdateGooglePlayIcon(_document->GetControlByName("googleplay").get());
            }
            _logged = false;
        }
#endif

        return GameState::MainMenu;
    }


    void MenuStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void MenuStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void MenuStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void MenuStateProcessor::GenerateLevel()
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();


        std::string image = "gamelogo.png";
        // Create background material
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(image, "General");
        material->getTechnique(0)->getPass(0)->setVertexProgram("simpletex_vs");
        material->getTechnique(0)->getPass(0)->setFragmentProgram("simpletex_ps");
        auto state = material->getTechnique(0)->getPass(0)->createTextureUnitState(image);
        state->setTextureNameAlias("diffuseMap");
        state->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
        //material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);

        // Create background rectangle covering the whole screen
        Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
        rect->setCorners(-0.8f, 0.7f, 0.8f, 0.3f);
        rect->setMaterial(image);

        // Render the background before everything else
        rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY-2);

        // Use infinite AAB to always stay visible
        Ogre::AxisAlignedBox aabInf;
        aabInf.setInfinite();
        rect->setBoundingBox(aabInf);

        // Attach background to the scene
        Ogre::SceneNode* node = sceneManager->getRootSceneNode()->createChildSceneNode("Background2");
        node->attachObject(rect);
        _sector = new SceneSector(node);
    }

    void MenuStateProcessor::SetLightAndCamera()
    {
        auto camera = RenderProcessor::Instance()->GetCamera();
        Vector3 pos = Vector3(
                CAMERA_RADIUS*cos(_totalTime * CAMERA_SPEED),
                1.5 * SIZE,
                CAMERA_RADIUS*sin(_totalTime * CAMERA_SPEED));
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 1.5 * SIZE,0));

        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += SIZE;
        light->SetPosition(pos);
    }

    void MenuStateProcessor::Hide()
    {
        _sector->GetNode()->setVisible(false);
        _sectorShip->GetNode()->setVisible(false);
        _document->Hide();
    }

    void MenuStateProcessor::Show()
    {
        _sector->GetNode()->setVisible(true);
        _sectorShip->GetNode()->setVisible(true);
        _document->Show();
        ChangeSettingsPanelVisibility();
        //UpdateGooglePlayIcon(_document->GetControlByName("googleplay").get());
        _document->GetControlByName("totalenergy")->SetText(std::to_string(Scores::Instance()->GetTotalEnergy()));
    }

    void MenuStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        static int scoresNum = 0;

        if (type == IEventHandler::MouseUp)
        {
            if (control->GetName() == "start")
            {
                LevelManager::Instance()->SetIsPuzzle(false);
                LevelManager::Instance()->SetLevelNum(1);
                Game::Instance()->ChangeState(GameState::ShipSelect);
            }

            if (control->GetName() == "editor")
            {
                Game::Instance()->ChangeState(GameState::Editor);
            }

            if (control->GetName() == "story")
            {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }

            if (control->GetName() == "sound")
            {
                Config::Instance()->SetSoundEnabled(!Config::Instance()->IsSoundEnabled());
                UpdateSoundIcon(control);
            }
            if (control->GetName() == "music")
            {
                auto config = Config::Instance();
                bool enabled = !config->IsMusicEnabled();
                config->SetMusicEnabled(enabled);
                if (enabled)
                    SoundSystem::Instance()->StartBackgroundMusic();
                else
                    SoundSystem::Instance()->StopBackgroundMusic();

                UpdateMusicIcon(control);
            }
            if (control->GetName() == "info")
            {
                //Game::Instance()->ChangeState(GameState::Editor);
                Game::Instance()->ChangeState(GameState::Credits);
            }

            if (control->GetName() == "graphics")
            {
                Game::Instance()->ChangeState(GameState::ShipSelect);
            }

            if (control->GetName() == "config")
            {
                ChangeSettingsPanelVisibility();
            }

            if (control->GetName() == "store")
            {
                Game::Instance()->ChangeState(GameState::Store);
            }

            if (control->GetName() == "googleplay")
            {
#ifdef __ANDROID_API__
#ifndef CHINA_SHOP
                if (!BillingProcessor::Instance()->IsLoggedGoogle())
                    BillingProcessor::Instance()->LogInGoogle();
                else
                    BillingProcessor::Instance()->ShowAchievements();
#else
                Game::Instance()->ChangeState(GameState::Achievements);
#endif
#else
                Game::Instance()->ChangeState(GameState::Achievements);
#endif
            }

            if (control->GetName() == "facebook")
            {
                BillingProcessor::Instance()->ShareOnFacebook();
            }
            if (control->GetName() == "rate")
            {
                BillingProcessor::Instance()->RateApp();
            }
        }
    }

    void MenuStateProcessor::ChangeSettingsPanelVisibility()
    {
        auto cp = _document->GetControlByName("configpanel");
        bool visibility = !cp->IsVisible();
        cp->SetVisible(visibility);
        _document->GetControlByName("graphics")->SetVisible(visibility);
        _document->GetControlByName("sound")->SetVisible(visibility);
        _document->GetControlByName("music")->SetVisible(visibility);
        _document->GetControlByName("info")->SetVisible(visibility);
    }

    void MenuStateProcessor::UpdateSoundIcon(Control* control)
    {
        if (!Config::Instance()->IsSoundEnabled())
        {
            control->SetDefaultMaterial("buttons/NoSound.png");
            control->SetHoverMaterial("buttons/NoSound.png");
        }
        else {
            control->SetDefaultMaterial("buttons/Sound.png");
            control->SetHoverMaterial("buttons/Sound.png");
        }
    }

    void MenuStateProcessor::UpdateGooglePlayIcon(Control* control)
    {
        if (BillingProcessor::Instance()->IsLoggedGoogle())
        {
            control->SetDefaultMaterial("buttons/googleplay.png");
            control->SetHoverMaterial("buttons/googleplay.png");
            control->SetPushMaterial("buttons/googleplay.png");
        }
        else {
#ifndef CHINA_SHOP
            control->SetDefaultMaterial("buttons/googleplay_gray.png");
            control->SetHoverMaterial("buttons/googleplay_gray.png");
            control->SetPushMaterial("buttons/googleplay_gray.png");
#else
            control->SetDefaultMaterial("buttons/achievments.png");
            control->SetHoverMaterial("buttons/achievments.png");
            control->SetPushMaterial("buttons/achievments.png");
#endif
        }
    }

    void MenuStateProcessor::UpdateMusicIcon(Control* control)
    {
        if (!Config::Instance()->IsMusicEnabled())
        {
            control->SetDefaultMaterial("buttons/NoMusic.png");
            control->SetHoverMaterial("buttons/NoMusic.png");
        }
        else {
            control->SetDefaultMaterial("buttons/Music.png");
            control->SetHoverMaterial("buttons/Music.png");
        }
    }
}

