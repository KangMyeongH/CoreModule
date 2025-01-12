#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class UI : public Behaviour
	{
	public:
		UI() : Behaviour(nullptr), m_Texture(nullptr)
		{
		}

		explicit UI(GameObject* _owner) : Behaviour(_owner), m_Texture(nullptr)
		{
		}

		~UI() override;

	private:
		std::wstring		m_Path;
		LPDIRECT3DTEXTURE9	m_Texture;
	};
}