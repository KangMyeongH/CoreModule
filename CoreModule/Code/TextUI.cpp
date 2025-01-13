#include "TextUI.h"

#include "Transform.h"

GameEngine::TextUI::TextUI()
	: UI(nullptr), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f),
	  m_FontSize(24)
{
}

GameEngine::TextUI::TextUI(GameObject* _owner)
	: UI(_owner), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f),
	  m_FontSize(24)
{
}

GameEngine::TextUI::TextUI(const TextUI& _rhs)
	: UI(_rhs), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f),
	  m_FontSize(24)
{
}

GameEngine::TextUI::~TextUI()
{
	if (m_Sprite)
	{
		m_Sprite->Release();
	}

	if (m_Font)
	{
		m_Font->Release();
	}

}

void GameEngine::TextUI::Ready_UI(LPDIRECT3DDEVICE9 _device)
{
	m_Device = _device;

	ZeroMemory(&m_FontDesc, sizeof(D3DXFONT_DESC));

	m_FontDesc.CharSet = HANGUL_CHARSET;
	m_FontDesc.Width = 0;
	m_FontDesc.Height = m_FontSize;
	m_FontDesc.Weight = FW_NORMAL;
	lstrcpy(m_FontDesc.FaceName, L"Noto Sans KR Regular");

	if (FAILED(D3DXCreateFontIndirect(_device, &m_FontDesc, &m_Font)))
	{
		return;
	}

	if (FAILED(D3DXCreateSprite(_device, &m_Sprite)))
	{
		return;
	}
}

void GameEngine::TextUI::Render_UI(LPDIRECT3DDEVICE9 _device)
{
	int x = static_cast<int>(Get_Transform().Position().x);
	int y = static_cast<int>(Get_Transform().Position().y);
	RECT rc{ x - 960, y - 540};

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_Font->DrawTextW(m_Sprite, m_Buffer.c_str(), lstrlen(m_Buffer.c_str()), &rc, DT_NOCLIP, m_FontColor);

	m_Sprite->End();
}

void GameEngine::TextUI::Set_FontSize(int _size)
{
	if (m_Font)
	{
		m_Font->Release();
		m_Font = nullptr;
	}

	m_FontDesc.Height = _size;

	if (FAILED(D3DXCreateFontIndirect(m_Device, &m_FontDesc, &m_Font)))
	{
		return;
	}
}

void GameEngine::TextUI::to_json(nlohmann::ordered_json& _j)
{
	std::string type = "TextUI";
	_j = nlohmann::ordered_json
	{
		{"type", type},
		{"enable", m_bEnabled},
		{"fontSize", m_FontSize},
		{"buffer", m_Buffer},
		{"colorR", m_FontColor.r},
		{"colorG", m_FontColor.g},
		{"colorB", m_FontColor.b},
		{"colorA", m_FontColor.a}
	};
}

void GameEngine::TextUI::from_json(const nlohmann::ordered_json& _j)
{
	if (_j.contains("enable"))
	{
		_j.at("enable").get_to(m_bEnabled);
	}
	if (_j.contains("fontSize"))
	{
		_j.at("fontSize").get_to(m_FontSize);
	}
	if (_j.contains("buffer"))
	{
		_j.at("buffer").get_to(m_Buffer);
	}
	if (_j.contains("colorR"))
	{
		_j.at("colorR").get_to(m_FontColor.r);
	}
	if (_j.contains("colorG"))
	{
		_j.at("colorG").get_to(m_FontColor.g);
	}
	if (_j.contains("colorB"))
	{
		_j.at("colorB").get_to(m_FontColor.b);
	}
	if (_j.contains("colorA"))
	{
		_j.at("colorA").get_to(m_FontColor.a);
	}
}
