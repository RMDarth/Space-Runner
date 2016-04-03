#pragma once

class IDrawable
{
public:
	virtual ~IDrawable()  { } ;

	virtual void Update(float deltaTime) = 0;
	virtual void SetVisible(bool value) = 0;
};