#pragma once
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;
}

class Platform :
	public SanyoniEngine::ComponentBase
{
public:
	Platform(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("Platform"));

private:
	bool m_AllowJumpThrough;

	bool m_AllowJumpDown;

public:
	inline bool GetAllowJumpThrough() const { return m_AllowJumpThrough; }

	inline bool GetAllowDownJump() const { return m_AllowJumpDown; }

	void SetAllowJumpThrough(bool _allow) { m_AllowJumpThrough = _allow; }

	void SetAllowJumpDown(bool _allow) { m_AllowJumpDown = _allow; }
};
