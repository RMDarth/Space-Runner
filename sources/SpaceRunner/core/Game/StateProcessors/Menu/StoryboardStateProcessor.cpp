#include "StoryboardStateProcessor.h"
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

        for (auto i = 0; i < LEVELNUM; i++)
        {
            std::stringstream ss;
            ss << (i+1);
            _document->GetControlByName(ss.str())->SetDefaultMaterial("LevelEffectMaterial");
        }

        auto *material = Ogre::MaterialManager::getSingleton().getByName("StarAnimatedMaterial").get();
        for (auto i = 0; i < 3; i++)
        {
            _starMaterials[i] = material->clone("StarAnimatedMaterial_"+std::to_string(i));
            _starTime[i] = (float)2-i;
        }

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
            return GameState::ShipSelect;
        }

        _alpha += time * 0.75;
        while (_alpha > 1.0f)
            _alpha -= 1.0f;

        auto material = Ogre::MaterialManager::getSingleton().getByName("LevelEffectMaterial").get();
        material->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("color", Ogre::ColourValue(1, 1, 1, _alpha));

        for (auto i = 0; i < 3; i++)
        {
            _starTime[i] += time * ((i+1) * 0.4 + 1);
            while (_starTime[i] > 3.0f)
                _starTime[i] -= 3.0f;

            auto program = _starMaterials[i]->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
            program->setNamedConstant("color",Ogre::ColourValue(1,1,1,_starTime[i] > 1.0f ? 1.0f : _starTime[i]));
        }

        return GameState::Storyboard;
    }

    void StoryboardStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
        _moving = true;
        _moveShift = x;
        _bigMove = false;
    }

    void StoryboardStateProcessor::OnMouseUp(int x, int y)
    {
        if (_moving && _document->GetControlByName("panel")->OnMouseUp(x, y))
        {
            _moving  = false;
            if (!_bigMove)
                _document->OnMouseUp(x, y);
        } else {
            _document->OnMouseUp(x, y);
        }
    }

    void StoryboardStateProcessor::OnMouseMove(int x, int y)
    {
        int left, top, width, height;
        RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);
        float aspect = (float) width / height;

        if (abs(_moveShift - x) > width * 0.1)
            _bigMove = true;

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
    }


    void StoryboardStateProcessor::Hide()
    {
        _sector->GetNode()->setVisible(false);
        _document->Hide();
    }

    void StoryboardStateProcessor::Show()
    {
        _sector->GetNode()->setVisible(true);
        _document->Show();

        SetLightAndCamera();


        bool previous = true;
        for (int i = 0; i < LEVELNUM; i++)
        {
            int stars = Scores::Instance()->GetBestStars(i + 1);

            if (stars > 0)
            {
                previous = true;
                _document->GetControlByName("lock" + std::to_string(i+1))->SetVisible(false);
            } else if (previous)
            {
                previous = false;
                _document->GetControlByName("lock" + std::to_string(i+1))->SetVisible(false);
            }

            for (int r = 0; r < stars; r++)
            {
                auto control = _document->GetControlByName("star" + std::to_string(i+1) + "_" + std::to_string(r+1));
                control->SetDefaultMaterial("StarAnimatedMaterial_"+std::to_string(r));
            }
            for (int r = stars; r < 3; r++)
            {
                auto control = _document->GetControlByName("star" + std::to_string(i+1) + "_" + std::to_string(r+1));
                control->SetDefaultMaterial("GrayStar.png");
            }
        };
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
        else if (control->GetClassType() == "Button")
        {
            int levelNum = stoi(control->GetName());
            if (!_document->GetControlByName("lock" + std::to_string(levelNum))->IsVisible())
            {
                LevelManager::Instance()->SetIsPuzzle(true);
                LevelManager::Instance()->SetLevelNum(levelNum);
                LevelManager::Instance()->SetStarted(false);
                _changeState = 1;
            }
            //_document->Hide();
        }
    }

    void StoryboardStateProcessor::SetLightAndCamera()
    {
        auto camera = RenderProcessor::Instance()->GetCamera();
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