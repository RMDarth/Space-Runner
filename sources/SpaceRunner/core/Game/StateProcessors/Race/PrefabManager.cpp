#include "FileSystem.h"
#include "PrefabManager.h"

namespace CoreEngine
{
    PrefabManager* PrefabManager::_instance = nullptr;

    PrefabManager::PrefabManager()
    {
    }

    void PrefabManager::LoadPrefabs()
    {
        FileInputRef file("Prefabs/info.dat");
        if (!file.IsOpened())
            return;

        int categoryCount = file.ReadInt();
        file.ReadSubstring();
        for (auto category = 0; category < categoryCount; category++)
        {
            auto categoryName = file.ReadSubstring(' ');
            auto categoryPrefabsCount = file.ReadInt();
            file.ReadSubstring();

            for (auto prefabId = 0; prefabId < categoryPrefabsCount; prefabId++)
            {
                std::stringstream filePath;
                filePath << "Prefabs/" << categoryName << "/" << (prefabId+1) << ".pf";

                _prefabMap[categoryName].push_back(make_shared<Prefab>(filePath.str()));
            }
        }
        file.Close();
    }

    PrefabManager *PrefabManager::Instance()
    {
        if (_instance == nullptr)
        {
            _instance = new PrefabManager();
        }
        return _instance;
    }

    const PrefabList &PrefabManager::getPrefabList(std::string category)
    {
        return _prefabMap[category];
    }
}

