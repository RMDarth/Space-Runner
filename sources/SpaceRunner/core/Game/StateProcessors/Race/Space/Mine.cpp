#include "Mine.h"
#include "Render/Drawables/ModelDrawable.h"
#include "Render/ParticleSystem.h"
using namespace std;

namespace CoreEngine
{
	Mine::Mine(Vector3 offset, float speed)
		: SpaceObject(offset, speed)
	{
		auto sceneManager = RenderProcessor::Instance()->GetSceneManager();
		auto sceneNode = sceneManager->createSceneNode();
		sceneManager->getRootSceneNode()->addChild(sceneNode);
		_sector = new SceneSector(sceneNode);
		sceneNode->setPosition(VectorToOgre(offset));

		//_sector->GetNode()->setDirection(Ogre::Vector3(0, 1, 0));
		_model = new ModelDrawable(_sector, "Mine.mesh");
		_model->SetScale(100.0f);

        Ogre::Vector3 posList[] = {
                {0.012, 0.0088f, 0},
                {-0.0096, 0.0064f, -0.0096},
                {-0.0096, 0.0064f, 0.0096},
                {-0.0096, -0.007f, 0.0096},
                {-0.0096, -0.007f, -0.0096},
                {0.012, -0.0082f, 0}
        };
        for (auto i = 0; i < 6; i++)
        {
            auto sceneNodeChild = sceneManager->createSceneNode();
            sceneNodeChild->setDirection(0, 1, 0);

            sceneNodeChild->setInheritScale(false);
            sceneNodeChild->setScale(0.2f, 0.2f, 0.2f);
            sceneNodeChild->setPosition(posList[i]);
            sceneNode->addChild(sceneNodeChild);

            _glowEffects[i] = new ParticleSystem(sceneNodeChild, "GlowMine" + to_string(i) + "_%d", "MineGlowEffect", 200, 0, true);
        }
	}

	Mine::~Mine()
	{
		delete _model;
        for (auto i = 0; i < 6; i++)
            delete _glowEffects[i];
		delete _sector;
	}

	void Mine::Update(float time, float roadSpeed)
	{
		SpaceObject::Update(time, roadSpeed);
		_sector->GetNode()->setPosition(VectorToOgre(_pos));
	}

	void Mine::SetVisible(bool visible)
	{
		_sector->GetNode()->setVisible(visible);
	}
}