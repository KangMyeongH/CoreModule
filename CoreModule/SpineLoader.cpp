#include "SpineLoader.h"

void spine::SpineLoader::load(AtlasPage& page, const String& path)
{
    // IDirect3DTexture9* 생성
    IDirect3DTexture9* d3dTexture = nullptr;

    // D3DXCreateTextureFromFileEx 등을 통해 파일 로드
    HRESULT hr = D3DXCreateTextureFromFileExA(
        m_Device,
        path.buffer(),
        D3DX_DEFAULT,           // Width
        D3DX_DEFAULT,           // Height
        D3DX_DEFAULT,           // MipLevels
        0,                      // Usage
        D3DFMT_UNKNOWN,         // Format
        D3DPOOL_MANAGED,        // Pool
        D3DX_DEFAULT,           // Filter
        D3DX_DEFAULT,           // MipFilter
        0,                      // ColorKey
        nullptr,                // pSrcInfo
        nullptr,                // pPalette
        &d3dTexture
    );

    if (FAILED(hr) || !d3dTexture) {
        // 로드 실패 시 적절한 예외 처리
        return;
    }

    // 텍스처 크기 알아내기
    D3DSURFACE_DESC desc;
    d3dTexture->GetLevelDesc(0, &desc);

    page.width = desc.Width;
    page.height = desc.Height;

    // Spine에서 TextureFilter_Linear 등을 확인하여 setFilter
    // ( DirectX9에서는 보통 샘플러 상태 설정으로 처리 )
    // if (page.magFilter == TextureFilter_Linear) { ... }

    // 래핑 모드(Repeat 등)는 DirectX9에서 TextureStageState/D3DSAMPLERSTATE로 설정
    // if (page.uWrap == TextureWrap_Repeat && page.vWrap == TextureWrap_Repeat) { ... }

    // 최종적으로 page에 저장
    page.setRendererObject((void*)d3dTexture);
}

void spine::SpineLoader::unload(void* texture)
{
    if (!texture) return;
    IDirect3DTexture9* d3dTexture = static_cast<IDirect3DTexture9*>(texture);
    d3dTexture->Release(); // DirectX9 Texture 해제
}
