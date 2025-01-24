#include "SpineMaterial.h"

GameEngine::SpineMaterial::SpineMaterial(LPDIRECT3DDEVICE9 _device, const std::string& _effectPath): m_Device(_device),
	m_Effect(nullptr)
{
	if (m_Device)
	{
		m_Device->AddRef();
	}

	Load_EffectFromFile(_effectPath);
}

GameEngine::SpineMaterial::~SpineMaterial()
{
	if (m_Effect) m_Effect->Release();
	if (m_Device) m_Device->Release();
}

void GameEngine::SpineMaterial::Set_WorldMat(const D3DXMATRIX& _worldMat)
{
	if (m_Effect)
	{
		m_Effect->SetMatrix("gWorld", &_worldMat);
	}
}

void GameEngine::SpineMaterial::Set_ViewProjMat(const D3DXMATRIX& _view, const D3DXMATRIX& _proj)
{
	if (m_Effect)
	{
		m_Effect->SetMatrix("gView", &_view);
		m_Effect->SetMatrix("gProj", &_proj);
	}
}

void GameEngine::SpineMaterial::Set_Texture(const std::string& _paramName, LPDIRECT3DTEXTURE9 _texture)
{
	if (m_Effect)
	{
		m_Effect->SetTexture(_paramName.c_str(), _texture);
	}
}

void GameEngine::SpineMaterial::Set_Light(const D3DLIGHT9* _light)
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

void GameEngine::SpineMaterial::Set_Color(const std::string& _paramName, const D3DXVECTOR4& _color)
{
	if (m_Effect)
	{
		m_Effect->SetVector(_paramName.c_str(), &_color);
	}
}

void GameEngine::SpineMaterial::Set_Bool(const std::string& _paramName, const bool _bool)
{
	if (m_Effect)
	{
		m_Effect->SetBool(_paramName.c_str(), _bool);
	}
}

void GameEngine::SpineMaterial::Set_Float(const std::string& _paramName, const float _float)
{
	if (m_Effect)
	{
		m_Effect->SetFloat(_paramName.c_str(), _float);
	}
}

bool GameEngine::SpineMaterial::Load_EffectFromFile(const std::string& _filePath)
{
	ID3DXBuffer* err = nullptr;
	HRESULT hr = D3DXCreateEffectFromFileA(
		m_Device,
		_filePath.c_str(),
		nullptr, nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&m_Effect,
		&err);

	if (FAILED(hr))
	{
		if (err)
		{
			const char* errMsg = (const char*)err->GetBufferPointer();
			MessageBoxA(nullptr, errMsg, "Effect Error", MB_OK);
			err->Release();
		}
		return false;
	}
	return true;
}

void GameEngine::SpineMaterial::Begin()
{
	if (!m_Effect) return;
	m_Effect->SetTechnique("SpineTech");
	UINT passCount = 0;
	m_Effect->Begin(&passCount, 0);
}

void GameEngine::SpineMaterial::Begin_Pass(UINT _pass)
{
	if (m_Effect)
	{
		m_Effect->BeginPass(_pass);
	}
}

void GameEngine::SpineMaterial::End_Pass()
{
	if (m_Effect)
	{
		m_Effect->EndPass();
	}
}

void GameEngine::SpineMaterial::End()
{
	if (m_Effect)
	{
		m_Effect->End();
	}
}
