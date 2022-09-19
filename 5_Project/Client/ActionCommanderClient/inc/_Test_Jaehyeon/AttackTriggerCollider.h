#pragma once

#include "ComponentBase.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class ColliderBase;
}

class AttackTriggerCollider :
	public SanyoniEngine::ComponentBase,
	public SanyoniEngine::ITriggerComponent
{
public:
	AttackTriggerCollider(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("AttackTriggerCollider"));

	void OnStart() override;

	void Tick() override;

	void OnTriggerEnter(ColliderBase* _other) override;

	void OnTriggerStay(ColliderBase* _other) override;

	void OnTriggerExit(ColliderBase* _other) override;

	void SetColliderSize(float _width, float _height);

private:

	size_t m_StayCount;

};

