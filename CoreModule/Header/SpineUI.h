#pragma once
#include "UI.h"

namespace spine
{
	class SpineLoader;
}

namespace GameEngine
{
	class COREMODULE_API SpineUI : public UI
	{
	public:
		SpineUI();
		explicit SpineUI(GameObject* _owner, const std::string& _path, const std::string& _skin = "", const std::string& _animation = "");
		explicit SpineUI(GameObject* _owner);
		SpineUI(const SpineUI& _rhs);
		~SpineUI() override;

		spine::Skeleton* Get_Skeleton() const { return m_Skeleton; }
		spine::AnimationState* Get_State() const { return m_State; }
		void Set_TimeScale(float scale) { m_TimeScale = scale; }

		// Premultiplied Alpha 사용 여부
		void Set_UsePremultipliedAlpha(bool usePMA) { m_UsePMA = usePMA; }
		bool Get_UsePremultipliedAlpha() const { return m_UsePMA; }

		void Set_FlipX(bool _flip) { m_FlipX = _flip; }
		void Set_FlipY(bool _flip) { m_FlipY = _flip; }

		bool Get_FlipX() const { return m_FlipX; }
		bool Get_FlipY() const { return m_FlipY; }

		std::string Get_Path() const { return m_Path; }
		void Set_Path(const std::string& _path) { m_Path = _path; }

		void Ready_UI(LPDIRECT3DDEVICE9 _device) override;
		void Update_Animation(float _deltaTime);
		void Render_UI(LPDIRECT3DDEVICE9 _device) override;

		void Change_Skin(const std::string& _skin);
		void Change_Animation(const std::string& _animation, bool _isLoop);
		void Change_Animation(const std::string& _track1, const std::string& _track2, bool _isLoop);
		bool Is_Finished(int _track);

		void to_json(nlohmann::ordered_json& _j) override;
		void from_json(const nlohmann::ordered_json& _j) override;
		Component* Clone() const override;

	private:
		std::shared_ptr<spine::SkeletonData> readSkeletonBinaryData(const std::string& _path, spine::Atlas* _atlas);

	public:
		bool Is_MouseHovered() override;
		bool Is_ButtonDown() override;
		bool Is_ButtonHold() override;
		bool Is_ButtonUp() override;

	private:
		spine::SpineLoader* m_Loader;
		std::unique_ptr<spine::Atlas> 			m_Atlas;
		std::shared_ptr<spine::SkeletonData> 	m_SkeletonData;
		spine::Skeleton* m_Skeleton;
		spine::AnimationState* m_State;
		bool            						m_OwnsAnimationStateData;
		bool            						m_UsePMA;
		float           						m_TimeScale;

		// 정점/UV 보관용 버퍼
		mutable spine::Vector<float> 		m_worldVertices;

		// 클리핑 처리
		mutable spine::SkeletonClipping 	m_clipper;

		// 사각형 인덱스(RegionAttachment)용
		spine::Vector<unsigned short> 		m_quadIndices;

		std::string m_Path;
		std::vector<std::string> m_Skins;
		std::vector<std::string> m_Animations;
		spine::String	m_CurrentSkin;
		spine::String	m_CurrentAnimation;
		bool	m_FlipX;
		bool	m_FlipY;
	};
	REGISTER_COMPONENT(SpineUI)
}

