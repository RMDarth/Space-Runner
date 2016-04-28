#include <RenderProcessor.h>
#include <Game/SkinManager.h>
#include "Render/SceneSector.h"
#include "Render/Drawables/ModelDrawable.h"
#include "SelectShipStateProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"

#define SIZE 2.5f
#define CAMERA_RADIUS 8
#define CAMERA_SPEED 0.35f

namespace CoreEngine
{

    SelectShipStateProcessor::SelectShipStateProcessor()
    {
        _document = new ControlDocument("GUI/selectship.xml");
        _document->SetMouseUpHandler(this);
        _document->Hide();

        _document->RaisePriority(2);

        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);

        sceneNode->setPosition(Ogre::Vector3(0, 4, 0));
        sceneNode->setDirection(Ogre::Vector3(-0.3, 0.1, 0.4));

        _skinId = SkinManager::Instance()->GetSkinID();

        UpdateModel();

        sceneNode->setVisible(false);
    }


    SelectShipStateProcessor::~SelectShipStateProcessor()
    {
    }

    GameState::State SelectShipStateProcessor::Update(float time)
    {
        _totalTime += time;
        SetLightAndCamera();
        return GameState::ShipSelect;
    }

    void SelectShipStateProcessor::OnMouseDown(int x, int y)
    {
        _document->OnMouseDown(x, y);
    }

    void SelectShipStateProcessor::OnMouseUp(int x, int y)
    {
        _document->OnMouseUp(x, y);
    }

    void SelectShipStateProcessor::OnMouseMove(int x, int y)
    {
        _document->OnMouseMove(x, y, 0);
    }

    void SelectShipStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            Game::Instance()->ChangeState(GameState::Level);
        }
    }


    void SelectShipStateProcessor::Hide()
    {
        _document->Hide();
        _sector->GetNode()->setVisible(false);
    }

    void SelectShipStateProcessor::Show()
    {
        _document->Show();
        UpdateModel();
        _sector->GetNode()->resetOrientation();
        _sector->GetNode()->setDirection(Ogre::Vector3(-0.3, 0.1, 0.4));
        _sector->GetNode()->setVisible(true);
    }

    void SelectShipStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
    {
        if (control->GetName() == "start")
        {
            SkinManager::Instance()->SetSkinID(_skinId);
            LevelManager::Instance()->SetStarted(false);
            Game::Instance()->ChangeState(GameState::Level);
        }
        if (control->GetName() == "left")
        {
            _skinId --;
            if (_skinId < 0)
                _skinId = SkinManager::Instance()->GetSkinCount() - 1;
            UpdateModel();
        }
        if (control->GetName() == "right")
        {
            _skinId ++;
            if (_skinId >= SkinManager::Instance()->GetSkinCount())
                _skinId = 0;
            UpdateModel();
        }
        if (control->GetName() == "back")
        {
            LevelManager::Instance()->SetStarted(false);
            if (LevelManager::Instance()->IsPuzzle())
            {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
            else {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
        }
    }

    bool SelectShipStateProcessor::IsOverlapping()
    {
        return Game::Instance()->GetState() != GameState::MainMenu;
    }

    void SelectShipStateProcessor::SetLightAndCamera()
    {
        auto camera = RenderProcessor::Instance()->GetCamera();
        Vector3 pos = Vector3(
                CAMERA_RADIUS*cos(0 * CAMERA_SPEED),
                1.5 * SIZE,
                CAMERA_RADIUS*sin(0 * CAMERA_SPEED));
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 1.5 * SIZE,0));

        auto light = RenderProcessor::Instance()->GetLight(0);
        pos.y += SIZE;
        light->SetPosition(pos);
    }

    void SelectShipStateProcessor::UpdateModel()
    {
        float scale = SkinManager::Instance()->GetShipScale(_skinId);
        _sector->GetNode()->setScale(scale, scale, scale);

        _currentShip = make_unique<ModelDrawable>(_sector, SkinManager::Instance()->GetShipModelName(_skinId));
        _currentShip->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 5);
    }


}