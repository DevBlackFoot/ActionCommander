#include "pch.h"
#include "DetectTriggerCollider.h"

#include "AABBCollider.h"
#include "GameObject.h"
#include "Screen.h"
#include "Nepen/Nepen.h"
#include "GoblinWarrior/GoblinWarrior.h"
#include "GoblinWarrior/GoblinWarriorRigidBodyController.h"
#include "GoblinArcher/GoblinArcher.h"
#include "GoblinArcher/GoblinArcherRigidBodyController.h"
#include "GoblinBoss/GoblinBoss.h"
#include "GoblinBoss/GoblinBossRigidBodyController.h"

DetectTriggerCollider::DetectTriggerCollider(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	m_GameObject->SetTag(EGameObjectTag::Enemy);

	const auto _collider = m_GameObject->AddComponent<AABBCollider>();
	_collider->SetWidth(300);
	_collider->SetHeight(50);
	_collider->SetIsTrigger(true);
}

void DetectTriggerCollider::OnStart()
{

}

void DetectTriggerCollider::Tick()
{
	m_GameObject->GetComponent<ColliderBase>()->SetDebugBoundsColor(m_StayCount > 0 ? Color::Blue : Color::Gray);
}

void DetectTriggerCollider::OnTriggerEnter(ColliderBase* _other)
{
	if (_other->GetGameObject()->GetTag() == EGameObjectTag::Player)
	{
		Vector2 _playerPos = _other->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		Vector2 _enemyPos = m_GameObject->GetComponent<Transform>()->GetWorldPosition();

		Vector2 _dir = (_enemyPos - _playerPos);
		_dir = _dir.Normalize();

		const auto _goblinWarrior = m_GameObject->GetParent()->GetComponent<GoblinWarrior>();
		const auto _goblinWarriorController = m_GameObject->GetParent()->GetComponent<GoblinWarriorRigidBodyController>();
		if (_goblinWarrior != nullptr && _goblinWarriorController != nullptr)
		{
			_goblinWarrior->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinWarriorController->Horizontal(_goblinWarrior->GetLookDirection());
			_goblinWarriorController->DoChase();
		}

		const auto _goblinArcher = m_GameObject->GetParent()->GetComponent<GoblinArcher>();
		const auto _goblinArcherController = m_GameObject->GetParent()->GetComponent<GoblinArcherRigidBodyController>();
		if (_goblinArcher != nullptr && _goblinArcherController != nullptr)
		{
			_goblinArcher->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinArcherController->Horizontal(_goblinArcher->GetLookDirection());
			_goblinArcherController->DoChase();
		}

		const auto _goblinBoss = m_GameObject->GetParent()->GetComponent<GoblinBoss>();
		const auto _goblinBossController = m_GameObject->GetParent()->GetComponent<GoblinBossRigidBodyController>();
		if (_goblinBoss != nullptr && _goblinBossController != nullptr)
		{
			_goblinBoss->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinBossController->Horizontal(_goblinBoss->GetLookDirection());
			_goblinBossController->DoChase();
		}

		// TODO 워리어와 아처의 경우 추격 시작.
	}
}

void DetectTriggerCollider::OnTriggerStay(ColliderBase* _other)
{

	if (_other->GetGameObject()->GetTag() == EGameObjectTag::Player)
	{
		// 바라보는 방향 확인.
		Vector2 _playerPos = _other->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
		Vector2 _enemyPos = m_GameObject->GetComponent<Transform>()->GetWorldPosition();

		Vector2 _dir = (_enemyPos - _playerPos);
		float _dirMag = _dir.Magnitude();
		_dir = _dir.Normalize();

		const auto _nepen = m_GameObject->GetParent()->GetComponent<Nepen>();
		if (_nepen != nullptr)
		{
			_nepen->SetLookDirection(_dir.x < 0 ? 1 : -1);
		}

		const auto _goblinWarrior = m_GameObject->GetParent()->GetComponent<GoblinWarrior>();
		const auto _goblinWarriorController = m_GameObject->GetParent()->GetComponent<GoblinWarriorRigidBodyController>();
		if (_goblinWarrior != nullptr && _goblinWarriorController != nullptr)
		{
			_goblinWarrior->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinWarriorController->Horizontal(_goblinWarrior->GetLookDirection());
			if (_dirMag < 100)
			{
				_goblinWarriorController->SetDesiredDeltaVelocityX(0);
				
			}
			else
			{
				_goblinWarriorController->DoChase();
				_goblinWarriorController->SetDesiredDeltaVelocityX(_dirMag);
			}
		}

		const auto _goblinArcher = m_GameObject->GetParent()->GetComponent<GoblinArcher>();
		const auto _goblinArcherController = m_GameObject->GetParent()->GetComponent<GoblinArcherRigidBodyController>();
		if (_goblinArcher != nullptr && _goblinArcherController != nullptr)
		{
			_goblinArcher->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinArcherController->Horizontal(_goblinArcher->GetLookDirection());
			if (_dirMag < 180)
			{
				_goblinArcherController->SetDesiredDeltaVelocityX(0);

			}
			else
			{
				_goblinArcherController->DoChase();
				_goblinArcherController->SetDesiredDeltaVelocityX(_dirMag);
			}
		}

		const auto _goblinBoss = m_GameObject->GetParent()->GetComponent<GoblinBoss>();
		const auto _goblinBossController = m_GameObject->GetParent()->GetComponent<GoblinBossRigidBodyController>();
		if (_goblinBoss != nullptr && _goblinBossController != nullptr)
		{
			_goblinBoss->SetLookDirection(_dir.x < 0 ? 1 : -1);
			_goblinBossController->Horizontal(_goblinBoss->GetLookDirection());
			if (_dirMag < 80)
			{
				_goblinBossController->Stop();

			}
			else
			{
				_goblinBossController->DoChase();
				_goblinBossController->SetDesiredDeltaVelocityX(_dirMag);
			}
		}
	}
}

void DetectTriggerCollider::OnTriggerExit(ColliderBase* _other)
{
	if (_other->GetGameObject()->GetTag() == EGameObjectTag::Player)
	{
		const auto _goblinWarriorController = m_GameObject->GetParent()->GetComponent<GoblinWarriorRigidBodyController>();
		if (_goblinWarriorController != nullptr)
		{
			_goblinWarriorController->SetChase(false);
		}

		// TODO 워리어와 아처의 경우 추격 종료.
	}

	const auto _goblinArcherController = m_GameObject->GetParent()->GetComponent<GoblinArcherRigidBodyController>();
	if (_goblinArcherController != nullptr)
	{
		_goblinArcherController->SetChase(false);
	}

	const auto _goblinBossController = m_GameObject->GetParent()->GetComponent<GoblinBossRigidBodyController>();
	if (_goblinBossController != nullptr)
	{
		_goblinBossController->SetChase(false);
	}
}

void DetectTriggerCollider::SetColliderSize(float _width, float _height)
{
	const auto _collider = m_GameObject->GetComponent<AABBCollider>();

	if (_collider != nullptr)
	{
		_collider->SetWidth(_width);
		_collider->SetHeight(_height);
	}


	/*
	m_GameObject->GetComponent<AABBCollider>()->SetWidth(_width);
	m_GameObject->GetComponent<AABBCollider>()->SetHeight(_height)*/;
}