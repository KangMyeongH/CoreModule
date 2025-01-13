#include "SpineUI.h"

#include "SpineLoader.h"
#include "TimeManager.h"
#include "Transform.h"

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

template<typename T, typename... Args>
std::unique_ptr<T> make_unique_test(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

GameEngine::SpineUI::SpineUI(): UI(nullptr), m_Loader(nullptr), m_Skeleton(nullptr), m_State(nullptr),
                                m_OwnsAnimationStateData(false), m_UsePMA(false),
                                m_TimeScale(1.0f),
                                m_FlipX(false),
                                m_FlipY(false)
{
    m_Option = ALPHA_BLENDING;
}

GameEngine::SpineUI::SpineUI(GameObject* _owner, const std::string& _path, const std::string& _skin,
    const std::string& _animation) : UI(_owner), m_Loader(nullptr), m_Skeleton(nullptr), m_State(nullptr),
    m_OwnsAnimationStateData(false),
    m_UsePMA(false),
    m_TimeScale(1.0f), m_Path(_path),
    m_CurrentSkin(_skin.c_str()),
    m_CurrentAnimation(_animation.c_str()),
    m_FlipX(false),
    m_FlipY(false)
{
    m_Option = ALPHA_BLENDING;
}

GameEngine::SpineUI::SpineUI(GameObject* _owner): UI(_owner), m_Loader(nullptr), m_Skeleton(nullptr), m_State(nullptr),
                                                  m_OwnsAnimationStateData(false), m_UsePMA(false),
                                                  m_TimeScale(1.0f),
                                                  m_FlipX(false), m_FlipY(false)
{
    m_Option = ALPHA_BLENDING;
}

GameEngine::SpineUI::SpineUI(const SpineUI& _rhs)
	: UI(_rhs), m_Loader(nullptr), m_Skeleton(nullptr), m_State(nullptr), m_OwnsAnimationStateData(false),
	  m_UsePMA(false), m_TimeScale(1.0f),
	  m_FlipX(false),
	  m_FlipY(false)
{
    m_Option = ALPHA_BLENDING;
}

GameEngine::SpineUI::~SpineUI()
{
    if (m_OwnsAnimationStateData) delete m_State->getData();
    delete m_State;
    delete m_Skeleton;

    m_Atlas.reset();
    delete m_Loader;
}

void GameEngine::SpineUI::Ready_UI(LPDIRECT3DDEVICE9 _device)
{
    if (m_Path.empty())
    {
        return;
    }

    if (m_State)
    {
        return;
    }

    std::string skelPath = m_Path;
    size_t pos = skelPath.find_last_of('.');
    if (pos != std::string::npos)
    {
        skelPath.replace(pos, std::string::npos, ".skel");
    }

    m_Loader = new spine::SpineLoader(_device);

    m_Atlas = make_unique_test<spine::Atlas>(m_Path.c_str(), m_Loader);

    m_SkeletonData = readSkeletonBinaryData(skelPath, m_Atlas.get());

    // 3) AnimationStateData 생성
    spine::AnimationStateData* stateData = new (__FILE__, __LINE__) spine::AnimationStateData(m_SkeletonData.get());

    // 필요한 만큼 버퍼 확보
    m_worldVertices.ensureCapacity(SPINE_MESH_VERTEX_COUNT_MAX);

    // Skeleton 생성
    m_Skeleton = new(__FILE__, __LINE__) spine::Skeleton(m_SkeletonData.get());

    // stateData가 없으면 새로 생성
    m_OwnsAnimationStateData = (stateData == nullptr);
    if (m_OwnsAnimationStateData) {
        stateData = new(__FILE__, __LINE__) spine::AnimationStateData(m_SkeletonData.get());
    }

    // AnimationState 생성
    m_State = new(__FILE__, __LINE__) spine::AnimationState(stateData);

    // RegionAttachment용 사각형 인덱스
    m_quadIndices.add(0);
    m_quadIndices.add(1);
    m_quadIndices.add(2);
    m_quadIndices.add(2);
    m_quadIndices.add(3);
    m_quadIndices.add(0);

    // 스킨 목록 저장
    spine::Vector<spine::Skin*>& skins = m_SkeletonData->getSkins();
    for (size_t i = 0; i < skins.size(); i++)
    {
        spine::Skin* skin = skins[i];
        m_Skins.emplace_back(skin->getName().buffer());
    }

    spine::Vector<spine::Animation*>& animations = m_SkeletonData->getAnimations();
    for (size_t i = 0; i < animations.size(); i++)
    {
        spine::Animation* animation = animations[i];
        m_Animations.emplace_back(animation->getName().buffer());
    }

    if (!m_CurrentSkin.isEmpty())
    {
        Change_Skin(m_CurrentSkin.buffer());
    }

    if (!m_CurrentAnimation.isEmpty())
    {
        Change_Animation(m_CurrentAnimation.buffer(), true);
    }
}

void GameEngine::SpineUI::Update_Animation(float _deltaTime)
{
    m_Skeleton->setScaleX(m_FlipX ? -1 : 1);
    m_Skeleton->setScaleY(m_FlipY ? -1 : 1);

    m_Skeleton->update(_deltaTime);

    // AnimationState 시간 업데이트
    m_State->update(_deltaTime * m_TimeScale);

    // Skeleton에 AnimationState 적용
    m_State->apply(*m_Skeleton);

    // 월드 변환 업데이트
    m_Skeleton->updateWorldTransform();
}

void GameEngine::SpineUI::Render_UI(LPDIRECT3DDEVICE9 _device)
{
    if (m_Path.empty())
    {
        return;
    }
    // Animation Update 해줘야함.
    Update_Animation(TimeManager::GetInstance().Get_DeltaTime());

    D3DXMATRIX scaleMat;
    D3DXMatrixIdentity(&scaleMat);
    D3DXMatrixScaling(&scaleMat, 1.f, 1.f, 1.f);
    D3DXMATRIX worldMat = scaleMat * Get_Transform().Get_WorldMatrix();

    _device->SetTransform(D3DTS_WORLD, &worldMat);

    _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (!_device || !m_Skeleton) return;

    // 전체 알파가 0이면 그릴 필요 없음
    if (m_Skeleton->getColor().a == 0) return;

    // 클리퍼 초기화
    m_clipper.clipEnd();

    // 디바이스에서 Render State 설정(예: ZWriteDisable, AlphaBlendEnable 등)
    _device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    // _device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
     // 필요에 따라 블렌드 모드(Normal, Additive 등) → D3DRS_SRCBLEND, D3DRS_DESTBLEND 설정

     // 슬롯 순회
    for (unsigned i = 0; i < m_Skeleton->getSlots().size(); ++i) {
        spine::Slot& slot = *m_Skeleton->getDrawOrder()[i];
        spine::Attachment* attachment = slot.getAttachment();
        if (!attachment) {
            m_clipper.clipEnd(slot);
            continue;
        }

        // 슬롯 색상 알파가 0이거나 본이 비활성화면 스킵
        if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
            m_clipper.clipEnd(slot);
            continue;
        }

        // 정점, UV, 인덱스
        spine::Vector<float>* vertices = &m_worldVertices;
        int verticesCount = 0;
        spine::Vector<float>* uvs = nullptr;
        spine::Vector<unsigned short>* indices = nullptr;
        int indicesCount = 0;

        // Attachment 별색상
        spine::Color* attachmentColor = nullptr;

        // 텍스처 (DirectX9)
        IDirect3DTexture9* currentTexture = nullptr;

        // RegionAttachment 처리
        if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
            spine::RegionAttachment* region = (spine::RegionAttachment*)attachment;
            attachmentColor = &region->getColor();

            if (attachmentColor->a == 0) {
                m_clipper.clipEnd(slot);
                continue;
            }

            // 4개 정점
            m_worldVertices.setSize(8, 0);
            region->computeWorldVertices(slot.getBone(), m_worldVertices, 0, 2);
            verticesCount = 4;
            uvs = &region->getUVs();
            indices = &m_quadIndices;
            indicesCount = 6;

            // DX9 텍스처 가져오기
            spine::AtlasRegion* atlasRegion = (spine::AtlasRegion*)region->getRendererObject();
            currentTexture = (IDirect3DTexture9*)atlasRegion->page->getRendererObject();

        }
        else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
            // MeshAttachment
            spine::MeshAttachment* mesh = (spine::MeshAttachment*)attachment;
            attachmentColor = &mesh->getColor();

            if (attachmentColor->a == 0) {
                m_clipper.clipEnd(slot);
                continue;
            }

            // 메시 정점
            m_worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
            mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), m_worldVertices, 0, 2);
            verticesCount = mesh->getWorldVerticesLength() >> 1;
            uvs = &mesh->getUVs();
            indices = &mesh->getTriangles();
            indicesCount = mesh->getTriangles().size();

            // DX9 텍스처
            spine::AtlasRegion* atlasRegion = (spine::AtlasRegion*)mesh->getRendererObject();
            currentTexture = (IDirect3DTexture9*)atlasRegion->page->getRendererObject();

        }
        else if (attachment->getRTTI().isExactly(spine::ClippingAttachment::rtti)) {
            // ClippingAttachment
            spine::ClippingAttachment* clip = (spine::ClippingAttachment*)attachment;
            m_clipper.clipStart(slot, clip);
            continue;
        }
        else {
            // 그 외 (BoundingBoxAttachment 등) - 여기서는 스킵
            m_clipper.clipEnd(slot);
            continue;
        }

        // Skeleton/Slot/Attachment 색상 곱
        float alpha = m_Skeleton->getColor().a
            * slot.getColor().a
            * attachmentColor->a;

        // 만약 클리핑 중이면 삼각형을 자르기
        if (m_clipper.isClipping()) {
            m_clipper.clipTriangles(*vertices, *indices, *uvs, 2);
            vertices = &m_clipper.getClippedVertices();
            verticesCount = m_clipper.getClippedVertices().size() >> 1;
            uvs = &m_clipper.getClippedUVs();
            indices = &m_clipper.getClippedTriangles();
            indicesCount = m_clipper.getClippedTriangles().size();
        }

        // 실제 DirectX9로 그리기 위해 사용할 임시 정점 배열
        // (x, y, z=0, rhw=1, diffuse color, u, v)
        struct SpineVertex {
            float x, y, z;
            float nx, ny, nz;
            D3DCOLOR color;
            float u, v;
        };

        // 삼각형 개수 * 3개의 정점
        // → indicesCount가 실제 인덱스 개수
        std::vector<SpineVertex> drawVerts;
        drawVerts.reserve(indicesCount);

        // 컬러 계산 (premultiplied alpha라면 r,g,b *= alpha)
        float r = m_Skeleton->getColor().r
            * slot.getColor().r
            * attachmentColor->r;
        float g = m_Skeleton->getColor().g
            * slot.getColor().g
            * attachmentColor->g;
        float b = m_Skeleton->getColor().b
            * slot.getColor().b
            * attachmentColor->b;

        if (m_UsePMA) {
            r *= alpha; g *= alpha; b *= alpha;
        }

        // 0~1 범위를 0xFF로 변환
        auto toByte = [](float c) {
            int val = (int)(c * 255.0f);
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            return (BYTE)val;
            };

        DWORD diffuseColor = D3DCOLOR_ARGB(
            toByte(alpha),
            toByte(r),
            toByte(g),
            toByte(b)
        );

        // 슬롯의 BlendMode 읽어오기
        spine::BlendMode spineBlend = slot.getData().getBlendMode();

        // 1) DirectX9 블렌드 설정 (슬롯마다 변경 가능)
        switch (spineBlend) {
        case spine::BlendMode_Normal:
            // Normal: SrcAlpha, InvSrcAlpha
            _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case spine::BlendMode_Additive:
            // Additive: SrcAlpha, One
            _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        case spine::BlendMode_Multiply:
            // Multiply(일반적인 D3D9 트릭)
            //   - 하나의 방법: D3DBLEND_DESTCOLOR, D3DBLEND_INVSRCALPHA
            _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
            _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case spine::BlendMode_Screen:
            // Screen(1 - (1 - srcColor)*(1 - dstColor))
            //   - Screen을 정확히 구현하기 어렵지만, 근사치: D3DBLEND_ONE, D3DBLEND_INVSRCCOLOR
            //   - 실제로 여러 기법이 있으니 프로젝트별로 조정
            _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
            _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
            break;
        default:
            // 그 외에는 Normal과 동일하게
            _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        }

        // 인덱스 순서대로 정점을 삼각형 목록에 추가
        for (int ii = 0; ii < indicesCount; ++ii) {
            int idx = (*indices)[ii] << 1;
            float vx = (*vertices)[idx + 0];
            float vy = (*vertices)[idx + 1];
            float tu = (*uvs)[idx + 0];
            float tv = (*uvs)[idx + 1];

            // Spine 좌표계 기준으로 변환
            // (DirectX9 뷰포트와 y축 반전, 오프셋 등 필요하다면 여기서 처리)
            SpineVertex sv;
            sv.x = vx;
            sv.y = vy;
            sv.z = 0.0f;
            sv.nx = 0.0f;
            sv.ny = 0.0f;
            sv.nz = 1.f;
            sv.color = diffuseColor;
            sv.u = tu;
            sv.v = tv;

            drawVerts.push_back(sv);
        }

        // 텍스처 설정
        _device->SetTexture(0, currentTexture);

        // 블렌드 모드 매핑 예시 (slot.getData().getBlendMode())
        //  - Normal: (SRCALPHA, INVSRCALPHA)
        //  - Additive: (SRCALPHA, ONE)
        //  - Multiply, Screen 등은 다양하게 설정 가능
        // 실제 프로젝트 상황에 맞춰 setRenderState로 처리하세요.

        // 정점 포맷(FVF)
        _device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

        // DrawPrimitiveUP을 사용해 간단히 그리기 (삼각형 리스트)
        _device->DrawPrimitiveUP(
            D3DPT_TRIANGLELIST,
            drawVerts.size() / 3, // 삼각형 개수
            drawVerts.data(),
            sizeof(SpineVertex)
        );

        // 클리핑 종료
        m_clipper.clipEnd(slot);
    }

    // 그리기 끝, 필요하면 RenderState 원복
    //_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
    //_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    // 클리퍼 완전 종료
    m_clipper.clipEnd();
}

void GameEngine::SpineUI::Change_Skin(const std::string& _skin)
{
    m_Skeleton->setSkin(_skin.c_str());
    m_Skeleton->setSlotsToSetupPose();
    m_Skeleton->updateWorldTransform();
}

void GameEngine::SpineUI::Change_Animation(const std::string& _animation, bool _isLoop)
{
    m_State->clearTrack(1);
    m_State->setAnimation(0, _animation.c_str(), _isLoop);
}

void GameEngine::SpineUI::Change_Animation(const std::string& _track1, const std::string& _track2, bool _isLoop)
{
    m_State->setAnimation(0, _track1.c_str(), _isLoop);
    m_State->setAnimation(1, _track2.c_str(), _isLoop);
}

bool GameEngine::SpineUI::Is_Finished(int _track)
{
    spine::TrackEntry* current = m_State->getCurrent(_track);
    if (current && current->isComplete())
    {
        return true;
    }

    return false;
}

void GameEngine::SpineUI::to_json(nlohmann::ordered_json& _j)
{
    std::string type = "SpineRenderer";
    _j = nlohmann::ordered_json
    {
        {"type", type},
        {"enable", m_bEnabled},
        {"path", m_Path},
        {"flipX", m_FlipX},
        {"flipY", m_FlipY}
    };
}

void GameEngine::SpineUI::from_json(const nlohmann::ordered_json& _j)
{
    if (_j.contains("enable"))
    {
        _j.at("enable").get_to(m_bEnabled);
    }
    if (_j.contains("path"))
    {
        _j.at("path").get_to(m_Path);
    }
    if (_j.contains("flipX"))
    {
        _j.at("flipX").get_to(m_FlipX);
    }
    if (_j.contains("flipY"))
    {
        _j.at("flipY").get_to(m_FlipY);
    }
}

GameEngine::Component* GameEngine::SpineUI::Clone() const
{
    return new SpineUI(*this);
}

std::shared_ptr<spine::SkeletonData> GameEngine::SpineUI::readSkeletonBinaryData(const std::string& _path,
	spine::Atlas* _atlas)
{
    spine::SkeletonBinary binary(_atlas);
    auto skeletonData = binary.readSkeletonDataFile(_path.c_str());
    if (!skeletonData)
    {
        return nullptr;
    }

    return std::shared_ptr<spine::SkeletonData>(skeletonData);
}

bool GameEngine::SpineUI::Is_MouseHovered()
{
    return false;
}

bool GameEngine::SpineUI::Is_ButtonDown()
{
    return false;
}

bool GameEngine::SpineUI::Is_ButtonHold()
{
    return false;
}

bool GameEngine::SpineUI::Is_ButtonUp()
{
    return false;
}