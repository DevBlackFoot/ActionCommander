#pragma once
#include "ComponentBase.h"

class GoblinBossAnimation :
	public ComponentBase
{
public:
	GoblinBossAnimation(GameObject* _gameObject, const tstring& _typeName = TEXT("GoblinBossAnimation"));

	void PostTick() override;

private:
	bool Attack();

	bool m_bDoAttack;

public:
	void DoAttack() { m_bDoAttack = true; }
};

