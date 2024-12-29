#pragma once
#include <regex>

#include "Renderer.h"
#include "RenderManager.h"

//�ӽ�
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
		//vertex buffer �� index buffer ����
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;

		//ȭ�鿡 ���
		void Render(LPDIRECT3DDEVICE9 _device) override;

		//void Get_Texture(LPDIRECT3DTEXTURE9& _texture) const;
		//void Set_Texture(const LPDIRECT3DTEXTURE9& _texture);

		void Set_Texture(std::wstring& _path)
		{
			//set path�� �ε����� �� ����� ������ ����� ���� ��
			m_path = _path;
			RenderManager::GetInstance().Add_Texture(_path);
			m_Texture = *(RenderManager::GetInstance().Get_Texture(_path));
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
