#pragma once
#include "LevelStructure.h"

#include <vector>
#include <string>

namespace CoreEngine
{
    class Prefab
    {
        int _rowNum;
        bool _mirror;
        bool _inverted;

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
        bool isMirrorable();

        void setInverted(bool value);
        bool isInverted();
    };
}