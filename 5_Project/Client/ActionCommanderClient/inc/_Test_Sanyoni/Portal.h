#pragma once
#include "ComponentBase.h"
#include "ComponentInterfaces.h"

class Portal :
	public ComponentBase,
	public ITriggerComponent
{
public:
	Portal(GameObject* _gameObject, const tstring& _typeName = TEXT("Portal"));

	void OnTriggerEnter(ColliderBase* _other) override;
};
