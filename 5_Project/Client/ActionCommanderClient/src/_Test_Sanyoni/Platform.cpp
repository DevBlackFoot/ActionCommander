#include "pch.h"
#include "Platform.h"

Platform::Platform(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_AllowJumpThrough(true),
	m_AllowJumpDown(true)
{
}
