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
		void Remove_Renderer(Renderer* _renderer);
		void Register_Renderer();
		void Destroy_Renderer();
		void Release();

		void Set_ViewMat(const D3DXMATRIX& _viewMat)	{ m_ViewMat = _viewMat; }
		void Set_ProjMat(const D3DXMATRIX& _projMat) 	{ m_ViewMat = _projMat; }
		void Set_DirLight(const D3DLIGHT9& _dirLight) 	{ m_DirLight = _dirLight; }

	private:
		std::vector<Renderer*> 	m_Renderers;
		std::list<Renderer*> 	m_RegisterQueue;
		std::list<Renderer*> 	m_DestroyQueue;
		D3DXMATRIX 	m_ViewMat;
		D3DXMATRIX 	m_ProjMat;
		D3DLIGHT9 	m_DirLight;

		// ������ ��ƾ��ϴµ�
		// �Ź� ��ü���� RenderState���� ���� �迭�� �Ҵ��ϰų� RenderUpdate()�� �ٸ��� ������ִ°� ��ȿ�� ����...
		// ������ Ÿ�Ժ��� �������� �ʿ伺�� ���� (Default, UI, Effect ���)


		// �׸��� ���� �������� ����ϴ� ������ RenderManager���� ������ ����
		// 1. ViewMat
		// 2. ProjMat
		// 3. D3DLIGHT9(Directional), Light Dir
		// *�ش� �������� ���忡 �ִ� Camera, Directional Light ������Ʈ���� �ڵ����� RenderManager�� ���� ä����.


		// �����Ұ� �߰��Ҷ����� ����
		// ĸ��ȭ ���ȭ >>> ��� ���� �ؾߵɰŰ���.
		// Cube, TextureRenderer, ������ ����� �ִ�.
	};
}