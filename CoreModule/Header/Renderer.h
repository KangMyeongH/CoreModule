#pragma once
#include "Behaviour.h"
namespace GameEngine
{
	class COREMODULE_API Renderer : public Behaviour
	{
	public:
		explicit Renderer(GameObject* _owner) : Behaviour(_owner) {}

		virtual void Get_Buffer(LPDIRECT3DVERTEXBUFFER9& _vertexBuffer, LPDIRECT3DINDEXBUFFER9& _indexBuffer) = 0;
		virtual void Set_Buffer(LPDIRECT3DVERTEXBUFFER9 _vertexBuffer, LPDIRECT3DINDEXBUFFER9 _indexBuffer) = 0;

		virtual void Ready_Buffer(LPDIRECT3DDEVICE9 _device) = 0;
		virtual void Render(LPDIRECT3DDEVICE9 _device) = 0;
		Component* Clone() const override = 0;
		void Destroy() override = 0;
		void to_json(nlohmann::ordered_json& _j) override = 0;
		void from_json(const nlohmann::ordered_json& _j) override = 0;
	};

}
