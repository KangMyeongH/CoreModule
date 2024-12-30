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
			m_Texture(nullptr)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;
		}
		explicit TextureRenderer(GameObject* _owner) : Renderer(_owner),
			m_Texture(nullptr)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;
		}
		TextureRenderer(const TextureRenderer& _rhs) : Renderer(_rhs),
			m_Texture(_rhs.m_Texture)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;
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

		bool Set_Texture(std::wstring& _path)
		{
			//set path는 로딩에서 다 해줘야 프레임 드랍이 없을 듯
			m_path = _path;
			RenderManager::GetInstance().Add_Texture(_path);
			m_Texture = *(RenderManager::GetInstance().Get_Texture(_path));

			if (m_Texture) return true;
			else return false;
		}

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
				{"type", type}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{

		}

	private:
		std::wstring				m_path;
		LPDIRECT3DTEXTURE9			m_Texture;
		//Shader*					m_Shader;
	};

	REGISTER_COMPONENT(TextureRenderer)
}
