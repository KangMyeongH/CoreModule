#include "RenderManager.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextureRenderer.h"
#include "Light.h"
#include "Material.h"
#include "SpineMaterial.h"
#include "SpineRenderer.h"

IMPLEMENT_SINGLETON(GameEngine::RenderManager)

void GameEngine::RenderManager::Initialize(LPDIRECT3DDEVICE9 _device)
{
	m_BackBufferColor = { 0.2f, 0.2f, 0.2f, 1.f };
	m_Device = _device;
	_device->AddRef();

	if (!m_DefaultMaterial)
	{
		m_DefaultMaterial = new Material(m_Device, R"(..\Client\Assets\Resource\Shader\DefaultShader.hlsl)");
	}

	if (!m_SpineMaterial)
	{
		m_SpineMaterial = new SpineMaterial(m_Device, R"(..\Client\Assets\Resource\Shader\DefaultSpineShader.hlsl)");
	}
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
			dynamic_cast<TextureRenderer*>(renderer)->Ready_Texture();
		}

		else if (dynamic_cast<SpineRenderer*>(renderer))
		{
			renderer->Ready_Buffer(_device);
			dynamic_cast<SpineRenderer*>(renderer)->Set_Material(m_SpineMaterial);
		}
	}
}

GameEngine::RenderManager::~RenderManager()
{
}

void GameEngine::RenderManager::Render_Begin(LPDIRECT3DDEVICE9 _device)
{
	_device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		m_BackBufferColor,
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

	// Light Setting
	m_Device->SetRenderState(D3DRS_LIGHTING, true);

	//임시
	if (m_GlobalLight)
	{
		m_GlobalLight->Update_Light(_device);
		m_GlobalLight->Ready_Light(_device); //test
		_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		//_device->SetRenderState(D3DRS_SPECULARENABLE, true);
	}

	m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//불투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);   // 알파 테스트 활성화
	m_Device->SetRenderState(D3DRS_ALPHAREF, 128);          // 알파값 128 기준
	m_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);     // Z 버퍼 쓰기 활성화

	for (const auto& renderer : m_Renderers[0])
	{
		if (renderer->Is_Enabled() && renderer->Get_GameObject()->Is_Active())
		{
			renderer->Render(_device);
		}
	}

	//반투명 객체 렌더
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 알파 테스트 비활성화
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // 알파 블렌딩 활성화
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);     // Z 버퍼 쓰기 비활성화

	std::sort(m_Renderers[1].begin(), m_Renderers[1].end(), [&_device](Renderer* dst, Renderer* src)->bool
		{
			D3DXMATRIX matCameraWorld;

			_device->GetTransform(D3DTS_VIEW, &matCameraWorld);
			D3DXMatrixInverse(&matCameraWorld, 0, &matCameraWorld);

			Vector3   cameraPosition;
			memcpy(&cameraPosition, &matCameraWorld.m[3][0], sizeof(Vector3));

			Vector3 dstZ = cameraPosition - dst->Get_Transform().Position();
			Vector3	srcZ = cameraPosition - src->Get_Transform().Position();

			float dstLength = D3DXVec3Length(&dstZ);
			float srcLength = D3DXVec3Length(&srcZ);

			return dstLength > srcLength;
		});

	
	for (const auto& renderer : m_Renderers[1])
	{
		if (renderer->Is_Enabled() && renderer->Get_GameObject()->Is_Active())
		{
			renderer->Render(_device);
		}
	}

	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
		dynamic_cast<TextureRenderer*>(_renderer)->Ready_Texture();
		dynamic_cast<TextureRenderer*>(_renderer)->Set_Material(m_DefaultMaterial);
	}

	else if (dynamic_cast<SpineRenderer*>(_renderer))
	{
		_renderer->Ready_Buffer(m_Device);
	}

	m_RegisterQueue.push_back(_renderer);
}

void GameEngine::RenderManager::Add_Texture(const std::wstring& _path)
{
	if (m_TextureMap.find(_path) != m_TextureMap.end())
		return;
	if (_path.empty())
		return;
	LPDIRECT3DTEXTURE9 texture = nullptr;
	if (E_FAIL != D3DXCreateTextureFromFile(m_Device, _path.c_str(), &texture))
	{
		m_TextureMap.insert({ _path, texture });
	}
}

//실패하면 0 반환, 성공하면 이미지 파일 개수 반환
int GameEngine::RenderManager::Add_MultiTexture(const std::wstring& _path)
{
	auto iter = m_MultiTextureMap.find(_path);
	if (iter != m_MultiTextureMap.end())
		return iter->second.size();

	//경로에서 이미지 파일 불러오기
	std::wstring searchPath = _path + L"/*.*"; // 모든 파일 검색
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		return 0;
	}

	LPDIRECT3DTEXTURE9 texture = nullptr;
	int fileCnt = 0;

	do {
		// 파일인지 확인
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			// 파일명 추출
			std::wstring fileName = findFileData.cFileName;
			std::wstring imagePath = _path + L"/" + fileName;
			
			// 확장자 검사
			std::wstring::size_type dotPos = fileName.find_last_of(L'.');
			if (dotPos != std::wstring::npos) {
				std::wstring extension = fileName.substr(dotPos + 1);
				if (extension == L"png" || extension == L"jpg" || extension == L"jpeg" || extension == L"bmp")
				{
					//image 파일을 찾으면
					if (E_FAIL != D3DXCreateTextureFromFile(m_Device, imagePath.c_str(), &texture))
					{
						m_MultiTextureMap[_path].push_back(texture);
						fileCnt++;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	//파일 순차적으로 안읽어오면 path vector 만들고 정렬 -> 정렬된 순서대로 texture 만들어서 삽입

	FindClose(hFind);
	return fileCnt;
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
			if (renderer->Get_RenderOption() == Renderer::ALPHA_RENDERING)
				m_Renderers[0].push_back(renderer);
			else if (renderer->Get_RenderOption() == Renderer::ALPHA_BLENDING)
				m_Renderers[1].push_back(renderer);

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
		m_Renderers[0].erase(std::remove(m_Renderers[0].begin(), m_Renderers[0].end(), renderer), m_Renderers[0].end());
		m_Renderers[1].erase(std::remove(m_Renderers[1].begin(), m_Renderers[1].end(), renderer), m_Renderers[1].end());
	}

	m_DestroyQueue.clear();
}

void GameEngine::RenderManager::Clear_Component()
{
	for (const auto& rendererList : m_Renderers)
	{
		for (const auto& renderer : rendererList)
		{
			delete renderer;
		}
	}

	for (const auto& renderer : m_RegisterQueue)
	{
		delete renderer;
	}

	m_Renderers[0].clear();
	m_Renderers[1].clear();

	m_RegisterQueue.clear();
	m_DestroyQueue.clear();
}

void GameEngine::RenderManager::Release()
{
	Clear_Component();

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

	m_BufferMap.clear();
	m_TextureMap.clear();
	m_PixelShaderMap.clear();

	if (m_Device) m_Device->Release();

	delete m_DefaultMaterial;
}

LPDIRECT3DTEXTURE9* GameEngine::RenderManager::Get_Texture(const std::wstring& _path)
{
	auto iter = m_TextureMap.find(_path);

	if (iter == m_TextureMap.end())
		return nullptr;

	return &(iter->second);
}

LPDIRECT3DPIXELSHADER9& GameEngine::RenderManager::Get_PixelShader(const std::wstring& _name)
{
	return m_PixelShaderMap.find(_name)->second;
}

std::vector<LPDIRECT3DTEXTURE9>* GameEngine::RenderManager::Get_MultiTexture(const std::wstring& _path)
{
	auto iter = m_MultiTextureMap.find(_path);

	if (iter == m_MultiTextureMap.end())
		return nullptr;
	
	return &(iter->second);
}