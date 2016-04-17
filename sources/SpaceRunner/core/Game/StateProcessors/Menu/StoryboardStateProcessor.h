#pragma once

#include <Render/Drawables/ModelDrawable.h>
#include <Render/Drawables/RectDrawable.h>
#include <Render/ParticleSystem.h>
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

class SceneSector;

namespace CoreEngine
{
    class StoryboardStateProcessor : public IStateProcessor, public IEventHandler
    {
        ControlDocument * _document;
        int _changeState;
        float _totalTime;

        ModelDrawable* _modelShip[2];
        ModelDrawable* _modelCruiser;
        ModelDrawable* _modelTurret;
        ModelDrawable* _modelMine[2];
        RectDrawable* _blaster[8];

        RectDrawable* _circle;

        Ogre::Vector3 _shipPos[2];
        Ogre::Vector3 _minePos[2];
        Ogre::Vector3 _explosionPos[2];
        Ogre::Vector3 _blasterPos[8];

        SceneSector* _sectorShip[2];
        SceneSector* _sectorCruiser;
        SceneSector* _sectorTurret;
        SceneSector* _sectorMine[2];
        SceneSector* _sectorBlaster[8];
        SceneSector* _sectorCircle;

        SceneSector* _sector;
        ParticleSystem * _explosionEffect[2][4];
        Ogre::ParticleSystem * _engineEffect[3];

        Ogre::TexturePtr _rttTexture;
        Ogre::TexturePtr _spriteSheet;

        bool _renderSpritesheet;
        float _renderStarted;
        int _currentFrame;

    public:
        StoryboardStateProcessor();
        ~StoryboardStateProcessor();

        virtual GameState::State Update(float time);

        virtual void OnMouseDown(int x, int y);
        virtual void OnMouseUp(int x, int y);
        virtual void OnMouseMove(int x, int y);
        virtual void OnKeyPressed(OIS::KeyCode key);

        virtual void Hide();
        virtual void Show();
        virtual bool IsOverlapping() { return false; }

        // IEventProcessor interface //
        virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

    private:
        void SetLightAndCamera();
        void InitExplosions(Vector3 pos, int num, float time);
    };
}