#include "TrailRenderer.h"

#include "Camera.h"
#include "CameraManager.h"
#include "TimeManager.h"
#include "Transform.h"

void GameEngine::TrailRenderer::Update_Trail()
{
	for (auto it = m_Points.begin(); it != m_Points.end();)
	{
		if ((m_CurrentTime - it->TimeCreated) > m_TrailTime)
		{
			it = m_Points.erase(it);
		}

		else
		{
			++it;
		}
	}

	if (m_Points.empty())
	{
		TrailPoint newPoint{ Get_Transform().Position(), m_CurrentTime };
		m_Points.push_back(newPoint);
	}

	else
	{
		Vector3 lastPos = m_Points.back().Position;
		float dist = Vector3::Distance(Get_Transform().Position(), lastPos);
		if (dist >= m_MinVertexDistance)
		{
			TrailPoint newPoint{ Get_Transform().Position(), m_CurrentTime };
			m_Points.push_back(newPoint);
		}
	}

	m_CurrentTime += TimeManager::GetInstance().Get_DeltaTime();
}

void GameEngine::TrailRenderer::Build_Trail(LPDIRECT3DDEVICE9 _device)
{
	if (m_Points.size() < 2) return;

	int segmentCount = static_cast<int>(m_Points.size()) - 1;
	m_VertexCount = segmentCount * 4;

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = nullptr;
	}

	HRESULT hr = _device->CreateVertexBuffer(
		m_VertexCount * sizeof(RibbonVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		FVF_RIBBON,
		D3DPOOL_DEFAULT,
		&m_VertexBuffer,
		nullptr
	);
	if (FAILED(hr))
	{
		return;
	}

	RibbonVertex* vertices = nullptr;
	hr = m_VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	if (FAILED(hr))
	{
		return;
	}

	for (int i = 0; i < segmentCount; ++i)
	{
		const TrailPoint& p0 = m_Points[i];
		const TrailPoint& p1 = m_Points[i + 1];

		Vector3 midPos = (p0.Position + p1.Position) * 0.5f;

		Vector3 toCam = midPos - CameraManager::GetInstance().Get_CurrentCamera()->Get_Transform().Position();
		D3DXVec3Normalize(&toCam, &toCam);

		Vector3 segDir = Vector3::Direction(p0.Position, p1.Position);

		//Vector3 worldUp{ 0,1,0 };
		Vector3 side;
		D3DXVec3Cross(&side, &segDir, &toCam);
		D3DXVec3Normalize(&side, &side);

		float lifeRatio0 = 1.0f - ((m_CurrentTime - p0.TimeCreated) / m_TrailTime);
		float lifeRatio1 = 1.0f - ((m_CurrentTime - p1.TimeCreated) / m_TrailTime);
		lifeRatio0 = (std::max)(0.0f, (std::min)(1.0f, lifeRatio0));
		lifeRatio1 = (std::max)(0.0f, (std::min)(1.0f, lifeRatio1));

		float halfW0 = (m_Width * 0.5f) * lifeRatio0;
		float halfW1 = (m_Width * 0.5f) * lifeRatio1;

		D3DXCOLOR colorStart = m_Color;
		D3DXCOLOR colorEnd = m_Color;
		colorEnd.a = 0.f;

		D3DXCOLOR color0 = colorStart * lifeRatio0 + colorEnd * (1.0f - lifeRatio0);
		D3DXCOLOR color1 = colorStart * lifeRatio1 + colorEnd * (1.0f - lifeRatio1);

		DWORD dwordColor0 = D3DCOLOR_COLORVALUE(color0.r, color0.g, color0.b, color0.a);
		DWORD dwordColor1 = D3DCOLOR_COLORVALUE(color1.r, color1.g, color1.b, color1.a);

		Vector3 left0 = p0.Position - side * halfW0;
		Vector3 right0 = p0.Position + side * halfW0;
		Vector3 left1 = p1.Position - side * halfW1;
		Vector3 right1 = p1.Position + side * halfW1;

		int baseIdx = i * 4;
		vertices[baseIdx + 0].Position = left0;
		vertices[baseIdx + 0].Color = dwordColor0;

		vertices[baseIdx + 1].Position = right0;
		vertices[baseIdx + 1].Color = dwordColor0;

		vertices[baseIdx + 2].Position = left1;
		vertices[baseIdx + 2].Color = dwordColor1;

		vertices[baseIdx + 3].Position = right1;
		vertices[baseIdx + 3].Color = dwordColor1;
	}

	m_VertexBuffer->Unlock();
}

void GameEngine::TrailRenderer::Clear_Trail()
{
	m_Points.clear();
}

void GameEngine::TrailRenderer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
}

void GameEngine::TrailRenderer::Render(LPDIRECT3DDEVICE9 _device)
{
	Update_Trail();
	Build_Trail(_device);

	if (!m_VertexBuffer || m_VertexCount < 4) return;
	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);
	_device->SetTransform(D3DTS_WORLD, &worldMat);
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetTexture(0, nullptr);
	//_device->SetIndices(nullptr);
	_device->SetStreamSource(0, m_VertexBuffer, 0, sizeof(RibbonVertex));
	_device->SetFVF(FVF_RIBBON);

	int segmentCount = (m_VertexCount / 4);
	for (int i = 0; i < segmentCount; ++i)
	{
		int startVertex = i * 4;
		_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, startVertex, 2);
	}

	_device->SetRenderState(D3DRS_LIGHTING, true);
}
