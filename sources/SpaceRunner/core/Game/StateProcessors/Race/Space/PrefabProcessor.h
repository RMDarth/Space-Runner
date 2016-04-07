#pragma once

namespace CoreEngine
{
    class Space;
    class Prefab;

    class PrefabProcessor
    {
        Space *_space;
    public:
        PrefabProcessor(Space *space);

        void Process(Prefab * prefab, float totaltime);
    };
}
