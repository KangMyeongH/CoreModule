#include "Material.h"

GameEngine::Material::Material(LPDIRECT3DDEVICE9 _device, const std::string& _effectPath) : m_Device(_device), m_Effect(nullptr), m_EffectPath(_effectPath)
{
	if (m_Device)
	{
		m_Device->AddRef();
	}

	Load_EffectFromFile(_effectPath);
}

GameEngine::Material::~Material()
{
	if (m_Effect) m_Effect->Release();
	if (m_Device) m_Device->Release();
}

void GameEngine::Material::Set_WorldMat(const D3DXMATRIX& _worldMat)
{
	if (m_Effect)
	{
		m_Effect->SetMatrix("gWorld", &_worldMat);
	}
}

void GameEngine::Material::Set_ViewProjMat(const D3DXMATRIX& _view, const D3DXMATRIX& _proj)
{
	if (m_Effect)
	{
		m_Effect->SetMatrix("gView", &_view);
		m_Effect->SetMatrix("gProj", &_proj);
	}
}

void GameEngine::Material::Set_Texture(const std::string& _paramName, LPDIRECT3DTEXTURE9 _texture)
{
	if (m_Effect)
	{
		m_Effect->SetTexture(_paramName.c_str(), _texture);
	}
}

void GameEngine::Material::Set_Light(const D3DLIGHT9* _light)
{
	if (m_Effect)
	{
		D3DXVECTOR4 dir = { _light->Direction.x, _light->Direction.y, _light->Direction.z, 0 };
		D3DXVECTOR4 diffuse = { _light->Diffuse.r,_light->Diffuse.g, _light->Diffuse.b, _light->Diffuse.a };
		D3DXVECTOR4 ambient = { _light->Ambient.r, _light->Ambient.g, _light->Ambient.b, _light->Ambient.a };
		m_Effect->SetVector("gLightDirection", &dir);
		m_Effect->SetVector("gLightColor", &diffuse);
		m_Effect->SetVector("gAmbientColor", &ambient);
	}
}

void GameEngine::Material::Set_Color(const std::string& _paramName, const D3DXVECTOR4& _color)
{
	if (m_Effect)
	{
		m_Effect->SetVector(_paramName.c_str(), &_color);
	}
}

void GameEngine::Material::Set_Billboard(bool _isBillboard)
{
	if (m_Effect)
	{
		m_Effect->SetBool("gEnableBillboard", _isBillboard);
	}
}

bool GameEngine::Material::Load_EffectFromFile(const std::string& _filePath)
{
	ID3DXBuffer* pErr = nullptr;
	HRESULT hr = D3DXCreateEffectFromFileA(
		m_Device,
		_filePath.c_str(),
		nullptr, nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&m_Effect,
		&pErr
	);

	if (FAILED(hr))
	{
		if (pErr)
		{
			const char* errMsg = (const char*)pErr->GetBufferPointer();
			MessageBoxA(nullptr, errMsg, "Effect Error", MB_OK);
			pErr->Release();
		}
		return false;
	}
	return true;
}

void GameEngine::Material::Begin()
{
	if (!m_Effect) return;
	m_Effect->SetTechnique("DirLightOnly");
	UINT passCount = 0;
	m_Effect->Begin(&passCount, 0);
}

void GameEngine::Material::Begin_Pass(UINT _pass)
{
	if (m_Effect)
	{
		m_Effect->BeginPass(_pass);
	}
}

void GameEngine::Material::End_Pass()
{
	if (m_Effect)
	{
		m_Effect->EndPass();
	}
}

void GameEngine::Material::End()
{
	if (m_Effect)
	{
		m_Effect->End();
	}
}
