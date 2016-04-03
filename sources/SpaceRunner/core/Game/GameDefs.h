#pragma once
#include <memory>
#include <vector>

class ModelDrawable;

namespace CoreEngine
{
	using ModelPtr = std::shared_ptr<ModelDrawable>;
	using ModelList = std::vector<ModelPtr>;
	
	template<typename T>
	using UPtr = std::unique_ptr<T>;

	template<typename T>
	using Ptr = std::shared_ptr<T>;

	template <typename T>
	using PtrList = std::vector<std::shared_ptr<T>>;
}