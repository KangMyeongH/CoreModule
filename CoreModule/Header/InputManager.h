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
		bool		Get_Key(int _iKey) const; //현재 프레임에 눌렸는지 여부
		bool		Key_Pressing(int _iKey) const; //계속 눌리는 중일 때
		bool		Key_Down(int _iKey) const; //처음 눌릴 때
		bool		Key_Up(int _iKey) const; //처음 눌리지 않을 때
		Vector3		Get_MousePos() const { return m_MousePos; }
		void		Set_MousePos(const Vector3& _mousePos) { m_MousePos = _mousePos; }
		void		Update_Key(HWND _hwnd);
		void		Update_Editor(HWND _hwnd, const Vector3& _mousePos);

	private:
		enum KEYSTATE
		{
			KEY_IDLE = 0b0000, //눌리지 않은 상태
			KEY_PRESSING = 0b0001, //눌리고 있는 상태
			KEY_DOWN = 0b0010, // 이전에 눌린 적 없는데 이번에 눌린 상태
			KEY_UP = 0b0100, // 이전에 눌렸는데 이번에 안눌린 상태
		};

		KEYSTATE	m_bKeyState[VK_MAX];
		Vector3		m_MousePos;
		bool		m_bEditor;
	};
}
