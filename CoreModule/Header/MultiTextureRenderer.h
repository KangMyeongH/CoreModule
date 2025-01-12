#pragma once

#include <regex>

#include "Renderer.h"
#include "RenderManager.h"
#include "TextureRenderer.h"

class Shader;

namespace GameEngine
{
	class COREMODULE_API MultiTextureRenderer : public Renderer
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		MultiTextureRenderer() : Renderer(nullptr),
			m_Frame(0),
			m_Texture(nullptr),
			m_FlipX(false), m_FlipY(false)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);

		}
		explicit MultiTextureRenderer(GameObject* _owner) : Renderer(_owner),
			m_Frame(0),
			m_Texture(nullptr),
			m_FlipX(false), m_FlipY(false)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);

		}
		MultiTextureRenderer(const MultiTextureRenderer& _rhs) : Renderer(_rhs),
			m_Frame(0),
			m_Texture(nullptr),
			m_FlipX(_rhs.m_FlipX), m_FlipY(_rhs.m_FlipY)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);

		}
		~MultiTextureRenderer() override = default;

		//======================================//
		//				 method					//
		//======================================//

	public:
		//vertex buffer 및 index buffer 생성
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;

		//화면에 출력
		void Render(LPDIRECT3DDEVICE9 _device) override;

		void Compute_FlipMat(D3DXMATRIX& _mat) const;

		bool Set_Frame(const std::wstring& _path, int _frame);

		void Set_NativeSize();
		void Set_NormalSize();

		void Set_FlipX(bool _flip) { m_FlipX = _flip; }
		void Set_FlipY(bool _flip) { m_FlipY = _flip; }

		bool Get_FlipX() const { return m_FlipX; }
		bool Get_FlipY() const { return m_FlipY; }

		//Texture 초기화, 실패 시 0 반환, 성공 시 불러들인 파일 수를 반환
		//static 함수라서 loading에서도 호출 가능
		int Ready_Texture(const std::wstring& _path)
		{
			return RenderManager::GetInstance().Add_MultiTexture(_path);
		}

		std::wstring Get_Path() const { return m_Path; }
		void Set_Path(const std::wstring& _path) { m_Path = _path; }

		Component* Clone() const override
		{
			return new MultiTextureRenderer(*this);
		}

		//======================================//
		//				 serialize				//
		//======================================//
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "MultiTextureRenderer";
			_j = nlohmann::ordered_json{
				{"type", type},
				{"enable", m_bEnabled},
				{"path", m_Path},
				{"frame", m_Frame}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{
			_j.at("enable").get_to(m_bEnabled);
			_j.at("path").get_to(m_Path);
			_j.at("frame").get_to(m_Frame);
		}

	private:
		std::wstring m_Path; //폴더 경로
		int m_Frame;
		std::vector<LPDIRECT3DTEXTURE9>* m_Texture;
		D3DXMATRIX					m_TextureScaleMatrix;

		bool						m_FlipX;
		bool						m_FlipY;
	};

	REGISTER_COMPONENT(MultiTextureRenderer)
}