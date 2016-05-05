#include "SpaceObject.h"
#include "SpaceDefs.h"
#include "Render/SceneSector.h"

namespace CoreEngine
{

    SpaceObject::SpaceObject(Vector3 offset, float speed)
        : _pos(std::move(offset))
        , _speed(speed)
        , _destroyed(false)
    {
    }

    SpaceObject::~SpaceObject()
    {

    }

    void SpaceObject::Update(float time, float roadSpeed)
    {
        _pos.x -= _speed * time;
        _pos.x += roadSpeed;
    }

    bool SpaceObject::IsIntersected(float turn)
    {
        return _pos.x < 10 + getHalfSize().x && _pos.x > 10 - getHalfSize().x
               && _pos.z > turn - getHalfSize().z && _pos.z < turn + getHalfSize().z;
    }

    bool SpaceObject::IsIntersected(SpaceObject * other)
    {
        Vector3 dist = _pos - other->_pos;
        dist.Abs();

        return other->getHalfSize().x + getHalfSize().x > dist.x
               && other->getHalfSize().z + getHalfSize().z > dist.z;
    }

    bool SpaceObject::IsDone()
    {
        return Passed() || _destroyed;
    }

	bool SpaceObject::Passed()
	{
		return _pos.x > BLOCK_SIZE * 5;
	}

    void SpaceObject::Destroy()
    {
        _destroyed = true;
    }

    const Vector3& SpaceObject::getPos() const
    {
        return _pos;
    }

    const Vector3& SpaceObject::getHalfSize()
    {
        static Vector3 standardSize(2, 2, 1.2f);
        return standardSize;
    }

    SceneSector* SpaceObject::getSector()
    {
        return _sector;
    }
}