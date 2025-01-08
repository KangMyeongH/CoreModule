#pragma once
#include <regex>

#include "Renderer.h"
#include "RenderManager.h"

//임시
#include "Transform.h"

class Shader;

namespace GameEngine
{
	class COREMODULE_API TextureRenderer : public Renderer
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//
		TextureRenderer() : Renderer(nullptr),
			m_Texture(nullptr), m_FlipX(false), m_FlipY(false)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}
		explicit TextureRenderer(GameObject* _owner) : Renderer(_owner),
			m_Texture(nullptr), m_FlipX(false), m_FlipY(false)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}
		TextureRenderer(const TextureRenderer& _rhs) : Renderer(_rhs),
			m_Texture(_rhs.m_Texture), m_FlipX(_rhs.m_FlipX), m_FlipY(_rhs.m_FlipY)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}
		~TextureRenderer() override = default;

		//======================================//
		//				 method					//
		//======================================//

	public:
		//vertex buffer 및 index buffer 생성
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;

		//화면에 출력
		void Render(LPDIRECT3DDEVICE9 _device) override;

		//void Get_Texture(LPDIRECT3DTEXTURE9& _texture) const;
		//void Set_Texture(const LPDIRECT3DTEXTURE9& _texture);

		bool Set_Texture(const std::wstring& _path)
		{
			//set path는 로딩에서 다 해줘야 프레임 드랍이 없을 듯
			m_Path = _path;
			RenderManager::GetInstance().Add_Texture(_path);
			m_Texture = *(RenderManager::GetInstance().Get_Texture(_path));

			Set_NativeSize();

			if (m_Texture) return true;
			else return false;
		}

		bool Ready_Texture()
		{
			if (m_Path.empty())
			{
				return false;
			}
			m_Texture = *(RenderManager::GetInstance().Get_Texture(m_Path));
			return true;
		}

		void Compute_FlipMat(D3DXMATRIX& _mat) const;

		void Set_NativeSize();
		void Set_NormalSize();

		void Set_FlipX(bool _flip) { m_FlipX = _flip; }
		void Set_FlipY(bool _flip) { m_FlipY = _flip; }

		bool Get_FlipX() const { return m_FlipX; }
		bool Get_FlipY() const { return m_FlipY; }


		std::wstring Get_Path() const { return m_Path; }
		void Set_Path(const std::wstring& _path) { m_Path = _path; }

		Component* Clone() const override
		{
			return new TextureRenderer(*this);
		}

		//======================================//
		//				 serialize				//
		//======================================//
		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "TextureRenderer";
			_j = nlohmann::ordered_json{
				{"type", type},
				{"enable", m_bEnabled},
				{"path", m_Path},
				{"flipX", m_FlipX},
				{"flipY", m_FlipY}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
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

	private:
		std::wstring				m_Path;
		LPDIRECT3DTEXTURE9			m_Texture;
		D3DXMATRIX					m_TextureScaleMatrix;
		//Shader*					m_Shader;
		bool						m_FlipX;
		bool						m_FlipY;
	};

	REGISTER_COMPONENT(TextureRenderer)
}
