#pragma once
#include "Renderer.h"

namespace GameEngine
{
	struct TrailPoint
	{
		Vector3 Position;
		float	TimeCreated;
	};

	class COREMODULE_API TrailRenderer : public Renderer
	{
	public:
		TrailRenderer() : Renderer(nullptr),
		m_Color(1.f,1.f,1.f,1.f),
		m_CurrentTime(0.f),
		m_TrailTime(0.2f),
		m_MinVertexDistance(0.1f),
		m_Width(2.f), m_VertexCount(0)
		{
			m_Option = ALPHA_BLENDING;
		}

		explicit TrailRenderer(GameObject* _owner)
			: Renderer(_owner),
			m_Color(1.f, 1.f, 1.f, 1.f),
			  m_CurrentTime(0),
			  m_TrailTime(0.2f),
			  m_MinVertexDistance(0.1f),
			  m_Width(2.f), m_VertexCount(0)
		{
			m_Option = ALPHA_BLENDING;
		}

		void Set_TrailTime(const float _time) { m_TrailTime = _time; }
		float Get_TrailTime() const { return m_TrailTime; }

		void Set_MinVertexDistance(const float _distance) { m_MinVertexDistance = _distance; }
		float Get_MinVertexDistance() const { return m_MinVertexDistance; }

		void Set_Width(const float _width) { m_Width = _width; }
		float Get_Width() const { return m_Width; }

		void Set_Color(D3DXCOLOR _color) { m_Color = _color; }
		D3DXCOLOR Get_Color() const { return m_Color; }
		
		void Update_Trail();
		void Build_Trail(LPDIRECT3DDEVICE9 _device);

		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
		void Render(LPDIRECT3DDEVICE9 _device) override;
		Component* Clone() const override
		{
			return new TrailRenderer(*this);
		}

		void to_json(nlohmann::ordered_json& _j) override
		{
			std::string type = "TrailRenderer";
			_j = nlohmann::ordered_json{
				{"type", type},
				{"enable", m_bEnabled},
				{"width", m_Width}
			};
		}

		void from_json(const nlohmann::ordered_json& _j) override
		{
			if (_j.contains("enable"))
			{
				_j.at("enable").get_to(m_bEnabled);
			}

			if (_j.contains("width"))
			{
				_j.at("width").get_to(m_Width);
			}
		}

	private:
		std::vector<TrailPoint> m_Points;
		D3DXCOLOR				m_Color;
		float 					m_CurrentTime;
		float 					m_TrailTime;
		float 					m_MinVertexDistance;
		float 					m_Width;
		int 					m_VertexCount;
	};
	REGISTER_COMPONENT(TrailRenderer)
}
