#include "Component.h"

#include "GameObject.h"

GameEngine::Transform& GameEngine::Component::Get_Transform() const
{
	return m_Owner->Get_Transform();
}
