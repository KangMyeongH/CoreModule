#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API Renderer : public Behaviour
	{
	public:
		explicit Renderer(GameObject* _owner)
			: Behaviour(_owner) {}

		virtual void Render() = 0;
	};

}
