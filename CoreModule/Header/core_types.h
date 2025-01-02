#pragma once
#include <cstdint>

namespace GameEngine
{
	typedef int TransformDirtyFlags;

	enum class ColliderType : std::uint8_t
	{
		Sphere,
		Box
	};
}
