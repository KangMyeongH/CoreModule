#pragma once
#include "Renderer.h"

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
		{}
		explicit TextureRenderer(GameObject* _owner) : Renderer(_owner),
			m_Texture(nullptr)
		{}
		TextureRenderer(const TextureRenderer& _rhs) : Renderer(_rhs),
			m_Texture(_rhs.m_Texture)
		{}
		~TextureRenderer() override = default;

		//======================================//
		//				 method					//
		//======================================//

	public:
		//vertex buffer 및 index buffer 생성
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;

		//화면에 출력
		void Render(LPDIRECT3DDEVICE9 _device) override;

		void Get_Texture(LPDIRECT3DTEXTURE9& _texture) const;
		void Set_Texture(const LPDIRECT3DTEXTURE9& _texture);

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
			_j = nlohmann::json{
				{"type", type}
			};
		}
		void from_json(const nlohmann::ordered_json& _j) override
		{

		}

	private:
		LPDIRECT3DTEXTURE9		m_Texture;
		//Shader*					m_Shader;
	};

	REGISTER_COMPONENT(TextureRenderer)
}
