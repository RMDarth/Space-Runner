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

        ModelDrawable* _modelShip;
        ModelDrawable* _modelCruiser;
        SceneSector* _sectorShip;
        SceneSector* _sectorCruiser;

        SceneSector* _sector;
        ParticleSystem * _explosionEffect[4];

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
        void InitExplosions(Vector3 pos);
    };
}