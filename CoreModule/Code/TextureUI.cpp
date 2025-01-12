#include "TextureUI.h"

#include "RenderManager.h"
#include "Transform.h"

GameEngine::TextureUI::TextureUI(GameEngine::GameObject* _owner)
	: UI(_owner), m_pVertexBuffer(nullptr)
{
}

GameEngine::TextureUI::~TextureUI()
{
}

void GameEngine::TextureUI::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
    GameEngine::CUSTOM_VERTEX vertices[] = 
    {
        // 첫 번째 삼각형 (왼쪽 하단, 왼쪽 상단, 오른쪽 하단)
        { D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DCOLOR_XRGB(255, 0, 0) }, // 왼쪽 하단 (빨간색)
        { D3DXVECTOR3(-0.5f,  0.5f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) }, // 왼쪽 상단 (초록색)
        { D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DCOLOR_XRGB(0, 0, 255) }, // 오른쪽 하단 (파란색)

        // 두 번째 삼각형 (오른쪽 하단, 왼쪽 상단, 오른쪽 상단)
        { D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DCOLOR_XRGB(0, 0, 255) }, // 오른쪽 하단 (파란색)
        { D3DXVECTOR3(-0.5f,  0.5f, 0.0f), D3DCOLOR_XRGB(0, 255, 0) }, // 왼쪽 상단 (초록색)
        { D3DXVECTOR3(0.5f,  0.5f, 0.0f), D3DCOLOR_XRGB(255, 255, 0) }  // 오른쪽 상단 (노란색)
    };

    // 정점 버퍼 생성
    HRESULT hr = _device->CreateVertexBuffer(
        sizeof(vertices),              // 버퍼 크기
        D3DUSAGE_WRITEONLY,            // 쓰기 전용
        GameEngine::FVF_UITEX,         // 정점 형식
        D3DPOOL_MANAGED,               // 메모리 풀
        &m_pVertexBuffer,                // 버텍스 버퍼 포인터
        NULL                           // 반환할 물리적 메모리 포인터 없음
    );


    // 버텍스 버퍼에 데이터 복사
    VOID*   pVoid;  // 데이터를 쓸 버퍼 포인터
    m_pVertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    m_pVertexBuffer->Unlock();
}

void GameEngine::TextureUI::Render_Buffer(LPDIRECT3DDEVICE9 _device)
{
    _device->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(GameEngine::FVF_UITEX));
    _device->SetFVF(GameEngine::FVF_UITEX);
    _device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);  // 두 개의 삼각형을 그림
}

bool GameEngine::TextureUI::Set_Texture(std::wstring _path)
{
    //set path는 로딩에서 다 해줘야 프레임 드랍이 없을 듯
    m_Path = _path;
    RenderManager::GetInstance().Add_Texture(_path);
    m_Texture = *(RenderManager::GetInstance().Get_Texture(_path));
    
    if (m_Texture) return true;
    else return false;
}


//void TextureUI::Set_NativeSize()
//{
//	if (!m_Texture)  // 근데 이전에 머ㅜ 해줘야됨
//		return;
//
//	D3DXIMAGE_INFO imageInfo;
//	D3DXGetImageInfoFromFile(m_Path.c_str(), &imageInfo);
//	D3DXMatrixScaling(&m_TextureScaleMatrix, static_cast<float>(imageInfo.Width) / 100.f, static_cast<float>(imageInfo.Height) / 100.f, 1.f);
//}
//
//void TextureUI::Set_NormalSize()
//{
//	D3DXMatrixScaling(&m_TextureScaleMatrix, 1.f, 1.f, 1.f);
//}


void GameEngine::TextureUI::Update_Texture(LPDIRECT3DDEVICE9 _device)
{
    m_SizeX = 1920;
    m_SizeY = 1080;
    m_X = m_SizeX * 0.5f;
    m_Y = m_SizeY * 0.5f;

    D3DXMatrixOrthoLH(&m_projMatrix, m_SizeX, m_SizeY, 0.f, 0.1f);

    GameEngine::Vector3   scale;
    scale.x = m_SizeX * 0.5f;
    scale.y = m_SizeY * 0.5f;
    scale.z = 0.f;
    Get_Transform().Set_LocalScale(scale);

    Get_Transform().Set_LocalPosition(Vector3(m_X - m_SizeX * 0.5f, m_Y - m_SizeY * 0.5f, 0.f));
}

void GameEngine::TextureUI::Render_Texture(LPDIRECT3DDEVICE9 _device)
{
    D3DXMATRIX	viewMatrix;
    D3DXMatrixIdentity(&viewMatrix);
    _device->SetTransform(D3DTS_WORLD, &(Get_Transform().Get_WorldMatrix()));
    _device->SetTransform(D3DTS_VIEW, &viewMatrix);
    _device->SetTransform(D3DTS_PROJECTION, &m_projMatrix);

    ////cullmode 변경을 매 renderer마다 껐다 키면 부하가 심함. 같은 애들끼리 묶어서 출력하게 변경해야 함
    _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    //texture 세팅

    _device->SetTexture(0, m_Texture);

    Render_Buffer(_device);

    _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}
