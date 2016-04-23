#include <sstream>
#include <FileSystem.h>
#include <Basic.h>
#include "LevelFileManager.h"
#include "PrefabManager.h"

namespace CoreEngine
{
    LevelFileManager* LevelFileManager::_instance = nullptr;

    LevelFileManager *LevelFileManager::Instance()
    {
        if (_instance == nullptr)
        {
            _instance = new LevelFileManager();
        }
        return _instance;
    }

    LevelFileManager::LevelFileManager()
    {

    }

    void LevelFileManager::SaveLevel(Level *level, std::string filename)
    {
        FileOutputRef file(filename);

        file.WriteInt((int)level->difficulty);
        file.WriteInt(level->energyToComplete);
        file.WriteInt(level->skyboxId);

        file.WriteInt((int)level->obstacleList.size());
        for (auto i = 0; i < level->obstacleList.size(); i++)
        {
            file.WriteInt((int)level->obstacleList[i]);
        }

        file.WriteInt((int)level->prefabList.size());
        for (auto i = 0; i < level->prefabList.size(); i++)
        {
            file.WriteString(level->prefabList[i].category);
            file.WriteInt(level->prefabList[i].prefabNum);
        }

        file.Close();
    }

    void LevelFileManager::SaveLevel(Level *level, int num)
    {
        std::stringstream stream;
        stream << "Levels/level_" << num << ".lvl";

        SaveLevel(level, stream.str());
    }

    Level *LevelFileManager::LoadLevel(int level)
    {
        std::stringstream stream;
        stream << "Levels/level_" << level << ".lvl";

        return LoadLevel(stream.str());
    }

    Level *LevelFileManager::LoadLevel(std::string filename)
    {
        FileInputRef file(filename);

        Level * structure = new Level();

        structure->difficulty = (LevelDifficulty)file.ReadInt();
        structure->energyToComplete = file.ReadInt();
        structure->skyboxId = file.ReadInt();
        int obstacleNum = file.ReadInt();
        for (int i = 0; i < obstacleNum; i++)
        {
            ObstacleType type = (ObstacleType)file.ReadInt();
            structure->obstacleList.push_back(type);
        }

        int prefabNum = file.ReadInt();
        for (auto i = 0; i < prefabNum; i++)
        {
            std::string prefabCategory = file.ReadString();
            int prefabNum = file.ReadInt();
            PrefabInfo info { prefabCategory, prefabNum };
            structure->prefabList.push_back(info);
        }

        structure->currentPrefab = 0;
        structure->currentObstacle = -1;

        file.Close();
        return structure;
    }



}











