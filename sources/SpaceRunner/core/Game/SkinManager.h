#include <vector>
#include <string>
#include <Basic.h>

#define SKIN_COUNT 2

namespace CoreEngine
{
    class SkinManager
    {
        using OffsetList = std::vector<Vector3>;
        using Features = std::vector<std::string>;

        static SkinManager* _instance;
        int _skinID;

        std::vector<std::string> _shipModelNames;
        std::vector<float> _shipScales;
        std::vector<OffsetList> _engineOffsets;
        OffsetList _shieldOffsets;

        std::vector<Features> _featuresList;
        std::vector<int> _priceList;
        std::vector<int> _livesList;
        std::vector<float> _shootingSpeedList;
        std::vector<bool> _shieldQueue;

        SkinManager();
    public:

        static SkinManager* Instance();

        void SetSkinID(int skinID);
        int GetSkinID();

        int GetSkinCount();

        std::string GetShipModelName();
        std::string GetShipModelName(int id);

        float GetShipScale();
        float GetShipScale(int id);

        int GetEngineCount();
        int GetEngineCount(int id);

        int LivesCount();
        int LivesCount(int id);

        float ShootingCooldown();
        float ShootingCooldown(int id);

        bool ShieldQueue();
        bool ShieldQueue(int id);

        Vector3 GetEngineOffset(int num);
        Vector3 GetEngineOffset(int num, int id);

        Vector3 GetShieldOffset();
        Vector3 GetShieldOffset(int id);

        const std::vector<std::string>& GetFeaturesList();
        const std::vector<std::string>& GetFeaturesList(int id);

        int GetPrice();
        int GetPrice(int id);
    };
}