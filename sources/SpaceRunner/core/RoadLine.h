#pragma once
#include "Basic.h"
#include <memory>
#include <vector>
#include <string>

class ModelDrawable;
class SceneSector;


#define BLOCKS_NUM 3

namespace CoreEngine
{
	class RoadBlock;

	class RoadLine
	{
		Vector3 position;

		ModelDrawable* _specialModel;
		ModelDrawable* _fenceLeft;
		ModelDrawable* _fenceRight;
		ModelDrawable* _borderLeft;
		ModelDrawable* _borderRight;

		SceneSector* _fenceLeftSector;
		SceneSector* _fenceRightSector;
		SceneSector* _borderLeftSector;
		SceneSector* _borderRightSector;

		SceneSector * _sector;
		RoadBlock* _roadBlockList[BLOCKS_NUM];

	public:
		RoadLine(Vector3 pos);
		~RoadLine();
		void SetPosition(Vector3 position);
		Vector3 GetPosition();
		void SetFence(std::string fance = "fense.mesh");
		void SetBorder(std::string border = "border.mesh");
		void SetSpecialModel(std::string specialModel = "");
		void SetRoadBlock(int index, int roadTile, std::string specialModel = "", bool blocked = false);
		void SetVisible(bool visible);

		void Update(float time);
	};
}