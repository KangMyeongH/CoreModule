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
		std::list<Renderer*> m_Renderers;
	};
}