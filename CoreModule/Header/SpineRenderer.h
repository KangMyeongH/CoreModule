#pragma once
#include "Renderer.h"

namespace GameEngine
{
	class SpineMaterial;
}

namespace spine
{
	class SpineLoader;
}

namespace GameEngine
{
	class COREMODULE_API SpineRenderer : public Renderer
	{
	public:
		SpineRenderer();
		explicit SpineRenderer(GameObject* _owner, const std::string& _path, const std::string& _skin = "", const std::string& _animation = "");
		explicit SpineRenderer(GameObject* _owner);
		SpineRenderer(const SpineRenderer& _rhs);

		~SpineRenderer() override;

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

		std::wstring Get_EmissionPath() const { return m_EmissionMapPath; }
		void Set_EmissionPath(const std::wstring& _path);

		D3DXCOLOR Get_EmissionColor() const { return m_EmissionColor; }
		void Set_EmissionColor(const D3DXCOLOR& _color) { m_EmissionColor = _color; }

		void Set_Material(SpineMaterial* _material);
		SpineMaterial* Get_Material() const { return m_SpineMaterial; }

		void Enable_Billboard(const bool _enable) { m_bBillboard = _enable; }

		bool Is_Outline() const { return m_bOutline; }
		void Enable_Outline(const bool _enable) { m_bOutline = _enable; }

		void Change_Skin(const std::string& _skin);
		void Change_Animation(const std::string& _animation, bool _isLoop);
		void Change_Animation(const std::string& _track1, const std::string& _track2, bool _isLoop);
		bool Is_Finished(int _track);

		void Update_Animation(float _deltaTime);
		void Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
		void Render(LPDIRECT3DDEVICE9 _device) override;
		Component* Clone() const override;
		void to_json(nlohmann::ordered_json& _j) override;
		void from_json(const nlohmann::ordered_json& _j) override;

	private:
		std::shared_ptr<spine::SkeletonData> readSkeletonBinaryData(const std::string& _path, spine::Atlas* _atlas);

	private:
		spine::SpineLoader* 					m_Loader;
		std::unique_ptr<spine::Atlas> 			m_Atlas;
		std::shared_ptr<spine::SkeletonData> 	m_SkeletonData;
		spine::Skeleton* 						m_Skeleton;
		spine::AnimationState*					m_State;
		bool            						m_OwnsAnimationStateData;
		bool            						m_UsePMA;
		bool									m_bOutline;
		float           						m_TimeScale;
		float									m_BlinkAlpha;

		// 정점/UV 보관용 버퍼
		mutable spine::Vector<float> 		m_worldVertices;

		// 클리핑 처리
		mutable spine::SkeletonClipping 	m_clipper;

		// 사각형 인덱스(RegionAttachment)용
		spine::Vector<unsigned short> 		m_quadIndices;

		std::string 				m_Path;
		std::vector<std::string> 	m_Skins;
		std::vector<std::string> 	m_Animations;
		spine::String				m_CurrentSkin;
		spine::String				m_CurrentAnimation;

		SpineMaterial*				m_SpineMaterial;
		LPDIRECT3DTEXTURE9			m_EmissionMap;
		std::wstring				m_EmissionMapPath;
		D3DXCOLOR					m_EmissionColor;

		bool						m_FlipX;
		bool						m_FlipY;
		bool						m_bBillboard;
	};
	REGISTER_COMPONENT(SpineRenderer)
}
