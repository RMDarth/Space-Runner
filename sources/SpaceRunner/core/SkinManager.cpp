#include "SkinManager.h"

namespace CoreEngine
{
	SkinManager* SkinManager::_instance = nullptr;

	SkinManager::SkinManager()
	{
		_skinID = 0;
		_quality = 0;

		_colorModelNames.push_back("lod0-ball.mesh");
		_colorModelNames.push_back("lo-classic.mesh");
		_colorModelNames.push_back("lo-easter.mesh");
		_colorModelNames.push_back("lo-fruit_ball.mesh");
		_colorModelNames.push_back("lo-halloween.mesh");

		_basisModelNames.push_back("lo-fly.mesh");
		_basisModelNames.push_back("lo-fly.mesh");
		_basisModelNames.push_back("lo-fly.mesh");
		_basisModelNames.push_back("lo-fly.mesh");
		_basisModelNames.push_back("lo-fly.mesh");

		_steelModelNames.push_back("lo-steel.mesh");
		_steelModelNames.push_back("lo-classic.mesh");
		_steelModelNames.push_back("lo-easter.mesh");
		_steelModelNames.push_back("lo-fruit_bomb.mesh");
		_steelModelNames.push_back("lo-halloween.mesh");

		_bombModelNames.push_back("lo-bomb.mesh");
		_bombModelNames.push_back("lo-classic.mesh");
		_bombModelNames.push_back("lo-easter.mesh");
		_bombModelNames.push_back("lo-fruit_bomb.mesh");
		_bombModelNames.push_back("lo-halloween.mesh");

		_materialPrefixes.push_back("ball");
		_materialPrefixes.push_back("classicball");
		_materialPrefixes.push_back("easterball");
		_materialPrefixes.push_back("fruitball");
		_materialPrefixes.push_back("halloweenball");

		_connectorUsageList.push_back(1);
		_connectorUsageList.push_back(1);
		_connectorUsageList.push_back(0);
		_connectorUsageList.push_back(0);
		_connectorUsageList.push_back(0);
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

	std::string SkinManager::GetColorModelName()
	{
		return _colorModelNames[_skinID];
	}

	std::string SkinManager::GetColorModelName(int id)
	{
		return _colorModelNames[id];
	}

	std::string SkinManager::GetBasisModelName()
	{
		return _basisModelNames[_skinID];
	}

	std::string SkinManager::GetBasisModelName(int id)
	{
		return _basisModelNames[id];
	}

	std::string SkinManager::GetBombModelName()
	{
		return _bombModelNames[_skinID];
	}

	std::string SkinManager::GetBombModelName(int id)
	{
		return _bombModelNames[id];
	}

	std::string SkinManager::GetSteelModelName()
	{
		return _steelModelNames[_skinID];
	}

	std::string SkinManager::GetSteelModelName(int id)
	{
		return _steelModelNames[id];
	}

	bool SkinManager::GetConnectorsUsage()
	{
		return _connectorUsageList[_skinID] != 0;
	}

	bool SkinManager::GetConnectorsUsage(int id)
	{
		return _connectorUsageList[id] != 0;
	}


	std::string SkinManager::GetMaterialPrefix()
	{
		return _quality == 0 ? _materialPrefixes[_skinID] : _materialPrefixes[_skinID] + "simple";
	}

	std::string SkinManager::GetMaterialPrefix(int id, int quality)
	{
		return quality == 0 ? _materialPrefixes[id] : _materialPrefixes[id] + "simple";
	}

	int SkinManager::GetQuality()
	{
		return _quality;
	}
	void SkinManager::SetQuality(int quality)
	{
		_quality = quality;
	}
}

