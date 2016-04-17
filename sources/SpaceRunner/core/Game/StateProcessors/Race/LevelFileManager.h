#pragma once
#include "LevelStructure.h"
#include <vector>
#include <string>

namespace CoreEngine
{
    class LevelFileManager
    {
        static LevelFileManager* _instance;

    public:
        static LevelFileManager* Instance();


        Level* LoadLevel(int level);
        Level* LoadLevel(std::string file);

        void SaveLevel(Level* level, std::string file);
        void SaveLevel(Level* level, int num);

    private:
        LevelFileManager();
    };
}