#pragma once
#include "IStateProcessor.h"
class SceneSector;

namespace CoreEngine
{
	class SliderProcessor
	{
	protected:
		int _objectCount;
		float _speed;
		float _objectSize;

		// current pos
		float _baseSlide;
		float _diffSlide;

		// swipe
		float _startSlide;
		float _currentSlide;
		float _ignoreSlide;

		int _currentObject;

		bool _moving;

		virtual void UpdateSlide(float time);
		virtual void SetObjectPosition(int object, float left, float top, float right, float bottom) { };
		virtual void SetCurrentObject(int object) { };
		

		void Reset();
		void Init();
		void OnMouseDown(int x, int y);
		void OnMouseUp(int x, int y);
		void OnMouseMove(int x, int y);

	public:
		SliderProcessor(int objectCount, float speed, float objectSize);
		virtual ~SliderProcessor();

	private:
		void Slide(float slide);
	};
}



