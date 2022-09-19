#pragma once
#include "ComponentBase.h"
#include "ComponentInterfaces.h"

class Heart :
	public ComponentBase,
	public ITriggerComponent
{
public:
	Heart(GameObject* _gameObject, const tstring& _typeName = TEXT("Heart"));

	void OnTriggerEnter(ColliderBase* _other) override;

private:
	int m_HealAmount;
};
