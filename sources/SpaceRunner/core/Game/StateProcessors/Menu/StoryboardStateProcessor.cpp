#include "StoryboardStateProcessor.h"
#include "RenderProcessor.h"
#include "Game/Game.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Scores.h"

#define RENDER_FRAMES_WIDTH 4
#define RENDER_FRAMES_HEIGHT 5
#define RENDER_TIME 2.0f

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

        // Adding mines
        _minePos[0] = { 100, -20, 150};
        _minePos[1] = {70, 100, 180};
        for (auto i = 0; i < 2; i++)
        {
            auto mineNode = sceneManager->createSceneNode();
            sceneNode->addChild(mineNode);
            _sectorMine[i] = new SceneSector(mineNode);
            _modelMine[i] = new ModelDrawable(_sectorMine[i], "Mine.mesh");
            _modelMine[i]->SetScale(1100);
            _sectorMine[i]->GetNode()->setDirection(-1.0f, 0.9f, -0.8f);
            _modelMine[i]->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);
            _explosionPos[i] = _minePos[i] +  Ogre::Vector3(-50.0f, -20.0f, 0) * (i+1) * 2.0f;
            _sectorMine[i]->GetNode()->setPosition(_minePos[i] +  Ogre::Vector3(-50.0f, -20.0f, 0) * (i+1) * 2.0f);
        }

        // Adding ships
        _shipPos[0] = { 150, -15, 150};
        _shipPos[1] = {130, 100, 180};
        for (auto i = 0; i < 2; i++)
        {
            auto shipNode = sceneManager->createSceneNode();
            sceneNode->addChild(shipNode);
            _sectorShip[i] = new SceneSector(shipNode);
            _modelShip[i] = new ModelDrawable(_sectorShip[i], "ship.mesh");
            _modelShip[i]->SetScale(100);
            _sectorShip[i]->GetNode()->setDirection(-1.0f, 0.9f, -0.8f);
            _modelShip[i]->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);
            _sectorShip[i]->GetNode()->setPosition(_shipPos[i] +  Ogre::Vector3(-50.0f, -20.0f, 0) * i * 3.0f);

            // Engine
            auto sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setPosition(0.25f, 0.03f, 0.12f);
            shipNode->addChild(sceneNodeChild);

            std::stringstream ss;
            ss << "EngineFire_board" << i;
            _engineEffect[0] = sceneManager->createParticleSystem(ss.str(), "EngineBig");
            _engineEffect[0]->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY + 2);
            sceneNodeChild->attachObject(_engineEffect[0]);

            sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setPosition(0.25f, 0.03f, -0.12f);
            shipNode->addChild(sceneNodeChild);

            ss.str("");
            ss << "EngineFire2_board" << i;
            _engineEffect[1] = sceneManager->createParticleSystem(ss.str(), "EngineBig");
            _engineEffect[1]->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY + 2);
            sceneNodeChild->attachObject(_engineEffect[1]);
        }

        // Adding blaster shots
        for (auto i = 0; i < 2; i++)
        {
            for (auto r = 0; r < 4; r++)
            {
                auto blastNode = sceneManager->createSceneNode();
                //sceneNode->addChild(blastNode);
                _sectorShip[i]->GetNode()->addChild(blastNode);
                _sectorBlaster[i*4 + r] = new SceneSector(blastNode);

                std::vector<Vector3> pointList;
                pointList.push_back(Vector3(0.4f, 0.0, 0.1f));
                pointList.push_back(Vector3(-0.4f, 0.0, 0.1f));
                pointList.push_back(Vector3(-0.4f, 0.0,  -0.1f));
                pointList.push_back(Vector3(0.4f, 0.0, -0.1f));
                float sign = r % 2 ? -1.0f : 1.0f;
                float step = r < 2 ? -0.8f : 0.0f;
                _blasterPos[i * 4 + r] = Ogre::Vector3(step, 0, -0.1f * sign);
                blastNode->setPosition(_blasterPos[i * 4 + r]);
                _blaster[i*4 + r] = new RectDrawable(_sectorBlaster[i*4 + r], "BlasterShotMaterial", pointList);
                _blaster[i*4 + r]->SetPriority(Ogre::RENDER_QUEUE_OVERLAY + 1);
            }
        }

        // Adding cruiser
        auto cruiserNode = sceneManager->createSceneNode();
        sceneNode->addChild(cruiserNode);
        _sectorCruiser = new SceneSector(cruiserNode);
        _modelCruiser = new ModelDrawable(_sectorCruiser, "Cruiser.mesh");
        _modelCruiser->SetScale(30);
        _sectorCruiser->GetNode()->setDirection(2.1f, 3.6f, 2.7f);
        _sectorCruiser->GetNode()->setPosition(0,0,100);
        _modelCruiser->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);

        // Adding turret
        auto turretNode = sceneManager->createSceneNode();
        cruiserNode->addChild(turretNode);
        _sectorTurret = new SceneSector(turretNode);
        _modelTurret = new ModelDrawable(_sectorTurret, "CruiserTurret.mesh");
        _modelTurret->SetScale(3);
        _sectorTurret->GetNode()->setPosition(0,0,-0.7f);
        _modelTurret->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 2);

        // Adding cruiser engine
        auto sceneNodeCruiserEngine = sceneManager->createSceneNode();
        sceneNodeCruiserEngine->setPosition(0, -3.8f, 0);
        cruiserNode->addChild(sceneNodeCruiserEngine);
        _engineEffect[2] = sceneManager->createParticleSystem("CruiserEngine_storyboard", "EngineRightBig");
        _engineEffect[2]->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY + 2);
        sceneNodeCruiserEngine->attachObject(_engineEffect[2]);

        // Adding circle
        auto circleNode = sceneManager->createSceneNode();
        sceneNode->addChild(circleNode);
        _sectorCircle = new SceneSector(circleNode);
        std::vector<Vector3> pointList;
        pointList.push_back(Vector3(-250.0f, 250, 0));
        pointList.push_back(Vector3(250.0f, 250, 0));
        pointList.push_back(Vector3(250.0f, -250,  0));
        pointList.push_back(Vector3(-250.0f, -250, 0));
        _sectorCircle->GetNode()->setPosition(0, -40, 0);
        _sectorCircle->GetNode()->setDirection(0.0f, 0.2f, 0.1f);
        _circle = new RectDrawable(_sectorCircle, "SelectionCircleMaterial", pointList);
        _circle->SetPriority(Ogre::RENDER_QUEUE_OVERLAY + 2);


        sceneNode->setPosition(Ogre::Vector3(0, 0, 0));
        //_explosionEffect[0][0] = nullptr;
        //_explosionEffect[1][0] = nullptr;


        // Create texture to render
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

        // Creating spritesheet
        _spriteSheet = Ogre::TextureManager::getSingleton().createManual(
                "DynamicTexture", // name
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::TEX_TYPE_2D,      // type
                RENDER_FRAMES_WIDTH * width, RENDER_FRAMES_HEIGHT * height,         // width & height
                0,                // number of mipmaps
                Ogre::PF_BYTE_BGRA,     // pixel format
                Ogre::TU_DEFAULT);

        _renderSpritesheet = false;
        _totalTime = 0;

    }

    StoryboardStateProcessor::~StoryboardStateProcessor()
    {
        delete _document;
    }

    // save texture to disk
    void SaveImage(Ogre::TexturePtr TextureToSave, std::string filename)
    {
        Ogre::HardwarePixelBufferSharedPtr readbuffer;
        readbuffer = TextureToSave->getBuffer(0, 0);
        readbuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL );
        const Ogre::PixelBox &readrefpb = readbuffer->getCurrentLock();
        unsigned char *readrefdata = static_cast<unsigned char*>(readrefpb.data);

        Ogre::Image img;
        img = img.loadDynamicImage (readrefdata, TextureToSave->getWidth(),
                                    TextureToSave->getHeight(), TextureToSave->getFormat());
        img.save(filename);

        readbuffer->unlock();
    }

    GameState::State StoryboardStateProcessor::Update(float time)
    {
        _totalTime += time;
        if (_changeState == 1)
            _changeState = 2;
        else if (_changeState == 2)
        {
            _changeState = 0;
            return GameState::Level;
        }

        // Update explosions and mines
        for (auto i = 0; i < 2; i++)
        {
            if (_explosionEffect[i][0] != nullptr)
            {
                if (_explosionEffect[i][0]->IsFinished())
                {
                    InitExplosions(VectorFromOgre(_explosionPos[i]), i, 0);
                    _sectorMine[i]->GetNode()->setVisible(false);
                }
            }
            if (_explosionEffect[i][0]->getRemainingTime() < 0.5f)
            {
                _sectorMine[i]->GetNode()->setVisible(true);
            }
            for (auto r = 0; r < 4; r++)
                _explosionEffect[i][r]->Update(time);
        }



        // Update ships
        for (auto i = 0; i < 2; i++)
        {
            Ogre::Vector3 pos = _sectorShip[i]->GetNode()->getPosition();
            pos = pos + Ogre::Vector3(-70.0f, -20.0f, 0) * time * 2;
            if (pos.x < _shipPos[i].x - 280)
            {
                for (auto r = 0;r < 4; r++)
                    _sectorBlaster[i*4 + r]->GetNode()->setPosition(_blasterPos[i*4 + r]);
                pos = _shipPos[i];
            }
            _sectorShip[i]->GetNode()->setPosition(pos);
        }

        // Update shots
        for (auto i = 0; i < 8; i++)
        {
            Ogre::Vector3 pos = _sectorBlaster[i]->GetNode()->getPosition();
            pos = pos + Ogre::Vector3(-0.5f, 0, 0) * time * 2;
            /*if (pos.x < -1.3f - 2.0f)
                pos.x = 0;*/
            _sectorBlaster[i]->GetNode()->setPosition(pos);
        }

        // Update circle
        _sectorCircle->GetNode()->rotate(Ogre::Quaternion(Ogre::Degree(time*45), Ogre::Vector3::UNIT_Z));

        // rendering spritesheet
        if (_renderSpritesheet)
        {
            constexpr float frameTime = RENDER_TIME / (RENDER_FRAMES_WIDTH * RENDER_FRAMES_HEIGHT);
            if ((float)_currentFrame * frameTime + _renderStarted < _totalTime)
            {
                auto pixelBox = _rttTexture->getBuffer()->lock(
                        Ogre::Box(0,0,_rttTexture->getWidth(), _rttTexture->getHeight()),
                        Ogre::HardwareBuffer::LockOptions::HBL_NORMAL);

                int leftOffset = _currentFrame % RENDER_FRAMES_WIDTH;
                int topOffset = _currentFrame / RENDER_FRAMES_WIDTH;

                _spriteSheet->getBuffer()->blitFromMemory(
                        pixelBox,
                        Ogre::Box(leftOffset * _rttTexture->getWidth(),
                                  topOffset * _rttTexture->getHeight(),
                                  (leftOffset+1) * _rttTexture->getWidth(),
                                  (topOffset+1) * _rttTexture->getHeight()));
                //_spriteSheet->getBuffer()->copyData(*_rttTexture->getBuffer(), 0, size * _currentFrame, size);
                _rttTexture->getBuffer()->unlock();
                _currentFrame++;
            }
            if (_currentFrame == RENDER_FRAMES_WIDTH * RENDER_FRAMES_HEIGHT)
            {
                _renderSpritesheet = false;
                SaveImage(_spriteSheet, "spritesheet.png");
            }
        }


        return GameState::Storyboard;
    }

    void StoryboardStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void StoryboardStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
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

            _renderSpritesheet=true;
            _currentFrame=0;
            _renderStarted = _totalTime;
        }

        /*static float x = -1.7f;
        static float y = 1.3f;
        static float z = -0.8f;
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
        _sectorShip[0]->GetNode()->resetOrientation();
        _sectorShip[0]->GetNode()->setDirection(x, y, z);*/
    }


    void StoryboardStateProcessor::Hide()
    {
        _sector->GetNode()->setVisible(false);
        _document->Hide();
        RenderProcessor::Instance()->GetCamera()->SetPerspective();
    }

    void StoryboardStateProcessor::Show()
    {
        _sector->GetNode()->setVisible(true);
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

        InitExplosions(VectorFromOgre(_minePos[0]), 0, 0.0f);
        InitExplosions(VectorFromOgre(_minePos[1]), 1, 0.5f);
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
                1800);
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 0, 0));


        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += 1;
        pos.z += 500;
        light->SetPosition(pos);
    }

    void StoryboardStateProcessor::InitExplosions(Vector3 pos, int num, float time)
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
            _explosionEffect[num][i] = new ParticleSystem(sceneNodeChild, templateName, str.str(), 2, 10 - i, true);
            _explosionEffect[num][i]->FastForward(time);
        }
    }


}