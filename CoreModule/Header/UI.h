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

		virtual void Ready_UI(LPDIRECT3DDEVICE9 _device) = 0;
		virtual void Render_UI(LPDIRECT3DDEVICE9 _device) = 0;
		void Destroy() override;
		Component* Clone() const override = 0;

	protected:
		RenderOption 	m_Option;
		D3DXMATRIX 		m_TextureScaleMatrix;

	};
}