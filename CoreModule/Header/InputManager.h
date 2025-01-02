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
		Vector3		Get_MousePos() const { return m_MousePos; }
		void		Set_MousePos(const Vector3& _mousePos) { m_MousePos = _mousePos; }
		void		Update_Key(HWND _hwnd);
		void		Update_Editor(HWND _hwnd, const Vector3& _mousePos);

	private:
		enum KEYSTATE
		{
			KEY_IDLE = 0b0000, //������ ���� ����
			KEY_PRESSING = 0b0001, //������ �ִ� ����
			KEY_DOWN = 0b0010, // ������ ���� �� ���µ� �̹��� ���� ����
			KEY_UP = 0b0100, // ������ ���ȴµ� �̹��� �ȴ��� ����
		};

		KEYSTATE	m_bKeyState[VK_MAX];
		Vector3		m_MousePos;
		bool		m_bEditor;
	};
}
