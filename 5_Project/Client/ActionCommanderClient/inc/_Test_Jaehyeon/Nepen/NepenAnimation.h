#pragma once
#include "ComponentBase.h"

class NepenAnimation :
	public ComponentBase
{
public:
	NepenAnimation(GameObject* _gameObject, const tstring& _typeName = TEXT("NepenAnimation"));

	void PostTick() override;

private:
	bool Attack();

	bool m_bDoAttack;

public:
	void DoAttack() { m_bDoAttack = true; }
};

