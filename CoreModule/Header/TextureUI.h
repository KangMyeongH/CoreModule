#pragma once
#include "UI.h"

namespace GameEngine
{
	class COREMODULE_API TextureUI : public UI
	{
	public:
		TextureUI() : UI(), m_pVertexBuffer(nullptr) {}
		explicit TextureUI(GameObject* _owner) : UI(_owner),m_pVertexBuffer(nullptr) {}

		~TextureUI();


		virtual void	Ready_Buffer(LPDIRECT3DDEVICE9 _device, IDirect3DVertexBuffer9* _buffer);
		void			Render_Buffer(LPDIRECT3DDEVICE9 _device);

		bool			Set_Texture(std::wstring _path);

		void			Set_NativeSize();
		void			Set_NormalSize();

		virtual	void	Update_Texture();
		virtual void	Render_Texture(LPDIRECT3DDEVICE9 _device);

		Component* Clone() const override
		{
			return new TextureUI(*this);
		}

		//======================================//
		//				 serialize				//
		//======================================//
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "TextureUI";
			_j = nlohmann::ordered_json{
				{"type", type}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
		}

	protected:
		IDirect3DVertexBuffer9* m_pVertexBuffer;

		std::wstring				m_Path;
		LPDIRECT3DTEXTURE9			m_Texture;
		//D3DXMATRIX					m_TextureScaleMatrix;

		D3DXMATRIX		m_projMatrix;

		// ºäÆ÷Æ® ÁÂÇ¥ °ø°£(À©µµ¿ì ÁÂÇ¥)
		float		m_SizeX, m_SizeY, m_X, m_Y;

		// UIÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
		void Destroy() override;
	};
}


