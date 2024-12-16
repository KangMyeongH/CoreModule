#include "Core.h"

void Core::Initialize(LPDIRECT3DDEVICE9 device)
{
	device->AddRef();

}

void Core::Progress()
{

}

void Core::Release()
{
	if (m_Device)
	{
		m_Device->Release();
	}


}
