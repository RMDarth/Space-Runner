#include "FileSystem.h"
#include "Prefab.h"

namespace CoreEngine
{
    Prefab::Prefab()
    {
    }

    Prefab::Prefab(std::string filename)
    {
        Load(filename);
    }

    Prefab::~Prefab()
    {
        _rowNum = 0;
        _inverted = false;
        _mirror = false;
        _rowList.clear();
    }

    void Prefab::Load(std::string filename)
    {
        FileInputRef file(filename);
        if (!file.IsOpened())
            return;

        _rowNum = file.ReadInt();
        _mirror = file.ReadInt() == 1;

        for (int rowId = 0; rowId < _rowNum; rowId++)
        {
            Row row;
            for (auto lane = 0; lane < 3; lane++)
            {
                row.objects[lane].type = static_cast<SpaceObjectType>(file.ReadInt());
                row.objects[lane].speed = file.ReadFloat();
                row.objects[lane].param = file.ReadInt();
            }
            _rowList.push_back(std::move(row));
        }
        file.Close();
    }

    const std::vector<Prefab::Row>& Prefab::getRowList()
    {
        return _rowList;
    }

    bool Prefab::isMirrorable()
    {
        return _mirror;
    }

    void Prefab::setInverted(bool value)
    {
        _inverted = value;
    }

    bool Prefab::isInverted()
    {
        return _inverted;
    }


}