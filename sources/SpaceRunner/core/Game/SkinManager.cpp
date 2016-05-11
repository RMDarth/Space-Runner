#include "SkinManager.h"

namespace CoreEngine
{
    SkinManager* SkinManager::_instance = nullptr;

    SkinManager::SkinManager()
    {
        _skinID = 0;

        _shipModelNames.push_back("ship.mesh");
        _shipModelNames.push_back("ship2.mesh");
        _shipScales.push_back(5.0f);
        _shipScales.push_back(0.5f);

        OffsetList ship1Engines;
        ship1Engines.push_back(Vector3(0.25f, 0.03f, 0.12f));
        ship1Engines.push_back(Vector3(0.25f, 0.03f, -0.12f));
        _engineOffsets.push_back(ship1Engines);

        OffsetList ship2Engines;
        ship2Engines.push_back(Vector3(0.28f * 10, -0.02f * 10, 0.0f * 10));
        _engineOffsets.push_back(ship2Engines);

        _shieldOffsets.push_back(Vector3(0.3f, 0.0f, 0.0f));
        _shieldOffsets.push_back(Vector3(4.2f, 0.6f, 0.0f));

        Features features1;
        _featuresList.push_back(features1);
        Features features2 { "Faster shooting" };
        _featuresList.push_back(features2);

        _priceList.push_back(0);
        _priceList.push_back(2500);

        _livesList.push_back(1);
        _livesList.push_back(1);

        _shootingSpeedList.push_back(1.5f);
        _shootingSpeedList.push_back(1.0f);

        _shieldQueue.push_back(false);
        _shieldQueue.push_back(false);
    }

    SkinManager* SkinManager::Instance()
    {
        if (_instance == NULL)
        {
            _instance = new SkinManager();
        }
        return _instance;
    }

    void SkinManager::SetSkinID(int skinID)
    {
        _skinID = skinID;
    }

    int SkinManager::GetSkinID()
    {
        return _skinID;
    }

    int SkinManager::GetSkinCount()
    {
        return SKIN_COUNT;
    }

    std::string SkinManager::GetShipModelName()
    {
        return _shipModelNames[_skinID];
    }

    std::string SkinManager::GetShipModelName(int id)
    {
        return _shipModelNames[id];
    }

    float SkinManager::GetShipScale()
    {
        return _shipScales[_skinID];
    }

    float SkinManager::GetShipScale(int id)
    {
        return _shipScales[id];
    }

    int SkinManager::GetEngineCount()
    {
        return (int)_engineOffsets[_skinID].size();
    }

    int SkinManager::GetEngineCount(int id)
    {
        return (int)_engineOffsets[id].size();
    }

    Vector3 SkinManager::GetEngineOffset(int num)
    {
        return _engineOffsets[_skinID][num];
    }

    Vector3 SkinManager::GetEngineOffset(int num, int id)
    {
        return _engineOffsets[id][num];
    }

    Vector3 SkinManager::GetShieldOffset()
    {
        return _shieldOffsets[_skinID];
    }

    Vector3 SkinManager::GetShieldOffset(int id)
    {
        return _shieldOffsets[id];
    }

    const std::vector<std::string> &SkinManager::GetFeaturesList()
    {
        return _featuresList[_skinID];
    }

    const std::vector<std::string> &SkinManager::GetFeaturesList(int id)
    {
        return _featuresList[id];
    }

    int SkinManager::GetPrice()
    {
        return _priceList[_skinID];
    }

    int SkinManager::GetPrice(int id)
    {
        return _priceList[id];
    }

    int SkinManager::LivesCount()
    {
        return _livesList[_skinID];
    }

    int SkinManager::LivesCount(int id)
    {
        return _livesList[id];
    }

    float SkinManager::ShootingCooldown()
    {
        return _shootingSpeedList[_skinID];
    }

    float SkinManager::ShootingCooldown(int id)
    {
        return _shootingSpeedList[id];
    }

    bool SkinManager::ShieldQueue()
    {
        return _shieldQueue[_skinID];
    }

    bool SkinManager::ShieldQueue(int id)
    {
        return _shieldQueue[id];
    }


}

