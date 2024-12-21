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
		// ������ ��ƾ��ϴµ�
		// �Ź� ��ü���� RenderState���� ���� �迭�� �Ҵ��ϰų� RenderUpdate()�� �ٸ��� ������ִ°� ��ȿ�� ����...
		// ������ Ÿ�Ժ��� �������� �ʿ伺�� ���� (Default, UI, Effect ���)


		// �׸��� ���� �������� ����ϴ� ������ RenderManager���� ������ ����
		// 1. ViewMat
		// 2. ProjMat
		// 3. D3DLIGHT9(Directional), Light Dir



		std::list<Renderer*> m_Renderers;
	};
}