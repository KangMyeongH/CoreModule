#pragma once
#include "core_define.h"
#include "Transform.h"
#include "UI.h"

namespace GameEngine
{
	class TextureUI;

	class COREMODULE_API UIManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		UIManager() = default;
		~UIManager()
		{
			Release();
		}
	public:
		DECLARE_SINGLETON(UIManager)

	public:
		IDirect3DVertexBuffer9* Get_VertexBuffer() const { return m_VertexBuffer; }

		void 	Initialize(LPDIRECT3DDEVICE9 _device);
		void 	Render_UI();
		void 	Add_UI(UI* _ui);

		void 	Remove_Renderer(UI* _ui);
		void 	Register_UI();
		void 	Destroy_UI();
		void 	Clear_Component();
		void 	Release();

		void 	Render_LoadingScreen();

		void	Set_FadeEffect(float _fadeSpeed) { m_FadeSpeed = _fadeSpeed; }
		void	Enable_FadeIn() { m_bFadeIn = true; m_bFadeOut = false; m_bFadeInFinish = false; }
		void	Enable_FadeOut() { m_bFadeOut = true; m_bFadeOutFinish = false; }
		bool	Is_FadeOut() const { return m_bFadeOut; }
		bool	Is_FadeIn() const { return m_bFadeIn; }
		bool	Is_FadeInFinish() const { return m_bFadeInFinish; }
		bool	Is_FadeOutFinish() const { return m_bFadeOutFinish; }

	private:
		void 	update_FadeEffect();
		void	render_FadeEffect();

	private:
		LPDIRECT3DDEVICE9		m_Device;
		IDirect3DVertexBuffer9* m_VertexBuffer;

		VTXCOLORUI				m_IconVertex[4];
		VTXCOLORUI				m_StarVertex[4];
		Transform				m_IconTransform;
		Transform				m_StarTransform;
		D3DXMATRIX				m_IconScale;
		D3DXMATRIX				m_StarScale;

		std::vector<UI*> 		mUI[UI::RENDER_OPTION_END];
		std::list<UI*>			m_RegisterQueue;
		std::list<UI*>			m_DestroyQueue;

		FADE_VERTEX 			m_FadeVertex[4];
		float 					m_FadeAlpha;
		float 					m_FadeSpeed;
		bool 					m_bFadeIn;
		bool 					m_bFadeOut;
		bool					m_bFadeInFinish;
		bool					m_bFadeOutFinish;
	};
}


