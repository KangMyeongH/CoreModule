#include "TextureUI.h"

#include "RenderManager.h"
#include "Transform.h"
#include "UIManager.h"


GameEngine::TextureUI::~TextureUI()
{
    if (m_VertexBuffer)
    {
        m_VertexBuffer->Release();
    }
}

void GameEngine::TextureUI::Ready_Buffer(LPDIRECT3DDEVICE9 _device, IDirect3DVertexBuffer9* _buffer)
{
    m_VertexBuffer = _buffer;
}

void GameEngine::TextureUI::Ready_UI(LPDIRECT3DDEVICE9 _device)
{
    m_VertexBuffer = UIManager::GetInstance().Get_VertexBuffer();
    m_VertexBuffer->AddRef();
}

bool GameEngine::TextureUI::Set_Texture(const std::wstring& _path)
{
    //set path는 로딩에서 다 해줘야 프레임 드랍이 없을 듯
    m_Path = _path;
    RenderManager::GetInstance().Add_Texture(_path);
    m_Texture = *(RenderManager::GetInstance().Get_Texture(_path));

    if (m_Texture)
    {
        Set_NativeSize();
        return true;
    }
    return false;
}

void GameEngine::TextureUI::Set_NativeSize()
{
    if (m_Texture)
    {
    	D3DXIMAGE_INFO imageInfo;
		D3DXGetImageInfoFromFile(m_Path.c_str(), &imageInfo);
		D3DXMatrixScaling(&m_TextureScaleMatrix, static_cast<float>(imageInfo.Width) / 100.f, static_cast<float>(imageInfo.Height) / 100.f, 1.f);
    }
}

void GameEngine::TextureUI::Render_UI(LPDIRECT3DDEVICE9 _device)
{
    D3DXMATRIX worldMat = m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();

    _device->SetTransform(D3DTS_WORLD, &worldMat);

    //texture 세팅
    _device->SetTexture(0, m_Texture);

    _device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(FVF_UITEX));
    _device->SetFVF(FVF_UITEX);
    _device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);  // 두 개의 삼각형을 그림
}