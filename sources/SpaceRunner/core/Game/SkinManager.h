#include <vector>
#include <string>

#define SKIN_COUNT 5

namespace CoreEngine
{
	class SkinManager
	{
		static SkinManager* _instance;
		int _skinID;
		int _quality;

		std::vector<std::string> _colorModelNames;
		std::vector<std::string> _basisModelNames;
		std::vector<std::string> _bombModelNames;
		std::vector<std::string> _steelModelNames;
		std::vector<std::string> _materialPrefixes;
		std::vector<int> _connectorUsageList;

		SkinManager();
	public:
		
		static SkinManager* Instance();

		void SetSkinID(int skinID);
		int GetSkinID();

		int GetQuality();
		void SetQuality(int quality);

		int GetSkinCount();

		std::string GetColorModelName();
		std::string GetColorModelName(int id);
		std::string GetBasisModelName();
		std::string GetBasisModelName(int id);
		std::string GetBombModelName();
		std::string GetBombModelName(int id);
		std::string GetSteelModelName();
		std::string GetSteelModelName(int id);
		bool GetConnectorsUsage();
		bool GetConnectorsUsage(int id);
		
		std::string GetMaterialPrefix();
		std::string GetMaterialPrefix(int id, int quality);
	};
}