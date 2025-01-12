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
		~UIManager() = default;
	public:
		DECLARE_SINGLETON(UIManager)

	public:
		void Initialize(LPDIRECT3DDEVICE9 _device);
		void Render_UI();
		void Add_TextureUI(TextureUI* _textureUI);

		void Remove_Renderer(TextureUI* _textureUI);
		void Register_UI();
		void Destroy_UI();
		void Clear_Component();
		void Release();

	private:

		LPDIRECT3DDEVICE9		m_Device;
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		
		std::vector<TextureUI*> 		m_TextureUI[UI::RENDER_OPTION_END];

		std::list<TextureUI*>		m_RegisterQueue;
		std::list<TextureUI*>		m_DestroyQueue;
	};
}


