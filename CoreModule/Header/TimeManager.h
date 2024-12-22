#pragma once
#include "core_define.h"

namespace GameEngine
{
	class COREMODULE_API TimeManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//

		TimeManager();
		~TimeManager();
	public:
		DECLARE_SINGLETON(TimeManager)

	public:
		//======================================//
		//				 property				//
		//======================================//

		float	Get_DeltaTime() const { return m_DeltaTime; }
		float	Get_UnscaledDeltaTime() const { return m_UnscaledDeltaTime; }
		float	Get_TimeScale() const { return m_TimeScale; }
		void	Set_TimeScale(const float& _scale) { m_TimeScale = _scale; }
		float	Get_UnscaledTime() const { return m_UnscaledTime; }
		void	Set_TargetFrameRate(const float& _fps) { m_TargetFrameTime = 1.0f / _fps; }

		//======================================//
		//				  method				//
		//======================================//

		void	Initialize();
		void	TimeUpdate();
		void	FrameLimit();
		void	SlowMotion(const float& _timeScale, const float& _slowMotionTime)
		{
			m_TimeScale = _timeScale;
			m_SlowMotionTime = _slowMotionTime;
		}

	private:
		LARGE_INTEGER	m_Frequency;
		LARGE_INTEGER	m_LastFrameTime;
		LARGE_INTEGER	m_CurrentFrameTime;
		double			m_AccTime;
		float			m_DeltaTime;
		float			m_UnscaledDeltaTime;
		float			m_TimeScale;
		float			m_UnscaledTime;
		float			m_TargetFrameTime;
		float			m_SlowMotionTime;
		float			m_SlowTime;
		int				m_FPS;
	};
}

