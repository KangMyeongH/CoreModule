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

		void Ready_Buffer(LPDIRECT3DDEVICE9 _device);
		void Render_Begin(LPDIRECT3DDEVICE9 _device);
		void Render(LPDIRECT3DDEVICE9 _device);
		void Render_End(LPDIRECT3DDEVICE9 _device);
		void Add_Renderer(Renderer* _renderer);
		void Remove_Renderer(Renderer* _renderer);
		void Register_Renderer();
		void Destroy_Renderer();
		void Release();

		void Set_ViewMat(const D3DXMATRIX& _viewMat)	{ m_ViewMat = _viewMat; }
		void Set_ProjMat(const D3DXMATRIX& _projMat) 	{ m_ProjMat = _projMat; }
		void Set_DirLight(const D3DLIGHT9& _dirLight) 	{ m_DirLight = _dirLight; }

	private:
		std::vector<Renderer*> 	m_Renderers;
		std::list<Renderer*> 	m_RegisterQueue;
		std::list<Renderer*> 	m_DestroyQueue;

		D3DXMATRIX 				m_ViewMat;
		D3DXMATRIX 				m_ProjMat;
		D3DLIGHT9 				m_DirLight;


		// 컴객체 주소를 해당하는 컴포넌트가 공유해서 사용
		// Renderer매니저가 인덱스, 버텍스 버퍼의 주소 Map컨테이너로 들고있어야할지.......

		// 기준을 잡아야하는데
		// 매번 객체마다 RenderState마다 따로 배열을 할당하거나 RenderUpdate()를 다르게 만들어주는건 비효율 같음...
		// 하지만 타입별로 나눠야할 필요성은 있음 (Default, UI, Effect 등등)


		// 그리고 월드 전역으로 사용하는 값들을 RenderManager에서 변수로 보유
		// 1. ViewMat
		// 2. ProjMat
		// 3. D3DLIGHT9(Directional), Light Dir
		// *해당 변수들은 월드에 있는 Camera, Directional Light 컴포넌트들이 자동으로 RenderManager에 값을 채워줌.


		// 렌더할거 추가할때마다 개선
		// 캡슐화 모듈화 >>> 고민 많이 해야될거같다.
		// Cube, TextureRenderer, 게임은 만들수 있다.
	};
}