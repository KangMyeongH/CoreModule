#pragma once
#include "UI.h"

namespace GameEngine
{
	class COREMODULE_API TextureUI : public UI
	{
	public:
		TextureUI()
		: UI(nullptr),
		m_VertexBuffer(nullptr),
		m_Texture(nullptr)
		{}

		explicit TextureUI(GameObject* _owner)
		: UI(_owner),
		m_VertexBuffer(nullptr),
		m_Texture(nullptr)
		{}

		~TextureUI() override;

		void			Ready_Buffer(LPDIRECT3DDEVICE9 _device, IDirect3DVertexBuffer9* _buffer);
		void 			Ready_UI(LPDIRECT3DDEVICE9 _device) override;
		bool			Set_Texture(const std::wstring& _path);
		void			Set_NativeSize();
		void 			Render_UI(LPDIRECT3DDEVICE9 _device) override;

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
				{"type", type},
				{"enable", m_bEnabled},
				{"path", m_Path}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
			if (_j.contains("enable"))
			{
				_j.at("enable").get_to(m_bEnabled);
			}

			if (_j.contains("path"))
			{
				_j.at("path").get_to(m_Path);
			}
		}

	protected:
		IDirect3DVertexBuffer9* m_VertexBuffer;
		LPDIRECT3DTEXTURE9		m_Texture;
		std::wstring			m_Path;
	};
}


