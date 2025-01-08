#pragma once
#include "Renderer.h"
namespace GameEngine
{
	class COREMODULE_API SpriteAnimation : public Renderer
	{
	public:
		//======================================//
		//				constructor				//
		//======================================//

		SpriteAnimation() : Renderer(nullptr),
		                    m_SpriteWidth(0),
		                    m_SpriteHeight(0), m_DurationTime(0), m_AccTime(0),
		                    m_CurrentFrame(0)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}

		explicit SpriteAnimation(GameObject* _owner) : Renderer(_owner),
		                                               m_SpriteWidth(0),
		                                               m_SpriteHeight(0), m_DurationTime(0), m_AccTime(0),
		                                               m_CurrentFrame(0)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}

		SpriteAnimation(const SpriteAnimation& _rhs) : Renderer(_rhs),
		                                               m_SpriteWidth(_rhs.m_SpriteWidth),
		                                               m_SpriteHeight(_rhs.m_SpriteHeight), m_DurationTime(0),
		                                               m_AccTime(0),
		                                               m_CurrentFrame(_rhs.m_CurrentFrame)
		{
			m_VertexSize = sizeof(VTXTEX);
			m_VertexCnt = 4;
			m_TriangleCnt = 2;
			m_FVF = FVF_TEX;

			D3DXMatrixIdentity(&m_TextureScaleMatrix);
		}

		~SpriteAnimation() override = default;

	public:
		void		Add_Animator(const std::string& _key, const std::wstring& _path);
		void		Change_Animation(const std::string& _key);
		void		Set_SpriteSize(float _width, float _height);

		void		Set_TextureCoordinates(LPDIRECT3DDEVICE9 _device);
		void		Update_Frame(float _deltaTime);
		void 		Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
		void 		Render(LPDIRECT3DDEVICE9 _device) override;
		Component* 	Clone() const override;
		void 		to_json(nlohmann::ordered_json& _j) override;
		void 		from_json(const nlohmann::ordered_json& _j) override;

	private:
		std::unordered_map<std::string, Animator> m_Animator;
		std::string				m_CurrentAnimationState;
		D3DXMATRIX				m_TextureScaleMatrix;
		float					m_SpriteWidth;		// 각 프레임의 가로 픽셀
		float					m_SpriteHeight;		// 각 프레임의 세로 픽셀
		float					m_DurationTime;
		float					m_AccTime;
		int						m_CurrentFrame;
	};
	REGISTER_COMPONENT(SpriteAnimation)
}
