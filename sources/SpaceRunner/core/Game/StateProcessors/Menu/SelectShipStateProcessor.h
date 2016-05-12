#pragma once

#include <Render/ParticleSystem.h>
#include "Game/GameDefs.h"
#include "Game/StateProcessors/IStateProcessor.h"
#include "ControlLibrary/ControlDocument.h"

class ModelDrawable;
class SceneSector;

namespace CoreEngine
{
    class SelectShipStateProcessor : public IStateProcessor, public IEventHandler
    {
        ControlDocument * _document;
        ControlDocument * _query;
        bool _queryVisible;

        SceneSector* _sector;
        UPtr<ModelDrawable> _currentShip;
        ParticleSystem *_sparksEffect;

        float _totalTime;

        int _skinId;

    public:
        SelectShipStateProcessor();
        ~SelectShipStateProcessor();

        virtual GameState::State Update(float time);
        virtual void OnMouseDown(int x, int y);
        virtual void OnMouseUp(int x, int y);
        virtual void OnMouseMove(int x, int y);
        virtual void OnKeyPressed(OIS::KeyCode key);

        virtual void Hide();
        virtual void Show();
        virtual bool IsOverlapping();

        // IEventProcessor interface //
        virtual void ProcessEvent(Control* control, IEventHandler::EventType type, int x, int y);

    private:
        void SetLightAndCamera();
        void UpdateModel();
        void UpdateHUD();
        void InitSparks();
        void UpdateAchievements();
    };
}

