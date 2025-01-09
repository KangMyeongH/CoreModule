#pragma once
#include "core_define.h"

namespace spine
{
	class COREMODULE_API SpineLoader : public TextureLoader
	{
	public:
		SpineLoader(LPDIRECT3DDEVICE9 _device) : m_Device(_device) {}

		// AtlasPage 로딩
		void load(AtlasPage& page, const String& path) override;

		// AtlasPage 언로딩
		void unload(void* texture) override;

	private:
		LPDIRECT3DDEVICE9 m_Device;
	};

}
