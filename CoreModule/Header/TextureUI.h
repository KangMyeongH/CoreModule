#pragma once
#include "UI.h"

namespace GameEngine
{
	class TextureUI : public UI
	{
	public:
		TextureUI() : UI(nullptr), m_pVertexBuffer(nullptr) {}
		explicit TextureUI(GameObject* _owner);
		~TextureUI() override;


		virtual void	Ready_Buffer(LPDIRECT3DDEVICE9 _device);
		void			Render_Buffer(LPDIRECT3DDEVICE9 _device);

		bool			Set_Texture(std::wstring _path);

		void			Set_NativeSize();
		void			Set_NormalSize();

		virtual	void	Update_Texture(LPDIRECT3DDEVICE9 _device);
		virtual void	Render_Texture(LPDIRECT3DDEVICE9 _device);

	protected:
		IDirect3DVertexBuffer9* m_pVertexBuffer;

		std::wstring				m_Path;
		LPDIRECT3DTEXTURE9			m_Texture;
		//D3DXMATRIX					m_TextureScaleMatrix;

		D3DXMATRIX		m_projMatrix;

		// ºäÆ÷Æ® ÁÂÇ¥ °ø°£(À©µµ¿ì ÁÂÇ¥)
		float		m_SizeX, m_SizeY, m_X, m_Y;
	};
}


