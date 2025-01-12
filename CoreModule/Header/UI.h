#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API UI : public Behaviour
	{
	public:
		enum RenderOption
		{
			ALPHA_RENDERING,
			ALPHA_BLENDING,
			RENDER_OPTION_END
		};

		UI() : Behaviour(nullptr), m_Option(ALPHA_RENDERING)
		{
		}

		explicit UI(GameObject* _owner) : Behaviour(_owner), m_Option(ALPHA_RENDERING)
		{
		}

		~UI() override;

	public:
		RenderOption Get_RenderOption() const { return m_Option; }

	private:
		//std::wstring		m_Path;
		//LPDIRECT3DTEXTURE9	m_Texture;

		RenderOption m_Option;
	};
}