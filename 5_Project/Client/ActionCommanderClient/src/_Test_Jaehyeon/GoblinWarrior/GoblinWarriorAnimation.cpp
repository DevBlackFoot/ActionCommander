#include "pch.h"
#include "GoblinWarrior/GoblinWarriorAnimation.h"

#include "Animator.h"
#include "Core.h"
#include "GameObject.h"
#include "Health.h"
#include "GoblinWarrior/GoblinWarrior.h"
#include "GoblinWarrior/GoblinWarriorRigidBodyController.h"

GoblinWarriorAnimation::GoblinWarriorAnimation(GameObject* _gameObject, const tstring& _typeName)
	: ComponentBase(_gameObject, _typeName)
{
}

void GoblinWarriorAnimation::PostTick()
{
	ComponentBase::PostTick();

	// 이 컴포넌트를 가지고 있는 게임오브젝트의 컴포넌트를 검사합니다.
	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _goblinWrrior = GetGameObject()->GetComponent<GoblinWarrior>();
	if (_goblinWrrior == nullptr)
		return;

	const auto _controller = GetGameObject()->GetComponent<GoblinWarriorRigidBodyController>();
	if (_controller == nullptr)
		return;

	if (_animator->GetCurrentClip() == nullptr)
	{
		_animator->SetCurrentClip(_goblinWrrior->GetIdleAnimClip());
	}

	/*
	 * 다른 one shot 애니메이션이 재생중이여도
	 * 실행 조건에 해당된다면 바로 재생되어야 하는
	 * 우선순위가 높은 애니메이션들을 재생합니다.
	 */

	 // 죽는 애니메이션일 때는 바로 리턴
	if (_animator->GetCurrentClip() == _goblinWrrior->GetDeadAnimClip())
		return;

	if (_animator->GetCurrentClip() == _goblinWrrior->GetHitAnimClip()
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}
	else
	{
		_controller->SetChaseSpeed(250.f);
	}


	if (Attack() == true)
		return;

	if (_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}
	// Idle의 조건..

	/*if (_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == true)
	{
		_animator->SetCurrentClip(_goblinWrrior->GetIdleAnimClip());
	}*/

	if(MathHelper::AlmostEquals(_controller->deltaVelocity.x, 0) == true)
	{
		_animator->SetCurrentClip(_goblinWrrior->GetIdleAnimClip());
	}

	if(_controller->deltaVelocity.x > 0)
	{
		_animator->SetCurrentClip(_goblinWrrior->GetWalkAnimClip());
	}
	else if(_controller->deltaVelocity.x < 0)
	{
		_animator->SetCurrentClip(_goblinWrrior->GetWalkAnimClip());
	
	}
	 // 플레이어의 경우 => attack, skill, dash 등등의 순
}

bool GoblinWarriorAnimation::Attack()
{
	if (m_bDoAttack == false)
		return false;

	m_bDoAttack = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _goblinWarrior = GetGameObject()->GetComponent<GoblinWarrior>();

	if (_animator == nullptr
		|| _goblinWarrior == nullptr)
		return false;

	// 연속 공격을 위한 클립 전환
	if (_animator->GetCurrentClip() == _goblinWarrior->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
		)
	{
		_animator->SetCurrentClip(_goblinWarrior->GetIdleAnimClip());
	}

	// 현재 공격 중이 아닐때.. 공격 애니메이션을 재생합니다.
	if (_goblinWarrior->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_goblinWarrior->GetAttackAnimClip());
		return true;
	}

	return false;
}