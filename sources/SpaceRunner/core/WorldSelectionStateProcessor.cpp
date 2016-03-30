#include "WorldSelectionStateProcessor.h"
#include "SceneSector.h"
#include "ModelDrawable.h"
#include "LevelManager.h"
#include "BillingProcessor.h"
#include "Scores.h"
#include "Game.h"

#include "OgreRectangle2D.h"

#include <sstream>
using namespace std;

#define SPEED 1.5f
#define WORLD_SIZE 0.5
#define LEVEL4_UNLOCKSTARS 175
#define LEVEL5_UNLOCKSTARS 235

namespace CoreEngine
{
	WorldSelectionStateProcessor::WorldSelectionStateProcessor()
		: SliderProcessor(WORLD_NUM, SPEED, WORLD_SIZE)
	{
		_totalTime = 0;
		_bought = false;
		
		CreateMaterials();
		CreateWorlds();
		SliderProcessor::Init();

		_document = new ControlDocument("GUI/worldselect.xml");
		_document->SetMouseUpHandler(this);
		_document->Hide();
	}


	WorldSelectionStateProcessor::~WorldSelectionStateProcessor()
	{
		for (int i = 0; i < WORLD_NUM; i++)
		{
			delete rects[i];
			delete lockRects[i];
			delete sectors[i];
		}
		delete[] rects;
		delete[] lockRects;
		delete[] sectors;
	}

	void WorldSelectionStateProcessor::CreateMaterials()
	{
		stringstream stream;
		for (int i = 0; i < WORLD_NUM; i++)
		{
			stream.str(string());
			stream << "World_" << (i+1);

			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(stream.str(), "General");
			stream << ".png";
			auto state = material->getTechnique(0)->getPass(0)->createTextureUnitState(stream.str());
			state->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
			
			material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
			material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
			material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
			material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
		}

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Lock", "General");
		auto state = material->getTechnique(0)->getPass(0)->createTextureUnitState("lock.png");
		state->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
		material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
		material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);

	}

	void WorldSelectionStateProcessor::CreateWorlds()
	{
		rects = new Ogre::Rectangle2D*[20];
		lockRects = new Ogre::Rectangle2D*[20];
		sectors = new SceneSector*[20];

		stringstream stream;
		int i;
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();

		int left, top, width, height;
		RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);

		for (i = 0; i < WORLD_NUM; i++)
		{
			stream.str(string());
			stream << "World_" << (i+1);

			rects[i] = new Ogre::Rectangle2D(true);
			rects[i]->setMaterial(stream.str());
			rects[i]->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 2);

			lockRects[i] = new Ogre::Rectangle2D(true);
			lockRects[i]->setMaterial("Lock");
			lockRects[i]->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);

			if (i < 3)
				lockRects[i]->setVisible(false);
			else
				_unlocked[i - 3] = false;

			auto sceneNode = sceneManager->createSceneNode();
			sceneNode->attachObject(lockRects[i]);
			sceneNode->attachObject(rects[i]);

			sceneManager->getRootSceneNode()->addChild(sceneNode);
			sectors[i] = new SceneSector(sceneNode);
		}
	}

	void WorldSelectionStateProcessor::SetObjectPosition(int object, float left, float top, float right, float bottom)
	{
		int wleft, wtop, width, height;
		RenderProcessor::Instance()->GetViewport()->getActualDimensions(wleft, wtop, width, height);
		float size = ((right + 1) * width - (left + 1) * width) * 0.5f;
		float delta = (height - size) * 0.5f / height;
		rects[object]->setCorners(left, -1 + delta * 2 + size * 2 / height, right, -1 + delta * 2);

		if (object >= 3 && !_bought && !_unlocked[object - 3])
			lockRects[object]->setCorners(left + 0.4f * height / width, -1 + delta * 2 + size * 2 / height - 0.4f, right - 0.4f * height / width, -1 + delta * 2 + 0.4f);
		else
			lockRects[object]->setVisible(false);
	}

	void WorldSelectionStateProcessor::SetCurrentObject(int object)
	{
		if (object >= 3 && !_unlocked[object - 3])
		{
			if (!_bought)
			{
				BillingProcessor::Instance()->BuyItem(LEVELS_ITEM_ID);
				return;
			}
		}

		LevelManager::Instance()->SetWorld(object);
		Game::Instance()->ChangeState(GameState::LevelSelect);
	}

	GameState::State WorldSelectionStateProcessor::Update(float time)
	{
		_totalTime += time;

		SliderProcessor::UpdateSlide(time);

		_bought = BillingProcessor::Instance()->IsItemBought(LEVELS_ITEM_ID);
		if (_bought)
		{
			for_each(lockRects, lockRects + WORLD_NUM, std::bind(&Ogre::Rectangle2D::setVisible, std::placeholders::_1, false));
		}
		if (_unlocked[0])
		{
			lockRects[3]->setVisible(false);
		}
		if (_unlocked[1])
		{
			lockRects[4]->setVisible(false);
		}

		SetLightAndCamera();

		UpdateUnlockText();

		return GameState::WorldSelect;
	}

	void WorldSelectionStateProcessor::OnMouseDown(int x, int y)
	{
		if (_document->OnMouseDown(x, y))
			return;

		SliderProcessor::OnMouseDown(x, y);
	}

	void WorldSelectionStateProcessor::OnMouseUp(int x, int y)
	{
		if (_document->OnMouseUp(x, y))
            return;

		SliderProcessor::OnMouseUp(x, y);
	}

	void WorldSelectionStateProcessor::OnMouseMove(int x, int y)
	{
		_document->OnMouseMove(x, y, 0);

		SliderProcessor::OnMouseMove(x, y);
	}

	void WorldSelectionStateProcessor::OnKeyPressed(OIS::KeyCode key)
	{
		if (key == OIS::KC_ESCAPE)
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}

	void WorldSelectionStateProcessor::SetLightAndCamera()
	{
		auto camera = RenderProcessor::Instance()->GetCamera();
		Vector3 pos = Vector3(0, 0,	10);
		camera->SetPosition(pos);
		camera->SetTarget(Vector3(0, 0, 0));

		auto light = RenderProcessor::Instance()->GetLight(0);
		light->SetPosition(pos);
	}

	void WorldSelectionStateProcessor::Hide()
	{
		for (int i = 0; i < WORLD_NUM; i++)
		{
			sectors[i]->GetNode()->setVisible(false);
		}

		_document->Hide();
	}

	void WorldSelectionStateProcessor::Show()
	{
		for (int i = 0; i < WORLD_NUM; i++)
		{
			sectors[i]->GetNode()->setVisible(true);
		}

		_stars = -1;
		_prevObject = -1;
		_document->Show();
		UpdateUnlockText();
	}

	void WorldSelectionStateProcessor::ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y)
	{
		if (control->GetName() == "back")
		{
			Game::Instance()->ChangeState(GameState::MainMenu);
		}
	}

	void WorldSelectionStateProcessor::UpdateUnlockText()
	{
		if (_stars < 0)
		{
			_stars = 0;
			for (int i = 0; i < LEVELNUM; i++)
			{
				_stars += Scores::Instance()->GetBestStars(i + 1);
			}
		}

		//if (_currentObject != _prevObject)
		{
			static auto unlockStarsControl = _document->GetControlByName("unlockstars");
			static auto starControl = _document->GetControlByName("star");
			static auto unlockMoneyControl = _document->GetControlByName("unlockmoney");

			_prevObject = _currentObject;
			if (!_bought &&
				((_currentObject == 3 && _stars < LEVEL4_UNLOCKSTARS) || (_currentObject == 4 && _stars < LEVEL5_UNLOCKSTARS)))
			{
				unlockStarsControl->SetVisible(true);
				starControl->SetVisible(true);
				unlockMoneyControl->SetVisible(true);

				stringstream stream;
				if (_currentObject == 3)
					stream << _stars << "/" << LEVEL4_UNLOCKSTARS;
				else
					stream << _stars << "/" << LEVEL5_UNLOCKSTARS;

				unlockStarsControl->SetText(stream.str());
			}
			else {
				if (!_bought && _currentObject >= 3)
				{
					_unlocked[_currentObject - 3] = true;
				}

				unlockStarsControl->SetVisible(false);
				starControl->SetVisible(false);
				unlockMoneyControl->SetVisible(false);
			}
		}
	}

}
