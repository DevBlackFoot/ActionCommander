#pragma once

#include "ComponentBase.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class ColliderBase;
}

class FollowMouse :
	public SanyoniEngine::ComponentBase,
	public SanyoniEngine::ITriggerComponent
{
public:
	FollowMouse(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("FollowMouse"));

	void OnStart() override;

	void Tick() override;

	void OnTriggerEnter(SanyoniEngine::ColliderBase* _other) override;

	void OnTriggerStay(SanyoniEngine::ColliderBase* _other) override;

	void OnTriggerExit(SanyoniEngine::ColliderBase* _other) override;

private:
	size_t m_StayCount;
};
