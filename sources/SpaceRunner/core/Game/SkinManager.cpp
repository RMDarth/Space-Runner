#include "SkinManager.h"

namespace CoreEngine
{
    SkinManager* SkinManager::_instance = nullptr;

    SkinManager::SkinManager()
    {
        _skinID = 0;

        _shipModelNames.push_back("ship.mesh");
        _shipModelNames.push_back("ship2.mesh");
        _shipModelNames.push_back("ship3/ship3.mesh");
        _shipModelNames.push_back("ship4/ship4.mesh");
        _shipModelNames.push_back("ship5/ship5.mesh");

        _shipScales.push_back(5.0f);
        _shipScales.push_back(0.5f);
        _shipScales.push_back(0.4f);
        _shipScales.push_back(0.003f);
        _shipScales.push_back(0.0045f);

        OffsetList ship1Engines;
        ship1Engines.push_back(Vector3(0.25f, 0.03f, 0.12f));
        ship1Engines.push_back(Vector3(0.25f, 0.03f, -0.12f));
        _engineOffsets.push_back(ship1Engines);

        OffsetList ship2Engines;
        ship2Engines.push_back(Vector3(0.28f * 10, -0.02f * 10, 0.0f * 10));
        _engineOffsets.push_back(ship2Engines);

        OffsetList ship3Engines;
        ship3Engines.push_back(Vector3(0.38f * 12.5f, 0.01f * 12.5f, 0.03f * 12.5f));
        ship3Engines.push_back(Vector3(0.38f * 12.5f, 0.01f * 12.5f, -0.035f * 12.5f));
        _engineOffsets.push_back(ship3Engines);

        OffsetList ship4Engines;
        ship4Engines.push_back(Vector3(0.37f * 1666, 0.105f * 1666, 0.11f * 1666));
        ship4Engines.push_back(Vector3(0.37f * 1666, 0.105f * 1666, -0.115f * 1666));
        _engineOffsets.push_back(ship4Engines);

        OffsetList ship5Engines;
        ship5Engines.push_back(Vector3(0.37f * 1111, 0.034f * 1111, -0.005f * 1111));
        _engineOffsets.push_back(ship5Engines);

        _shieldOffsets.push_back(Vector3(0.3f, 0.0f, 0.0f));
        _shieldOffsets.push_back(Vector3(4.2f, 0.6f, 0.0f));
        _shieldOffsets.push_back(Vector3(4.2f, 0.6f, 0.0f));
        _shieldOffsets.push_back(Vector3(700, 100.6f, 0.0f));
        _shieldOffsets.push_back(Vector3(500, 100.6f, 0.0f));

        Features features1;
        _featuresList.push_back(features1);
        Features features2 { "Fast shooting" };
        _featuresList.push_back(features2);
        Features features3 { "Fast shooting", "Shield queue" };
        _featuresList.push_back(features3);
        Features features4 { "Fast shooting, shield queue", "Additional life" };
        _featuresList.push_back(features4);
        Features features5 { "Fast shooting, shield queue", "Additional life", "Shield block mines" };
        _featuresList.push_back(features5);

        _priceList.push_back(0);
        _priceList.push_back(2500);
        _priceList.push_back(10000);
        _priceList.push_back(50000);
        _priceList.push_back(100000);

        _livesList.push_back(1);
        _livesList.push_back(1);
        _livesList.push_back(1);
        _livesList.push_back(2);
        _livesList.push_back(2);

        _shootingSpeedList.push_back(1.5f);
        _shootingSpeedList.push_back(1.0f);
        _shootingSpeedList.push_back(0.9f);
        _shootingSpeedList.push_back(0.9f);
        _shootingSpeedList.push_back(0.9f);

        _shieldQueue.push_back(false);
        _shieldQueue.push_back(false);
        _shieldQueue.push_back(true);
        _shieldQueue.push_back(true);
        _shieldQueue.push_back(true);
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

