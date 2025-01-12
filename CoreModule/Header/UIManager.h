#pragma once
#include "core_define.h"

namespace GameEngine
{
	class TextureUI;

	class UIManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		UIManager() = default;
		~UIManager();
	public:
		DECLARE_SINGLETON(UIManager)

	public:
		void Register_UI();
		void Render_UI();
		void Add_TextureUI(TextureUI* _textureUI);


	private:
		std::vector<TextureUI*> 		m_TextureUI;

		std::list<TextureUI*>		m_RegisterQueue;
		std::list<TextureUI*>		m_DestroyQueue;
	};
}


