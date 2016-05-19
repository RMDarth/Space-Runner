#pragma once
#include "Control.h"

class SlidePanelControl : public Control
{
    int _storedX;
    int _diff;
    bool _moving = false;

    int _prevX[2];
	int _lastX;
    int _speed;
    float _stoppingTime;
	float _stepDuration;
    bool _stopping = false;

    int _realWidth;
    int _curPos;
    int _originalPos;


public:
	SlidePanelControl(SceneSector* sector, std::string name, float x, float y, float width, float height, Control * parent = nullptr);

	void Update(float time) override;

	bool OnMouseMove(int x, int y, float deltaTime) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;

    std::string GetCustomAttribute(std::string name) override;
    void SetCustomAttribute(std::string name, std::string value) override;

private:
    void Move(int x);
};