#include "InputManager.h"

IMPLEMENT_SINGLETON(GameEngine::InputManager)

GameEngine::InputManager::InputManager(): m_MouseClientPos()
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

//눌리고 있을 때
bool GameEngine::InputManager::Key_Pressing(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_PRESSING)
		return true;

	return false;
}

// 이전에 눌린 적이 없고, 현재 눌렸을 경우

bool GameEngine::InputManager::Key_Down(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_DOWN)
		return true;

	return false;
}

// 이전에 눌린 적이 있고, 현재 눌리지 않았을 경우
bool GameEngine::InputManager::Key_Up(const int _iKey) const
{
	if (m_bKeyState[_iKey] == KEY_UP)
		return true;

	return false;
}

void GameEngine::InputManager::Update_Key()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] & (KEY_PRESSING | KEY_DOWN)) //이전에 눌린 적이 있을 때
		{
			if (GetAsyncKeyState(i) & 0x8000) //이번에 눌렸을 때
				m_bKeyState[i] = KEY_PRESSING;

			else //이번에 안 눌렸을 때
				m_bKeyState[i] = KEY_UP;
		}
		else //이전에 눌린 적 없을 때
		{
			if (GetAsyncKeyState(i) & 0x8000) //이번에 눌렸을 때
				m_bKeyState[i] = KEY_DOWN;

			else //이번에 안 눌렸을 때
				m_bKeyState[i] = KEY_IDLE;
		}
	}
}