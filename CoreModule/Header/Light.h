#pragma once
#include "Behaviour.h"

namespace GameEngine
{
	class COREMODULE_API Light : public Behaviour
	{
	private:
		static UINT m_IndexValue;

	public:
		Light() : Behaviour(nullptr),
		m_LightInfo(nullptr)
		{
			m_Index = m_IndexValue;
			m_IndexValue++;
		}

		explicit Light(GameObject* _owner) : Behaviour(_owner),
		m_LightInfo(nullptr)
		{}

		~Light()
		{
			//삭제하기 전에 _device->LightEnable(m_Index, FALSE); 필요
			// 근데 device 어케 가져올지 모르겠음
			// 문제 생기면 띵호햄한테 토스 ㅎ
		}

	public:
		void	Ready_Light(LPDIRECT3DDEVICE9 _device);
		void 	Update_Light(LPDIRECT3DDEVICE9 _device) const;

		void	Set_Type(D3DLIGHTTYPE _type) const { m_LightInfo->Type = _type; }
		void	Set_Diffuse(D3DCOLORVALUE _diffuse) const { m_LightInfo->Diffuse = _diffuse; }
		void	Set_Specular(D3DCOLORVALUE _specular) const { m_LightInfo->Specular = _specular; }
		void	Set_Ambient(D3DCOLORVALUE _ambient) const { m_LightInfo->Ambient = _ambient; }
		void	Set_Range(float _range) const { m_LightInfo->Range = _range; }

		UINT	Get_Index() const { return m_Index; }

		void 	Destroy() override;

	public:
		void 	to_json(nlohmann::ordered_json& _j) override;
		void 	from_json(const nlohmann::ordered_json& _j) override;

		Component* Clone() const override;

	private:
		D3DLIGHT9*	m_LightInfo;
		UINT		m_Index;
	};
	REGISTER_COMPONENT(Light)
}

