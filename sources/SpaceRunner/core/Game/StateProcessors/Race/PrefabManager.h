#pragma once
#include "Prefab.h"
#include <string>
#include <memory>
#include <map>
#include <Game/GameDefs.h>

namespace CoreEngine
{
    using PrefabList = PtrList<Prefab>;
    using PrefabMap = std::map<std::string, PrefabList>;

    class PrefabManager
    {
        static PrefabManager *_instance;
        PrefabMap _prefabMap;

    public:
        void LoadPrefabs();

        static PrefabManager *Instance();
        const PrefabList& getPrefabList(std::string category);

    private:
        PrefabManager();

    };
}

