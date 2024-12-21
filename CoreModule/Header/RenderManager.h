#pragma once
#include "core_define.h"


namespace GameEngine
{
	class Renderer;

	class COREMODULE_API RenderManager
	{
	private:
		//======================================//
		//				constructor				//
		//======================================//
		RenderManager() = default;
		~RenderManager();

	public:
		DECLARE_SINGLETON(RenderManager)

	public:
		//======================================//
		//				  method				//
		//======================================//
		void Render_Begin(LPDIRECT3DDEVICE9 _device);
		void Render(LPDIRECT3DDEVICE9 _device);
		void Render_End(LPDIRECT3DDEVICE9 _device);
		void Add_Renderer(Renderer* _renderer);
		void Release();

	private:
		// 기준을 잡아야하는데
		// 매번 객체마다 RenderState마다 따로 배열을 할당하거나 RenderUpdate()를 다르게 만들어주는건 비효율 같음...
		// 하지만 타입별로 나눠야할 필요성은 있음 (Default, UI, Effect 등등)


		// 그리고 월드 전역으로 사용하는 값들을 RenderManager에서 변수로 보유
		// 1. ViewMat
		// 2. ProjMat
		// 3. D3DLIGHT9(Directional), Light Dir



		std::list<Renderer*> m_Renderers;
	};
}