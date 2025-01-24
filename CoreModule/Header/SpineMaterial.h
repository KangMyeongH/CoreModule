#pragma once
#include "Object.h"

namespace GameEngine
{
	class SpineMaterial : public Object
	{
	public:
		SpineMaterial(LPDIRECT3DDEVICE9 _device, const std::string& _effectPath);
		~SpineMaterial() override;

		ID3DXEffect* Get_Effect() const { return  m_Effect; }

		void Set_WorldMat(const D3DXMATRIX& _worldMat);
		void Set_ViewProjMat(const D3DXMATRIX& _view, const D3DXMATRIX& _proj);
		void Set_Texture(const std::string& _paramName, LPDIRECT3DTEXTURE9 _texture);
		void Set_Light(const D3DLIGHT9* _light);
		void Set_Color(const std::string& _paramName, const D3DXVECTOR4& _color);
		void Set_Bool(const std::string& _paramName, bool _bool);
		void Set_Float(const std::string& _paramName, float _float);

		bool Load_EffectFromFile(const std::string& _filePath);
		void Begin();
		void Begin_Pass(UINT _pass);
		void End_Pass();
		void End();
		void Destroy() override {}

	private:
		LPDIRECT3DDEVICE9	m_Device;
		ID3DXEffect*		m_Effect;
		std::string			m_EffectPath;
	};

}

