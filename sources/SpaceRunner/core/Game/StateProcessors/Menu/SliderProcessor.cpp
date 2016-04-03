#include "SliderProcessor.h"
#include "RenderProcessor.h"
#include <sstream>
using namespace std;

namespace CoreEngine
{
	SliderProcessor::SliderProcessor(int objectCount, float speed, float objectSize)
	{
		_objectCount = objectCount;
		_objectSize = objectSize;
		_speed = speed;
		_startSlide = 0;
		_baseSlide = 0;
		_diffSlide = 0;
		_ignoreSlide = 0;
		_currentSlide = 0;
		_currentObject = 0;
		_moving = false;
	}


	SliderProcessor::~SliderProcessor()
	{

	}

	void SliderProcessor::Init()
	{
		for (int i = 0; i < _objectCount; i++)
		{
			SetObjectPosition(i, -0.5f + 1.25f*i, 0.5f, 0.5f + 1.25f*i, -0.5f);
		}
	}

	void SliderProcessor::UpdateSlide(float time)
	{
		if (_moving == false && fabs(_diffSlide) > 0.01f)
		{
			if (_diffSlide > 0)
			{
				_diffSlide -= _speed * time;
				if (_diffSlide < 0)
					_diffSlide = 0;
			}
			else
			{
				_diffSlide += _speed * time;
				if (_diffSlide > 0)
					_diffSlide = 0;
			}
			Slide(_diffSlide);
		}
	}

	void SliderProcessor::Reset()
	{
		Slide(0);
	}

	void SliderProcessor::OnMouseDown(int x, int y)
	{
		_moving = true;

		int left, top, width, height;
		RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);
		_startSlide = -(((float)x / width) * 2.0f - 1.0f) + _diffSlide;
	}

	void SliderProcessor::OnMouseUp(int x, int y)
	{
		int left, top, width, height;
		RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);
		if (fabs(_currentSlide + _startSlide) * 2 * width < width * 0.03f)
		{
			_diffSlide = 0;
			_startSlide = 0;
			float delta = (1.0f - _objectSize) / 2.0f;
			float pos = (float)x / width;
			if (pos > delta && pos < (1.0f - delta))
			{

				SetCurrentObject(_currentObject);
			}
			else if (pos > delta)
			{
				if (_baseSlide > -1.25f * (_objectCount - 1) + 0.01f)
				{
					_baseSlide -= 1.25f;
					_diffSlide += 1.25f;
					_currentObject++;
				}
			}
			else {
				if (_baseSlide < -0.01f)
				{
					_baseSlide += 1.25f;
					_diffSlide -= 1.25f;
					_currentObject--;
				}
			}
		}
		else {
			_diffSlide = _startSlide + _currentSlide - _ignoreSlide;
			if (_diffSlide < -0.00001)
			{
				if (_baseSlide > -1.25f * (_objectCount - 1) + 0.01f)
				{
					_baseSlide -= 1.25f;
					_diffSlide += 1.25f;
					_currentObject++;
				}
			}
			else if (_diffSlide > 0.00001) {
				if (_baseSlide < -0.01f)
				{
					_baseSlide += 1.25f;
					_diffSlide -= 1.25f;
					_currentObject--;
				}
			}
			_startSlide = 0;
		}

		_moving = false;
	}

	void SliderProcessor::OnMouseMove(int x, int y)
	{
		if (_moving)
		{
			_ignoreSlide = 0;
			int left, top, width, height;
			RenderProcessor::Instance()->GetViewport()->getActualDimensions(left, top, width, height);

			_currentSlide = ((float)x / (float)width) * 2.0f - 1.0f;
			if (_currentSlide + _startSlide + _baseSlide > 0 && _currentObject == 0)
				_ignoreSlide = _currentSlide + _startSlide;
			if (_currentSlide + _startSlide + _baseSlide < -1.25f * (_objectCount - 1) && _currentObject == _objectCount - 1)
				_ignoreSlide = _currentSlide + _startSlide;
			Slide(_currentSlide);
		}
	}

	void SliderProcessor::Slide(float slide)
	{
		for (int i = 0; i < _objectCount; i++)
		{
			SetObjectPosition(i, _baseSlide + _startSlide - 0.5f + 1.25f*i + slide - _ignoreSlide, 0.5f, _baseSlide + _startSlide + 0.5f + 1.25f*i + slide - _ignoreSlide, -0.5f);
		}		
	}

}
