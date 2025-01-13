#include "TextUI.h"

#include "Transform.h"

GameEngine::TextUI::TextUI()
	: UI(nullptr), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f), m_ViewportWidth(0), m_ViewportHeight(0),
	  m_FontSize(24)
{
}

GameEngine::TextUI::TextUI(GameObject* _owner)
	: UI(_owner), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f), m_ViewportWidth(0), m_ViewportHeight(0),
	  m_FontSize(24)
{
}

GameEngine::TextUI::TextUI(const TextUI& _rhs)
	: UI(_rhs), m_Device(nullptr),
	  m_FontDesc(),
	  m_Font(nullptr),
	  m_Sprite(nullptr),
	  m_FontColor(1.f, 1.f, 1.f, 1.f), m_ViewportWidth(0), m_ViewportHeight(0),
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
	if (m_Font)
	{
		m_Font->Release();
		m_Font = nullptr;
	}

	if (m_Sprite)
	{
		m_Sprite->Release();
		m_Sprite = nullptr;
	}

	m_Device = _device;

	D3DVIEWPORT9 viewport;
	m_Device->GetViewport(&viewport);

	m_ViewportWidth = viewport.Width;
	m_ViewportHeight = viewport.Height;

	float heightRatio = static_cast<float>(m_ViewportHeight) / 1080.0f;

	ZeroMemory(&m_FontDesc, sizeof(D3DXFONT_DESC));

	m_FontDesc.CharSet = HANGUL_CHARSET;
	m_FontDesc.Width = 0;
	m_FontDesc.Height = static_cast<int>(static_cast<float>(m_FontSize) * heightRatio);
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
	D3DVIEWPORT9 viewport;
	_device->GetViewport(&viewport);

	float viewportWidth = static_cast<float>(viewport.Width);
	float viewportHeight = static_cast<float>(viewport.Height);

	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);
	_device->SetTransform(D3DTS_WORLD, &worldMat);

	float x = Get_Transform().Position().x;
	float y = Get_Transform().Position().y;

	float widthRatio = viewportWidth / 1920.0f;   // 기준 해상도: 1920x1080
	float heightRatio = viewportHeight / 1080.0f;

	if (viewport.Width != m_ViewportWidth || viewport.Height != m_ViewportHeight)
	{
		if (m_Font)
		{
			m_Font->Release();
			m_Font = nullptr;
		}

		m_ViewportWidth = viewport.Width;
		m_ViewportHeight = viewport.Height;

		m_FontDesc.Height = static_cast<int>(static_cast<float>(m_FontSize) * heightRatio);
		D3DXCreateFontIndirect(_device, &m_FontDesc, &m_Font);
	}

	RECT rc{
	static_cast<LONG>((x + 960) * widthRatio),                              // 왼쪽
	static_cast<LONG>((y + 540) * heightRatio),                             // 위쪽
	static_cast<LONG>((x + 1920.f) * widthRatio),                     // 오른쪽
	static_cast<LONG>((y + 1080.f) * heightRatio)                     // 아래쪽
	};

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (!m_Buffer.empty())
	{
		m_Font->DrawTextW(m_Sprite, m_Buffer.c_str(), -1, &rc, DT_NOCLIP, m_FontColor);
	}

	m_Sprite->End();
}

void GameEngine::TextUI::Set_FontSize(int _size)
{
	if (m_Font)
	{
		m_Font->Release();
		m_Font = nullptr;
	}

	float heightRatio = static_cast<float>(m_ViewportHeight) / 1080.0f;
	m_FontSize = _size;
	m_FontDesc.Height = static_cast<int>(static_cast<float>(m_FontSize) * heightRatio);
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

bool GameEngine::TextUI::Is_MouseHovered()
{
	return false;
}

bool GameEngine::TextUI::Is_ButtonDown()
{
	return false;
}

bool GameEngine::TextUI::Is_ButtonHold()
{
	return false;
}

bool GameEngine::TextUI::Is_ButtonUp()
{
	return false;
}