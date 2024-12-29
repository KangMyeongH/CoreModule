#pragma once
#include <typeindex>

#include "core_define.h"


namespace GameEngine
{
	class Renderer;

	enum Buffer
	{
		CUBE,
		TEXTURE,
		BUFFER_END
	};

	using Buffer_Map = std::unordered_map<GameEngine::Buffer, std::pair<LPDIRECT3DVERTEXBUFFER9, LPDIRECT3DINDEXBUFFER9>>;
	using Texture_Map = std::unordered_map<std::wstring, LPDIRECT3DTEXTURE9>;
	using PixelShader_Map = std::unordered_map<std::wstring, LPDIRECT3DPIXELSHADER9>;


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
		void Initialize(LPDIRECT3DDEVICE9 _device);

		void Ready_Buffer(LPDIRECT3DDEVICE9 _device);
		void Render_Begin(LPDIRECT3DDEVICE9 _device);
		void Render(LPDIRECT3DDEVICE9 _device);
		void Render_End(LPDIRECT3DDEVICE9 _device);
		void Add_Renderer(Renderer* _renderer);
		void Add_Texture(const std::wstring& _name, const std::wstring& _path);
		void Add_PixelShader(const std::wstring& _name, const std::wstring& _path);
		void Remove_Renderer(Renderer* _renderer);
		void Register_Renderer();
		void Destroy_Renderer();
		void Release();

		void Set_ViewMat(const D3DXMATRIX& _viewMat)	{ m_ViewMat = _viewMat; }
		void Set_ProjMat(const D3DXMATRIX& _projMat) 	{ m_ProjMat = _projMat; }
		void Set_DirLight(const D3DLIGHT9& _dirLight) 	{ m_DirLight = _dirLight; }

		LPDIRECT3DTEXTURE9& Get_Texture(const std::wstring& _name);
		LPDIRECT3DPIXELSHADER9& Get_PixelShader(const std::wstring& _name);

	private:
		LPDIRECT3DDEVICE9		m_Device;

		std::vector<Renderer*> 	m_Renderers;
		std::list<Renderer*> 	m_RegisterQueue;
		std::list<Renderer*> 	m_DestroyQueue;
		Buffer_Map				m_BufferMap;
		Texture_Map				m_TextureMap;
		PixelShader_Map			m_PixelShaderMap;

		D3DXMATRIX 				m_ViewMat;
		D3DXMATRIX 				m_ProjMat;
		D3DLIGHT9 				m_DirLight;


		// �İ�ü �ּҸ� �ش��ϴ� ������Ʈ�� �����ؼ� ���
		// Renderer�Ŵ����� �ε���, ���ؽ� ������ �ּ� Map�����̳ʷ� ����־������.......

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
