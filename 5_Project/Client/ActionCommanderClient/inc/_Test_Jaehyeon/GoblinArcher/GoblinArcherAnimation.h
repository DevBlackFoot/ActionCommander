# pragma once
#include "ComponentBase.h"

class GoblinArcherAnimation :
	public ComponentBase
{
public :
	GoblinArcherAnimation(GameObject* _gameObject, const tstring& _typeName = TEXT("GoblinArcherAnimation"));

	void PostTick() override;

private:
	bool Attack();

	bool m_bDoAttack;

public:
	void DoAttack() { m_bDoAttack = true; }
};

