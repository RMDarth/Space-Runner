#include <vector>
#include <string>
#include <Basic.h>

#define SKIN_COUNT 2

namespace CoreEngine
{
    class SkinManager
    {
        using OffsetList = std::vector<Vector3>;

        static SkinManager* _instance;
        int _skinID;

        std::vector<std::string> _shipModelNames;
        std::vector<float> _shipScales;
        std::vector<OffsetList> _engineOffsets;
        OffsetList _shieldOffsets;

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

        Vector3 GetEngineOffset(int num);
        Vector3 GetEngineOffset(int num, int id);

        Vector3 GetShieldOffset();
        Vector3 GetShieldOffset(int id);
    };
}