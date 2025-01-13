#include "TextureUI.h"

#include "InputManager.h"
#include "RenderManager.h"
#include "Transform.h"
#include "UIManager.h"


GameEngine::TextureUI::TextureUI(const TextureUI& _rhs)
	: UI(_rhs), m_VertexBuffer(nullptr), m_Texture(nullptr), m_bFlipX(false), m_bFlipY(false)
{
}

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
    Set_Texture(m_Path);
    m_VertexBuffer = UIManager::GetInstance().Get_VertexBuffer();
    m_VertexBuffer->AddRef();
}

bool GameEngine::TextureUI::Set_Texture(const std::wstring& _path)
{
    //set path는 로딩에서 다 해줘야 프레임 드랍이 없을 듯
    if (_path.empty())
    {
        return false;
    }
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
		D3DXMatrixScaling(&m_TextureScaleMatrix, static_cast<float>(imageInfo.Width), static_cast<float>(imageInfo.Height), 1.f);
    }
}

void GameEngine::TextureUI::Render_UI(LPDIRECT3DDEVICE9 _device)
{
    D3DXMATRIX worldMat = m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();

    _device->SetTransform(D3DTS_WORLD, &worldMat);

    //texture 세팅
    _device->SetTexture(0, m_Texture);
    _device->SetFVF(FVF_UITEX);
    _device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(CUSTOM_VERTEX));
    _device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);  // 두 개의 삼각형을 그림
}

bool GameEngine::TextureUI::Is_MouseHovered()
{
    // 마우스 좌표
    const Vector3 mousePos = InputManager::GetInstance().Get_MousePos();
    POINT winMousePos;
    winMousePos.x = static_cast<long>(mousePos.x - 960.f);
    winMousePos.y = static_cast<long>(mousePos.y - 540.f);

    // UI의 크기 구하기
    D3DXMATRIX mat = m_TextureScaleMatrix * Get_Transform().Get_WorldMatrix();
    Vector2 scale;
    scale.x = sqrtf(mat._11 * mat._11 + mat._12 * mat._12 + mat._13 * mat._13);
    scale.y = sqrtf(mat._21 * mat._21 + mat._22 * mat._22 + mat._23 * mat._23);
    Vector3 position = Get_Transform().Position();

    RECT rect =
    {
        static_cast<LONG>(position.x - scale.x * 0.5f),
        static_cast<LONG>(position.y - scale.y * 0.5f),
        static_cast<LONG>(position.x + scale.x * 0.5f),
        static_cast<LONG>(position.y + scale.y * 0.5f)
    };

    return PtInRect(&rect, winMousePos);
}

bool GameEngine::TextureUI::Is_ButtonDown()
{
    return Is_MouseHovered() && InputManager::GetInstance().Key_Down(VK_LBUTTON);
}

bool GameEngine::TextureUI::Is_ButtonHold()
{
    return Is_MouseHovered() && InputManager::GetInstance().Key_Pressing(VK_LBUTTON);
}

bool GameEngine::TextureUI::Is_ButtonUp()
{
    return Is_MouseHovered() && InputManager::GetInstance().Key_Up(VK_LBUTTON);
}
