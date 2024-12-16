#pragma once
#include <cstdint>

namespace GameEngine
{
	enum class TransformDirtyFlags : std::uint8_t
	{
		None = 0,				// 0000
		LocalDirty = 1 << 0,	// 0001 
		WorldDirty = 1 << 1,	// 0010
		ParentChanged = 1 << 2	// 0100
	};
}
