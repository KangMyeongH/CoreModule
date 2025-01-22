#pragma once
#include "Object.h"
namespace GameEngine
{
	class COREMODULE_API Material : public Object
	{
	public:
		Material(
			LPDIRECT3DDEVICE9 _device,
			const std::string& _effectPath
		);

		~Material() override;

		void Set_WorldMat(const D3DXMATRIX& _worldMat);
		void Set_ViewProjMat(const D3DXMATRIX& _view, const D3DXMATRIX& _proj);
		void Set_Texture(const std::string& _paramName, LPDIRECT3DTEXTURE9 _texture);
		void Set_Light(const D3DLIGHT9* _light);
		void Set_Color(const std::string& _paramName, const D3DXVECTOR4& _color);
		void Set_Billboard(bool _isBillboard);

		bool Load_EffectFromFile(const std::string& _filePath);
		void Begin();
		void Begin_Pass(UINT _pass);
		void End_Pass();
		void End();
		void Destroy() override {}

	private:
		LPDIRECT3DDEVICE9				m_Device;
		ID3DXEffect* 					m_Effect;
		std::string						m_EffectPath;
	};

}
