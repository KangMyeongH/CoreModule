#include "InputManager.h"

IMPLEMENT_SINGLETON(GameEngine::InputManager)

GameEngine::InputManager::InputManager(): m_bEditor(false)
{
	memset(m_bKeyState, KEY_IDLE, sizeof(m_bKeyState));
}

GameEngine::InputManager::~InputManager() = default;

bool GameEngine::InputManager::Get_Key(const int _iKey) const
{
	if (m_bKeyState[_iKey] & (KEY_PRESSING | KEY_DOWN))
		return true;

	return false;
}

//������ ���� ��
bool GameEngine::InputManager::Key_Pressing(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_PRESSING)
		return true;

	return false;
}

// ������ ���� ���� ����, ���� ������ ���

bool GameEngine::InputManager::Key_Down(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_DOWN)
		return true;

	return false;
}

// ������ ���� ���� �ְ�, ���� ������ �ʾ��� ���
bool GameEngine::InputManager::Key_Up(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_UP)
		return true;

	return false;
}

void GameEngine::InputManager::Update_Key(HWND _hwnd)
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] & (KEY_PRESSING | KEY_DOWN)) //������ ���� ���� ���� ��
		{
			if (GetAsyncKeyState(i) & 0x8000) //�̹��� ������ ��
				m_bKeyState[i] = KEY_PRESSING;

			else //�̹��� �� ������ ��
				m_bKeyState[i] = KEY_UP;
		}
		else //������ ���� �� ���� ��
		{
			if (GetAsyncKeyState(i) & 0x8000) //�̹��� ������ ��
				m_bKeyState[i] = KEY_DOWN;

			else //�̹��� �� ������ ��
				m_bKeyState[i] = KEY_IDLE;
		}
	}

#ifdef EDITOR

#else
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(_hwnd, &mouse);
	m_MousePos = Vector3{ static_cast<float>(mouse.x), static_cast<float>(mouse.y), 0.f };
#endif
}

void GameEngine::InputManager::Update_Editor(HWND _hwnd, const Vector3& _mousePos)
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] & (KEY_PRESSING | KEY_DOWN)) //������ ���� ���� ���� ��
		{
			if (GetAsyncKeyState(i) & 0x8000) //�̹��� ������ ��
				m_bKeyState[i] = KEY_PRESSING;

			else //�̹��� �� ������ ��
				m_bKeyState[i] = KEY_UP;
		}
		else //������ ���� �� ���� ��
		{
			if (GetAsyncKeyState(i) & 0x8000) //�̹��� ������ ��
				m_bKeyState[i] = KEY_DOWN;

			else //�̹��� �� ������ ��
				m_bKeyState[i] = KEY_IDLE;
		}
	}

	m_MousePos = _mousePos;
}
