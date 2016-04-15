#include "StoryboardStateProcessor.h"
#include "RenderProcessor.h"
#include "Game/Game.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Scores.h"

namespace CoreEngine
{

    StoryboardStateProcessor::StoryboardStateProcessor()
    {
        _document = new ControlDocument("GUI/storyboard.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _changeState = 0;

        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);

        _sector = new SceneSector(sceneNode);
        sceneNode->setScale(3.0f,3.0f, 3.0f);

        auto shipNode = sceneManager->createSceneNode();
        sceneNode->addChild(shipNode);
        _sectorShip = new SceneSector(shipNode);
        _modelShip = new ModelDrawable(_sectorShip, "ship.mesh");
        _modelShip->SetScale(100);
        _sectorShip->GetNode()->setDirection(-1.7f, 1.3f, -0.8f);
        _modelShip->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);
        _sectorShip->GetNode()->setPosition(Ogre::Vector3(100, -5, 50));

        auto cruiserNode = sceneManager->createSceneNode();
        sceneNode->addChild(cruiserNode);
        _sectorCruiser = new SceneSector(cruiserNode);
        _modelCruiser = new ModelDrawable(_sectorCruiser, "Cruiser.mesh");
        _modelCruiser->SetScale(30);
        _sectorCruiser->GetNode()->setDirection(2.8f, 6.0f, 5.4f);
        _modelCruiser->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);


        sceneNode->setPosition(Ogre::Vector3(0, 0, 0));

        _explosionEffect[0] = nullptr;

        // create spritesheet tex
        int left, top, width, height;
        RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);

        _rttTexture = Ogre::TextureManager::getSingleton().createManual(
                        "RttTex",
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                        Ogre::TEX_TYPE_2D,
                        width, height,
                        0,
                        Ogre::PF_R8G8B8A8,
                        Ogre::TU_RENDERTARGET);

        Ogre::RenderTexture* renderTexture = _rttTexture->getBuffer()->getRenderTarget();

        renderTexture->addViewport(RenderProcessor::Instance()->GetCamera()->getPtr());
        renderTexture->getViewport(0)->setClearEveryFrame(true);
        renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
        renderTexture->getViewport(0)->setSkiesEnabled(false);
        renderTexture->getViewport(0)->setOverlaysEnabled(false);

    }

    StoryboardStateProcessor::~StoryboardStateProcessor()
    {
        delete _document;
    }

    GameState::State StoryboardStateProcessor::Update(float time)
    {
        if (_changeState == 1)
            _changeState = 2;
        else if (_changeState == 2)
        {
            _changeState = 0;
            return GameState::Level;
        }

        if (_explosionEffect[0] != nullptr)
        {
            if (_explosionEffect[0]->IsFinished())
            {
                InitExplosions(Vector3(-40, 30, 50));
            }
        }

        Ogre::Vector3 pos = _sectorShip->GetNode()->getPosition();
        pos = pos + Ogre::Vector3(-50.0f, -30.0f, 0) * time;
        if (pos.x < -100)
            pos = Ogre::Vector3(100, -5, 50);

        _sectorShip->GetNode()->setPosition(pos);

        for (auto i = 0; i < 4; i++)
            _explosionEffect[i]->Update(time);

        return GameState::Storyboard;
    }

    void StoryboardStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
        _moving = true;
    }

    void StoryboardStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
        _moving = false;
    }

    void StoryboardStateProcessor::OnMouseMove(int x, int y)
    {
        int left, top, width, height;
        RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);
        float aspect = (float) width / height;

        _document->OnMouseMove(x, y, 0);

    }

    void StoryboardStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
        }

        if (key == OIS::KC_P)
        {
            auto renderTexture = _rttTexture->getBuffer()->getRenderTarget();
            renderTexture->update();
            renderTexture->writeContentsToFile("screenshot.png");
        }

       /* static float x = 0;
        static float y = 0;
        static float z = 0;
        if (key == OIS::KC_RIGHT)
        {
            x += 0.1f;
        }
        if (key == OIS::KC_LEFT)
        {
            x -= 0.1f;
        }
        if (key == OIS::KC_UP)
        {
            y += 0.1f;
        }
        if (key == OIS::KC_DOWN)
        {
            y -= 0.1f;
        }
        if (key == OIS::KC_Z)
        {
            z += 0.1f;
        }
        if (key == OIS::KC_X)
        {
            z -= 0.1f;
        }
        _sectorShip->GetNode()->resetOrientation();
        _sectorShip->GetNode()->setDirection(x, y, z);*/
    }


    void StoryboardStateProcessor::Hide()
    {
        _sectorShip->GetNode()->setVisible(false);
        _document->Hide();
        RenderProcessor::Instance()->GetCamera()->SetPerspective();
    }

    void StoryboardStateProcessor::Show()
    {
        _sectorShip->GetNode()->setVisible(true);
        _document->Show();

        SetLightAndCamera();

        /*std::stringstream stream;
        for (int i = 0; i < 20; i++)
        {
            stream.str("");
            stream << (i+1);
            auto control = _document->GetControlByName(stream.str());

            stream.str("");
            stream << Scores::Instance()->GetBestStars(i + 1 + LevelManager::Instance()->GetWorld() * 20);
            control->SetCustomAttribute("stars", stream.str());
        }*/

        InitExplosions(Vector3(-40, 30, 50));
    }

    void StoryboardStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "back")
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
        }
        else if (control->GetName() == "reset")
        {
            Scores::Instance()->Reset();
            Show();
        }
        else if (control->GetName() != "panel")
        {
            LevelManager::Instance()->SetIsPuzzle(true);
            LevelManager::Instance()->SetLevelNum(LevelManager::Instance()->GetWorld() * 20 + stoi(control->GetName()));
            LevelManager::Instance()->SetStarted(false);
            _changeState = 1;
            _document->Hide();
        }
    }

    void StoryboardStateProcessor::SetLightAndCamera()
    {
        auto camera = RenderProcessor::Instance()->GetCamera();
        camera->SetOrthogonal();
        Vector3 pos = Vector3(
                0,
                0,
                300);
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 0, 0));


        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += 1;
        light->SetPosition(pos);
    }

    void StoryboardStateProcessor::InitExplosions(Vector3 pos)
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = _sector->GetNode();
        for (auto i = 0; i < 4; i++)
        {
            auto sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setPosition(VectorToOgre(pos));
            sceneNodeChild->setDirection(0, 1, 0);
            sceneNodeChild->setScale(10, 10, 10);
            sceneNode->addChild(sceneNodeChild);
            std::stringstream str;
            str << "StoryBlast" << (i + 1) << "_%d";
            std::string templateName = str.str();
            str.str("");
            str << "Blast" << (i + 1);
            _explosionEffect[i] = new ParticleSystem(sceneNodeChild, templateName, str.str(), 2, 10 - i, true);
        }
    }


}