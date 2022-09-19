#pragma once

#include "ComponentBase.h"
#include "ComponentInterfaces.h"

namespace SanyoniEngine
{
	class GameObject;
	class ColliderBase;
}

class DetectTriggerCollider :
	public SanyoniEngine::ComponentBase,
	public SanyoniEngine::ITriggerComponent
{
public:
	DetectTriggerCollider(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("DetectTriggerCollider"));

	void OnStart() override;

	void Tick() override;

	void OnTriggerEnter(ColliderBase* _other) override;

	void OnTriggerStay(ColliderBase* _other) override;

	void OnTriggerExit(ColliderBase* _other) override;

	void SetColliderSize(float _width, float _height);

private:
	// 디버그용 카운트
	size_t m_StayCount;
};

