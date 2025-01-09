#pragma once
#include "Renderer.h"

namespace GameEngine
{
	class COREMODULE_API SpineRenderer : public Renderer
	{
	public:
		SpineRenderer();
		explicit SpineRenderer(GameObject* _owner);
		SpineRenderer(const SpineRenderer& _rhs) = default;
		~SpineRenderer() override;

		spine::Skeleton* Get_Skeleton() const { return m_Skeleton; }
		spine::AnimationState* Get_State() const { return m_State; }
		void Set_TimeScale(float scale) { m_timeScale = scale; }

		// Premultiplied Alpha 사용 여부
		void Set_UsePremultipliedAlpha(bool usePMA) { m_usePMA = usePMA; }
		bool Get_UsePremultipliedAlpha() const { return m_usePMA; }

		void Set_FlipX(bool _flip) { m_FlipX = _flip; }
		void Set_FlipY(bool _flip) { m_FlipY = _flip; }

		bool Get_FlipX() const { return m_FlipX; }
		bool Get_FlipY() const { return m_FlipY; }

		std::string Get_Path() const { return m_Path; }
		void Set_Path(const std::string& _path) { m_Path = _path; }

		void Change_Skin(const std::string& _skin);
		void Change_Animation(const std::string& _animation, bool _isLoop);
		void Change_Animation(const std::string& _track1, const std::string& _track2, bool _isLoop);
		bool Is_Finished(int _track);

		void onSpineEvent(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) {
			if (type == spine::EventType_Event && event) {
				// 이벤트가 발생했을 때 처리
				std::string eventName = event->getData().getName().buffer();
				int intValue = event->getIntValue();
				float floatValue = event->getFloatValue();
				std::string stringValue = event->getStringValue().buffer();

				// 디버그 출력 예시
				printf("Event received: %s, Int: %d, Float: %.2f, String: %s\n",
					eventName.c_str(), intValue, floatValue, stringValue.c_str());
			}
		}
		void Update_Animation(float _deltaTime);
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
		void Render(LPDIRECT3DDEVICE9 _device) override;
		Component* Clone() const override;
		void to_json(nlohmann::ordered_json& _j) override;
		void from_json(const nlohmann::ordered_json& _j) override;

	private:
		spine::Skeleton* 		m_Skeleton;
		spine::AnimationState*	m_State;
		bool            		m_ownsAnimationStateData;
		bool            		m_usePMA;
		float           		m_timeScale;

		// 정점/UV 보관용 버퍼
		mutable spine::Vector<float> 		m_worldVertices;

		// 클리핑 처리
		mutable spine::SkeletonClipping 	m_clipper;

		// 사각형 인덱스(RegionAttachment)용
		spine::Vector<unsigned short> 		m_quadIndices;

		std::string m_Path;
		std::vector<std::string> m_Skins;
		std::vector<std::string> m_Animations;
		bool	m_FlipX;
		bool	m_FlipY;
	};
	REGISTER_COMPONENT(SpineRenderer)
}
