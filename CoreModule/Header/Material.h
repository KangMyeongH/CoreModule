#pragma once
#include "Object.h"
namespace GameEngine
{
	class Material : public Object
	{
	public:
		Material(
			LPDIRECT3DDEVICE9 _device,
			const std::string& vsFilePath,
			const std::string& vsEntry,
			const std::string& vsTarget = "vs_3_0",
			const std::string& psFilePath,
			const std::string& psEntry,
			const std::string& psTarget = "ps_3_0"
		);

		~Material() override;

		void SetVertexShaderMatrix(const std::string& path, const D3DXMATRIX& mat);


		void Destroy() override;

	private:
		LPDIRECT3DDEVICE9	m_Device;
		IDirect3DVertexShader9* m_VertexShader;
		LPD3DXCONSTANTTABLE		m_VSConstantTable;

		IDirect3DVertexShader9* m_PixelShader;
		LPD3DXCONSTANTTABLE		m_PSConstantTable;
	};

}
