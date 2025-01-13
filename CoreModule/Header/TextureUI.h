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
			m_Texture(nullptr), m_bFlipX(false), m_bFlipY(false)
		{}

		explicit TextureUI(GameObject* _owner)
			: UI(_owner),
			  m_VertexBuffer(nullptr),
			  m_Texture(nullptr), m_bFlipX(false), m_bFlipY(false)
		{}

		explicit TextureUI(GameObject* _owner, const std::wstring& _path)
			: UI(_owner),
			m_VertexBuffer(nullptr),
			m_Texture(nullptr), m_Path(_path),m_bFlipX(false), m_bFlipY(false)
		{}
		TextureUI(const TextureUI& _rhs);

		~TextureUI() override;

		void			Ready_Buffer(LPDIRECT3DDEVICE9 _device, IDirect3DVertexBuffer9* _buffer);
		void 			Ready_UI(LPDIRECT3DDEVICE9 _device) override;
		bool			Set_Texture(const std::wstring& _path);
		std::wstring	Get_Path() const { return m_Path; }

		void			Set_FlipX(bool _isFlip) { m_bFlipX = _isFlip; }
		bool			Get_FlipX() const { return m_bFlipX; }

		void			Set_FlipY(bool _isFlip) { m_bFlipY = _isFlip; }
		bool			Get_FlipY() const { return m_bFlipY; }

		RECT			Get_Rect() const;

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
				{"path", m_Path},
				{"flipX", m_bFlipX},
				{"flipY", m_bFlipY}
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
			if (_j.contains("flipX"))
			{
				_j.at("flipX").get_to(m_bFlipX);
			}
			if (_j.contains("flipY"))
			{
				_j.at("flipY").get_to(m_bFlipY);
			}
		}

		bool Is_MouseHovered() override;
		bool Is_ButtonDown() override;
		bool Is_ButtonHold() override;
		bool Is_ButtonUp() override;

	private:
		IDirect3DVertexBuffer9* m_VertexBuffer;
		LPDIRECT3DTEXTURE9		m_Texture;
		std::wstring			m_Path;
		bool					m_bFlipX;
		bool					m_bFlipY;
	};
	REGISTER_COMPONENT(TextureUI)
}


