#pragma once
#include "core_define.h"

namespace GameEngine
{
	class COREMODULE_API InputManager
	{
		DECLARE_SINGLETON(InputManager)

	private:
		InputManager();
		~InputManager();

	public:
		bool		Get_Key(int _iKey) const; //���� �����ӿ� ���ȴ��� ����
		bool		Key_Pressing(int _iKey) const; //��� ������ ���� ��
		bool		Key_Down(int _iKey) const; //ó�� ���� ��
		bool		Key_Up(int _iKey) const; //ó�� ������ ���� ��
		void		Update_Key();

	private:
		enum KEYSTATE
		{
			KEY_IDLE = 0b0000, //������ ���� ����
			KEY_PRESSING = 0b0001, //������ �ִ� ����
			KEY_DOWN = 0b0010, // ������ ���� �� ���µ� �̹��� ���� ����
			KEY_UP = 0b0100, // ������ ���ȴµ� �̹��� �ȴ��� ����
		};

		KEYSTATE	m_bKeyState[VK_MAX];

		POINT		m_MouseClientPos;
	};
}
