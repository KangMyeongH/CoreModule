#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class UI : public Behaviour
	{
	public:
		UI() : Behaviour(nullptr)
		{}
		explicit UI(GameObject* _owner) : Behaviour(_owner)
		{}
	};
}

