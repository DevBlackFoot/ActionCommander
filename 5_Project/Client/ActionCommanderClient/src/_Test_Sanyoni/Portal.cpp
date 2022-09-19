#include "pch.h"
#include "Portal.h"

#include "AABBCollider.h"
#include "ActionCommanderGameManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

Portal::Portal(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	const auto _collider = GetGameObject()->AddComponent<AABBCollider>();
	_collider->SetWidth(50);
	_collider->SetHeight(50);
	_collider->SetIsTrigger(true);
}

void Portal::OnTriggerEnter(ColliderBase* _other)
{
	if (_other->GetGameObject()->GetTag() != EGameObjectTag::Player)
		return;

	if (const auto _gameManager = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ActionCommanderGameManager>();
		_gameManager != nullptr)
	{
		if (_gameManager->IsStageCleared() == true)
		{
			_gameManager->GoNextLevel();
			return;
		}
	}
}
