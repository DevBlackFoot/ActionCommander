#include "pch.h"
#include "AttackTriggerCollider.h"

#include "AABBCollider.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Screen.h"
#include "Nepen/Nepen.h"
#include "Nepen/NepenAnimation.h"
#include "GoblinWarrior/GoblinWarrior.h"
#include "GoblinWarrior/GoblinWarriorAnimation.h"
#include "GoblinArcher/GoblinArcher.h"
#include "GoblinArcher/GoblinArcherAnimation.h"
#include "GoblinBoss/GoblinBoss.h"
#include "GoblinBoss/GoblinBossAnimation.h"


AttackTriggerCollider::AttackTriggerCollider(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
	m_GameObject->SetTag(EGameObjectTag::Enemy);

	const auto _collider = m_GameObject->AddComponent<AABBCollider>();
	_collider->SetWidth(60);
	_collider->SetHeight(40);
	_collider->SetIsTrigger(true);
}

void AttackTriggerCollider::OnStart()
{
}

void AttackTriggerCollider::Tick()
{
	m_GameObject->GetComponent<ColliderBase>()->SetDebugBoundsColor(m_StayCount > 0 ? Color::Gray : Color::Blue);
}

void AttackTriggerCollider::OnTriggerEnter(ColliderBase* _other)
{
	// Attack 애니메이션 재생.
	if (_other->GetGameObject()->GetTag() == EGameObjectTag::Player)
	{
		const auto _nepen = m_GameObject->GetParent()->GetComponent<Nepen>();
		const auto _nepenAnim = m_GameObject->GetParent()->GetComponent<NepenAnimation>();
		if (_nepen != nullptr && _nepenAnim != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _nepen->GetLastEndAttackTimeMillis() > 1500)
			{
				_nepenAnim->DoAttack();
			}
		}

		const auto _goblinWarriorAnim = m_GameObject->GetParent()->GetComponent<GoblinWarriorAnimation>();
		const auto _goblinWarrior = m_GameObject->GetParent()->GetComponent<GoblinWarrior>();
		if (_goblinWarrior != nullptr && _goblinWarriorAnim != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinWarrior->GetLastEndAttackTimeMillis() > 1500)
			{
				_goblinWarriorAnim->DoAttack();
			}
		}

		const auto _goblinArcherAnim = m_GameObject->GetParent()->GetComponent<GoblinArcherAnimation>();
		const auto _goblinArcher = m_GameObject->GetParent()->GetComponent<GoblinArcher>();
		if (_goblinArcherAnim != nullptr && _goblinArcher != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinArcher->GetLastEndAttackTimeMillis() > 3000)
			{
				_goblinArcherAnim->DoAttack();
			}
		}

		const auto _goblinBossAnim = m_GameObject->GetParent()->GetComponent<GoblinBossAnimation>();
		const auto _goblinBoss = m_GameObject->GetParent()->GetComponent<GoblinBoss>();
		if (_goblinBossAnim != nullptr && _goblinBoss != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinBoss->GetLastEndAttackTimeMillis() > 2000)
			{
				_goblinBossAnim->DoAttack();
			}
		}
	}
}

void AttackTriggerCollider::OnTriggerStay(ColliderBase* _other)
{
	// 내부에 있을 때도 지속적으로 어택을 바꿔준다..
	// Start / End Attack 이벤트를 처리하면, 연속 공격 가능(아마)
	if (_other->GetGameObject()->GetTag() == EGameObjectTag::Player)
	{
		const auto _nepen = m_GameObject->GetParent()->GetComponent<Nepen>();
		const auto _nepenAnim = m_GameObject->GetParent()->GetComponent<NepenAnimation>();
		if (_nepen != nullptr && _nepenAnim != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _nepen->GetLastEndAttackTimeMillis() > 1500)
			{
				_nepenAnim->DoAttack();
			}
		}

		const auto _goblinWarriorAnim = m_GameObject->GetParent()->GetComponent<GoblinWarriorAnimation>();
		const auto _goblinWarrior = m_GameObject->GetParent()->GetComponent<GoblinWarrior>();
		if (_goblinWarrior != nullptr && _goblinWarriorAnim != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinWarrior->GetLastEndAttackTimeMillis() > 1500)
			{
				_goblinWarriorAnim->DoAttack();
			}
		}

		const auto _goblinArcherAnim = m_GameObject->GetParent()->GetComponent<GoblinArcherAnimation>();
		const auto _goblinArcher = m_GameObject->GetParent()->GetComponent<GoblinArcher>();
		if (_goblinArcherAnim != nullptr && _goblinArcher != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinArcher->GetLastEndAttackTimeMillis() > 2500)
			{
				_goblinArcherAnim->DoAttack();
			}
		}

		const auto _goblinBossAnim = m_GameObject->GetParent()->GetComponent<GoblinBossAnimation>();
		const auto _goblinBoss = m_GameObject->GetParent()->GetComponent<GoblinBoss>();
		if (_goblinBossAnim != nullptr && _goblinBoss != nullptr)
		{
			if (GameTime::Instance.GetGameTimeMillis() - _goblinBoss->GetLastEndAttackTimeMillis() > 2000)
			{
				_goblinBossAnim->DoAttack();
			}
		}
	}
}

void AttackTriggerCollider::OnTriggerExit(ColliderBase* _other)
{
}

void AttackTriggerCollider::SetColliderSize(float _width, float _height)
{
	const auto _collider = m_GameObject->GetComponent<AABBCollider>();

	if (_collider != nullptr)
	{
		_collider->SetWidth(_width);
		_collider->SetHeight(_height);
	}
}
