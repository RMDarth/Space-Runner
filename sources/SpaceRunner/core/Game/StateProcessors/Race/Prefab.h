#pragma once
#include "LevelStructure.h"

#include <vector>
#include <string>

namespace CoreEngine
{
    class Prefab
    {
        int _rowNum;

        struct Row
        {
            struct Object
            {
                SpaceObjectType type;
                float speed;
                int param;
            } objects[3];
        };

        std::vector<Row> _rowList;

    public:
        Prefab();
        Prefab(std::string filename);
        ~Prefab();

        void Load(std::string filename);
        const std::vector<Row>& getRowList();
    };
}