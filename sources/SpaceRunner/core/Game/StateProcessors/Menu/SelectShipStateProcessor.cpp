#include <RenderProcessor.h>
#include <Game/SkinManager.h>
#include <Game/Scores.h>
#include <Game/Config.h>
#include <BillingProcessor.h>
#include "Render/SceneSector.h"
#include "Render/Drawables/ModelDrawable.h"
#include "SelectShipStateProcessor.h"
#include "Game/StateProcessors/Level/LevelManager.h"
#include "Game/Game.h"
using namespace std;

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

        _query = new ControlDocument("GUI/storequery.xml");
        _query->SetMouseUpHandler(this);
        _query->Hide();

        _loading = new ControlDocument("GUI/loading.xml");
        _loading->Hide();

        _document->RaisePriority(4);
        _query->RaisePriority(7);

        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = sceneManager->createSceneNode();
        sceneManager->getRootSceneNode()->addChild(sceneNode);
        _sector = new SceneSector(sceneNode);

        sceneNode->setPosition(Ogre::Vector3(0, 3.75f, 0));
        sceneNode->setDirection(Ogre::Vector3(-0.3f, 0.1f, 0.4f));

        _skinId = SkinManager::Instance()->GetSkinID();
        _sparksEffect = nullptr;
        _changeState = 0;

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

        if (_sparksEffect)
        {
            _sparksEffect->Update(time);
            if (_sparksEffect->IsFinished())
            {
                delete _sparksEffect;
                _sparksEffect = nullptr;
            }
        }

        if (_changeState == 1)
            _changeState = 2;
        else if (_changeState ==  2)
        {
            _changeState = 0;
            return GameState::Level;
        }

        return GameState::ShipSelect;
    }

    void SelectShipStateProcessor::OnMouseDown(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseDown(x, y);
        else
            _document->OnMouseDown(x, y);
    }

    void SelectShipStateProcessor::OnMouseUp(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseUp(x, y);
        else
            _document->OnMouseUp(x, y);
    }

    void SelectShipStateProcessor::OnMouseMove(int x, int y)
    {
        if (_queryVisible)
            _query->OnMouseMove(x, y, 0);
        else
            _document->OnMouseMove(x, y, 0);
    }

    void SelectShipStateProcessor::OnKeyPressed(OIS::KeyCode key)
    {
        if (key == OIS::KC_ESCAPE)
        {
            if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
            {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
            else {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
        }
    }


    void SelectShipStateProcessor::Hide()
    {
        _document->Hide();
        _query->Hide();
        _loading->Hide();
        _sector->GetNode()->setVisible(false);
    }

    void SelectShipStateProcessor::Show()
    {
        _document->Show();
        _query->Hide();
        _loading->Hide();
        _queryVisible = false;
        _changeState = 0;
        _skinId = SkinManager::Instance()->GetSkinID();
        UpdateModel();
        UpdateHUD();
        _sector->GetNode()->resetOrientation();
        _sector->GetNode()->setDirection(Ogre::Vector3(-0.3f, 0.1f, 0.4f));
        _sector->GetNode()->setVisible(true);
    }

    void SelectShipStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType /*type*/, int /*x*/, int /*y*/)
    {
        if (control->GetName() == "start")
        {
            // Update price
            bool bought = Config::Instance()->IsModelBought(_skinId);
            if (bought)
            {
                SkinManager::Instance()->SetSkinID(_skinId);
                Config::Instance()->SetSelectedModel(_skinId);
                LevelManager::Instance()->SetStarted(false);
                LevelManager::Instance()->SetLives(SkinManager::Instance()->LivesCount());
                LevelManager::Instance()->ResetMovies();

                _document->Hide();
                _sector->GetNode()->setVisible(false);
                _loading->Show();
                _changeState = 1;

                //Game::Instance()->ChangeState(GameState::Level);
            } else {
                auto price = SkinManager::Instance()->GetPrice(_skinId);
                auto bank = Scores::Instance()->GetTotalEnergy();
                if (bank < price)
                {
                    _query->Show();
                    _currentShip->SetRenderingQueue(Ogre::RENDER_QUEUE_MAIN);
                    _queryVisible = true;
                }
                else
                {
                    Scores::Instance()->UpdateTotalEnergy(-price);
                    InitSparks();
                    Config::Instance()->SetModelBought(_skinId);
                    UpdateHUD();

                    UpdateAchievements();
                }
            }
        }
        if (control->GetName() == "return")
        {
            _query->Hide();
            _currentShip->SetRenderingQueue(-Ogre::RENDER_QUEUE_MAIN + Ogre::RENDER_QUEUE_OVERLAY + 5);
            _queryVisible = false;
        }
        if (control->GetName() == "left")
        {
            _skinId --;
            if (_skinId < 0)
                _skinId = SkinManager::Instance()->GetSkinCount() - 1;
            UpdateModel();
            UpdateHUD();
        }
        if (control->GetName() == "right")
        {
            _skinId ++;
            if (_skinId >= SkinManager::Instance()->GetSkinCount())
                _skinId = 0;
            UpdateModel();
            UpdateHUD();
        }

        if (control->GetName() == "store")
        {
            Game::Instance()->ChangeState(GameState::Store);
        }
        if (control->GetName() == "back")
        {
            LevelManager::Instance()->SetStarted(false);
            if (LevelManager::Instance()->GetLevelType() == LevelType::Rush)
            {
                Game::Instance()->ChangeState(GameState::MainMenu);
            }
            else {
                Game::Instance()->ChangeState(GameState::Storyboard);
            }
        }
    }

    bool SelectShipStateProcessor::IsOverlapping()
    {
        return Game::Instance()->GetState() == GameState::Storyboard;
    }

    void SelectShipStateProcessor::SetLightAndCamera()
    {
        auto camera = RenderProcessor::Instance()->GetCamera();
        Vector3 pos = Vector3(
                CAMERA_RADIUS*cosf(0 * CAMERA_SPEED),
                1.5f * SIZE,
                CAMERA_RADIUS*sinf(0 * CAMERA_SPEED));
        camera->SetPosition(pos);
        camera->SetTarget(Vector3(0, 1.5f * SIZE,0));

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

    void SelectShipStateProcessor::UpdateHUD()
    {
        _document->GetControlByName("bank")->SetText(to_string(Scores::Instance()->GetTotalEnergy()));

        // update features;
        auto features = SkinManager::Instance()->GetFeaturesList(_skinId);
        std::shared_ptr<Control> featuresControl[4] =
                {
                        _document->GetControlByName("features1"),
                        _document->GetControlByName("features2"),
                        _document->GetControlByName("features3"),
                        _document->GetControlByName("features4")
                };
        if (features.empty())
        {
            for (auto i = 0; i < 4; i++)
                featuresControl[i]->SetVisible(false);
        } else {
            featuresControl[0]->SetVisible(true);
            for (auto i = 0; i < features.size(); i++)
            {
                featuresControl[i+1]->SetVisible(true);
                featuresControl[i+1]->SetText(features[i]);
            }
            for (auto i = features.size() + 1; i < 4; i++)
                featuresControl[i]->SetVisible(false);
        }

        // Update price
        bool bought = Config::Instance()->IsModelBought(_skinId);
        if (!bought)
        {
            _document->GetControlByName("costlogo")->SetVisible(true);
            auto costControl = _document->GetControlByName("cost");
            costControl->SetVisible(true);
            costControl->SetText(to_string(SkinManager::Instance()->GetPrice(_skinId)));
        } else {
            _document->GetControlByName("costlogo")->SetVisible(false);
            _document->GetControlByName("cost")->SetVisible(false);
        }

        // Update button
        if (bought)
        {
            _document->GetControlByName("start")->SetText("Start");
        } else {
            _document->GetControlByName("start")->SetText("Buy");
        }

    }

    void SelectShipStateProcessor::InitSparks()
    {
        auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
        auto sceneNode = _sector->GetNode();

        auto sceneNodeChild = sceneManager->createSceneNode();
        sceneNodeChild->setDirection(0, 1, 0);
        sceneNodeChild->setInheritScale(false);
        sceneNodeChild->setScale(1, 1, 1);
        sceneNode->addChild(sceneNodeChild);
        _sparksEffect = new ParticleSystem(sceneNodeChild, "BuySparks_%d", "Blast4", 2, 15, true);
    }

    void SelectShipStateProcessor::UpdateAchievements()
    {
        if (!Config::Instance()->IsAchievementCompleted(13))
        {
            int shipsBought = 0;
            for (auto i = 0; i < 5; i++)
            {
                if (Config::Instance()->IsModelBought(i))
                    shipsBought++;
            }
            if (shipsBought > 1)
            {
                Config::Instance()->SetAchievementCompleted(10);
            }
            if (shipsBought >= 5)
            {
                Config::Instance()->SetAchievementCompleted(13);
            }

            Config::Instance()->SetAchievementData(13, shipsBought);
        }

        BillingProcessor::Instance()->SyncAchievements();
    }


}