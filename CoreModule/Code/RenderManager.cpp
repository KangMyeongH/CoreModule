#include "RenderManager.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextureRenderer.h"
#include "Light.h"

IMPLEMENT_SINGLETON(GameEngine::RenderManager)

void GameEngine::RenderManager::Initialize(LPDIRECT3DDEVICE9 _device)
{
	m_Device = _device;
	_device->AddRef();


}

void GameEngine::RenderManager::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	std::pair<LPDIRECT3DVERTEXBUFFER9, LPDIRECT3DINDEXBUFFER9> buffer;

	for (auto& renderer : m_RegisterQueue)
	{
		if (dynamic_cast<CubeRenderer*>(renderer))
		{
			if (m_BufferMap.find(CUBE) == m_BufferMap.end())
			{
				renderer->Ready_Buffer(_device);
				renderer->Get_Buffer(buffer.first, buffer.second);

				m_BufferMap.insert({ CUBE, buffer });
				buffer.first->AddRef();
				buffer.second->AddRef();
			}
			else
			{
				buffer = (m_BufferMap.find(CUBE))->second;
				renderer->Set_Buffer(buffer.first, buffer.second);
			}
		}

		else if (dynamic_cast<TextureRenderer*>(renderer))
		{
			if (m_BufferMap.find(TEXTURE) == m_BufferMap.end())
			{
				renderer->Ready_Buffer(_device);
				renderer->Get_Buffer(buffer.first, buffer.second);
				buffer.first->AddRef();
				buffer.second->AddRef();

				m_BufferMap.insert({ TEXTURE, buffer });
			}
			else
			{
				buffer = (m_BufferMap.find(TEXTURE))->second;
				renderer->Set_Buffer(buffer.first, buffer.second);
			}
		}

	}

}

GameEngine::RenderManager::~RenderManager()
{
}

void GameEngine::RenderManager::Render_Begin(LPDIRECT3DDEVICE9 _device)
{
	D3DXCOLOR backColor = { 0.2f, 0.2f, 0.2f, 1.f };
	_device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		backColor,
		1.f,
		0);

	_device->BeginScene();
}

void GameEngine::RenderManager::Render(LPDIRECT3DDEVICE9 _device)
{
	if (FAILED(_device->SetTransform(D3DTS_VIEW, &m_ViewMat)))
	{
		return;
	}
	if (FAILED(_device->SetTransform(D3DTS_PROJECTION, &m_ProjMat)))
	{
		return;
	}

	m_Device->SetRenderState(D3DRS_LIGHTING, true);

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	std::sort(m_Renderers.begin(), m_Renderers.end(), [&_device](Renderer* dst, Renderer* src)->bool
		{
			D3DXMATRIX matCameraWorld;

			_device->GetTransform(D3DTS_VIEW, &matCameraWorld);
			D3DXMatrixInverse(&matCameraWorld, 0, &matCameraWorld);

			Vector3   cameraPosition;
			memcpy(&cameraPosition, &matCameraWorld.m[3][0], sizeof(Vector3));

			Vector3 dstZ = cameraPosition - dst->Get_Transform().Position();
			Vector3	srcZ = cameraPosition - src->Get_Transform().Position();

			D3DXVec3Length(&dstZ);
			D3DXVec3Length(&srcZ);

			return dstZ > srcZ;
		});

	//임시
	if (m_GlobalLight)
	{
		m_GlobalLight->Update_Light(_device);
		m_GlobalLight->Ready_Light(_device); //test
		_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		//_device->SetRenderState(D3DRS_SPECULARENABLE, true);
	}

	for (const auto& renderer : m_Renderers)
	{
		if (renderer->Is_Enabled() && renderer->Get_GameObject()->Is_Active())
		{
			renderer->Render(_device);
		}
	}
}

void GameEngine::RenderManager::Render_End(LPDIRECT3DDEVICE9 _device)
{
	_device->EndScene();
	_device->Present(NULL, NULL, NULL, NULL);
}

void GameEngine::RenderManager::Add_Renderer(Renderer* _renderer)
{
	std::pair<LPDIRECT3DVERTEXBUFFER9, LPDIRECT3DINDEXBUFFER9> buffer;

	if (dynamic_cast<CubeRenderer*>(_renderer))
	{
		if (m_BufferMap.find(CUBE) == m_BufferMap.end())
		{
			_renderer->Ready_Buffer(m_Device);
			_renderer->Get_Buffer(buffer.first, buffer.second);
			buffer.first->AddRef();
			buffer.second->AddRef();

			m_BufferMap.insert({ CUBE, buffer });
		}
		else
		{
			buffer = (m_BufferMap.find(CUBE))->second;
			_renderer->Set_Buffer(buffer.first, buffer.second);
		}
	}

	else if (dynamic_cast<TextureRenderer*>(_renderer))
	{
		if (m_BufferMap.find(TEXTURE) == m_BufferMap.end())
		{
			_renderer->Ready_Buffer(m_Device);
			_renderer->Get_Buffer(buffer.first, buffer.second);
			buffer.first->AddRef();
			buffer.second->AddRef();

			m_BufferMap.insert({ TEXTURE, buffer });
		}
		else
		{
			buffer = (m_BufferMap.find(TEXTURE))->second;
			_renderer->Set_Buffer(buffer.first, buffer.second);
		}
	}

	m_RegisterQueue.push_back(_renderer);
}

void GameEngine::RenderManager::Add_Texture(const std::wstring& _path)
{
	if (m_TextureMap.find(_path) != m_TextureMap.end())
		return;

	LPDIRECT3DTEXTURE9 texture = nullptr;
	if (E_FAIL != D3DXCreateTextureFromFile(m_Device, _path.c_str(), &texture))
	{
		m_TextureMap.insert({ _path, texture });
	}
}

void GameEngine::RenderManager::Add_PixelShader(const std::wstring& _name, const std::wstring& _path)
{
	LPD3DXBUFFER shaderBuffer = nullptr;
	LPDIRECT3DPIXELSHADER9 pixelShader = nullptr;

	if (E_FAIL != D3DXCompileShaderFromFile(_path.c_str(), nullptr, nullptr, "main", "ps_2_0", 0, &shaderBuffer, nullptr, nullptr))
	{
		m_Device->CreatePixelShader((DWORD*)shaderBuffer->GetBufferPointer(), &pixelShader);
		m_PixelShaderMap.insert({ _name, pixelShader });
	}
}

void GameEngine::RenderManager::Remove_Renderer(Renderer* _renderer)
{
	for (auto& renderer : m_DestroyQueue)
	{
		if (renderer == _renderer)
		{
			return;
		}
	}

	m_DestroyQueue.push_back(_renderer);
}

void GameEngine::RenderManager::Register_Renderer()
{
	for (auto it = m_RegisterQueue.begin(); it != m_RegisterQueue.end();)
	{
		Renderer* renderer = *it;

		if (renderer->Is_Enabled())
		{
			m_Renderers.push_back(renderer);

			it = m_RegisterQueue.erase(it);
		}

		else ++it;
	}
}

void GameEngine::RenderManager::Destroy_Renderer()
{
	for (auto& renderer : m_DestroyQueue)
	{
		GameObjectList* objList = Scene::GetInstance().Get_GameObjectList();
		for (auto& gameObject : *objList)
		{
			if (gameObject == renderer->Get_GameObject())
			{
				gameObject->Remove_Component(renderer);
				break;
			}
		}

		delete renderer;
		m_Renderers.erase(std::remove(m_Renderers.begin(), m_Renderers.end(), renderer), m_Renderers.end());
	}

	m_DestroyQueue.clear();
}

void GameEngine::RenderManager::Release()
{
	for (const auto& renderer : m_Renderers)
	{
		delete renderer;
	}

	for (const auto& renderer : m_RegisterQueue)
	{
		delete renderer;
	}

	for (const auto& buffer : m_BufferMap)
	{
		buffer.second.first->Release();
		buffer.second.second->Release();
	}

	for (const auto& texture : m_TextureMap)
	{
		texture.second->Release();
	}

	for (const auto& pixelShader : m_PixelShaderMap)
	{
		pixelShader.second->Release();
	}

	m_Renderers.clear();
	m_RegisterQueue.clear();
	m_DestroyQueue.clear();

	m_BufferMap.clear();
	m_TextureMap.clear();
	m_PixelShaderMap.clear();
}

LPDIRECT3DTEXTURE9* GameEngine::RenderManager::Get_Texture(const std::wstring& _path)
{
	auto iter = m_TextureMap.find(_path);

	if (iter == m_TextureMap.end())
		return nullptr;
	else
		return &(m_TextureMap.find(_path)->second);
}

LPDIRECT3DPIXELSHADER9& GameEngine::RenderManager::Get_PixelShader(const std::wstring& _name)
{
	return m_PixelShaderMap.find(_name)->second;
}
