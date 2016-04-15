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

        auto rectNode = sceneManager->createSceneNode();
        _sectorLevel = new SceneSector(rectNode);
        _sectorLevel->GetNode()->setPosition(0,0, 20);

        //_sectorLevel->GetNode()->setDirection(0,0,1);
        sceneNode->addChild(rectNode);
        std::vector<Vector3> pointList;
        pointList.push_back(Vector3(-35, 25, 0));
        pointList.push_back(Vector3(-35, -25, 0));
        pointList.push_back(Vector3(35, -25, 0));
        pointList.push_back(Vector3(35, 25, 0));
        _levelEffect = new RectDrawable(_sectorLevel, "LevelEffectMaterial", pointList);
        _levelEffect->SetRenderingQueue(Ogre::RENDER_QUEUE_OVERLAY + 2);
        rectNode->setScale(4, 4, 4);

        sceneNode->setPosition(Ogre::Vector3(0, 0, 0));
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

        _alpha += time * 1;
        while (_alpha > 1.0f)
            _alpha -= 1.0f;

        auto material = Ogre::MaterialManager::getSingleton().getByName("LevelEffectMaterial").get();
        material->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("color", Ogre::ColourValue(1, 1, 1, _alpha));

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
        if (_moving)
        {
            int x, y;
            _document->GetControlByName("panel")->GetPos(x, y);
            _sector->GetNode()->setPosition(Ogre::Vector3((float)x * aspect,0,0));
            SetLightAndCamera();
        }

    }

    void StoryboardStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::MainMenu);
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
        _sectorLevel->GetNode()->setVisible(false);
        _document->Hide();
        RenderProcessor::Instance()->GetCamera()->SetPerspective();
    }

    void StoryboardStateProcessor::Show()
    {
        _sectorLevel->GetNode()->setVisible(true);
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
        }*/;
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
                200);
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 0, 0));


        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += 1;
        light->SetPosition(pos);
    }


}