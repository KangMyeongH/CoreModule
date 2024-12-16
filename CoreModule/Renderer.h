#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class Renderer : public Behaviour
	{
	public:
		explicit Renderer(GameObject* _owner)
			: Behaviour(_owner) {}

		virtual void Render() = 0;
	};

}
