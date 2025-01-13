#pragma once
#include "core_define.h"
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

	private:
		LPDIRECT3DDEVICE9		m_Device;
		IDirect3DVertexBuffer9* m_VertexBuffer;
		
		std::vector<UI*> 		mUI[UI::RENDER_OPTION_END];
		std::list<UI*>			m_RegisterQueue;
		std::list<UI*>			m_DestroyQueue;
	};
}


