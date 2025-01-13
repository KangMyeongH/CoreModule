#pragma once
#include "UI.h"

namespace GameEngine
{
	class COREMODULE_API TextUI final : public UI
	{
	public:
		TextUI();
		explicit TextUI(GameObject* _owner);
		TextUI(const TextUI& _rhs);
		~TextUI() override;

		void Ready_UI(LPDIRECT3DDEVICE9 _device) override;
		void Render_UI(LPDIRECT3DDEVICE9 _device) override;

		std::wstring Get_Buffer() const { return m_Buffer; }
		void Set_Buffer(const std::wstring& _buff) { m_Buffer = _buff; }

		int Get_FontSize() const { return m_FontSize; }
		void Set_FontSize(int _size);

		D3DXCOLOR Get_FontColor() const { return m_FontColor; }
		void Set_FontColor(const D3DXCOLOR _color) { m_FontColor = _color; }

		bool Is_MouseHovered() override;
		bool Is_ButtonDown() override;
		bool Is_ButtonHold() override;
		bool Is_ButtonUp() override;

	public:
		void to_json(nlohmann::ordered_json& _j) override;
		void from_json(const nlohmann::ordered_json& _j) override;
		Component* Clone() const override
		{
			return new TextUI(*this);
		}

	private:
		LPDIRECT3DDEVICE9 	m_Device;
		D3DXFONT_DESC		m_FontDesc;
		LPD3DXFONT			m_Font;
		LPD3DXSPRITE		m_Sprite;
		D3DXCOLOR			m_FontColor;
		std::wstring 		m_Buffer;
		INT					m_FontSize;
	};

	REGISTER_COMPONENT(TextUI)
}

