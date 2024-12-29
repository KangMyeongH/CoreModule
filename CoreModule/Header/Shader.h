#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API Shader
	{
	public:
		//생성자 초기화
		Shader();
		Shader(const Renderer& _rhs);
		~Shader();

	public:
		Ready_Shader(LPDIRECT3DDEVICE9 _device) = 0;
		Render_Setting(LPDIRECT3DDEVICE9 _device) = 0;

	protected:
		std::wstring m_Path;
	};

}