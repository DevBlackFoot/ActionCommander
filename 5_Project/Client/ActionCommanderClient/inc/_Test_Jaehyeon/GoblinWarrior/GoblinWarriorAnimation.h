#pragma once
#include "ComponentBase.h"

class GoblinWarriorAnimation :
	public ComponentBase
{
public:
	GoblinWarriorAnimation(GameObject* _gameObject, const tstring& _typeName = TEXT("GoblinWarriorAnimation"));

	void PostTick() override;

private:
	bool Attack();

	bool m_bDoAttack;

public:
	void DoAttack() { m_bDoAttack = true; }
};

