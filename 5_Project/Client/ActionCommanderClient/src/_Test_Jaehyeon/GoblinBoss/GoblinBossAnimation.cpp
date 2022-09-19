#include "pch.h"
#include "GoblinBoss/GoblinBossAnimation.h"

#include "Animator.h"
#include "Core.h"
#include "GameObject.h"
#include "Health.h"
#include "GoblinBoss/GoblinBoss.h"
#include "GoblinBoss/GoblinBossRigidBodyController.h"

GoblinBossAnimation::GoblinBossAnimation(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void GoblinBossAnimation::PostTick()
{
	ComponentBase::PostTick();

	// 이 컴포넌트를 가지고 있는 게임오브젝트의 컴포넌트를 검사합니다.
	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _goblinBoss = GetGameObject()->GetComponent<GoblinBoss>();
	if (_goblinBoss == nullptr)
		return;

	const auto _controller = GetGameObject()->GetComponent<GoblinBossRigidBodyController>();
	if (_controller == nullptr)
		return;

	if (_animator->GetCurrentClip() == nullptr)
	{
		_animator->SetCurrentClip(_goblinBoss->GetIdleAnimClip());
	}

	/*
	 * 다른 one shot 애니메이션이 재생중이여도
	 * 실행 조건에 해당된다면 바로 재생되어야 하는
	 * 우선순위가 높은 애니메이션들을 재생합니다.
	 */

	 // 죽는 애니메이션일 때는 바로 리턴
	if (_animator->GetCurrentClip() == _goblinBoss->GetDeadAnimClip())
		return;

	if (Attack() == true)
		return;

	if (_animator->GetCurrentClip() == _goblinBoss->GetHitAnimClip()
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}
	else
	{
		_controller->SetChaseSpeed(150.f);
	}

	if (_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}

	// Idle의 조건..
	/*if (_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == true)
	{
		_animator->SetCurrentClip(_goblinBoss->GetIdleAnimClip());
	}*/

	if (MathHelper::AlmostEquals(_controller->deltaVelocity.x, 0) == true)
	{
		_animator->SetCurrentClip(_goblinBoss->GetIdleAnimClip());
	}

	if (_controller->deltaVelocity.x > 0)
	{
		_animator->SetCurrentClip(_goblinBoss->GetWalkAnimClip());
	}
	else if (_controller->deltaVelocity.x < 0)
	{
		_animator->SetCurrentClip(_goblinBoss->GetWalkAnimClip());

	}
	// 플레이어의 경우 => attack, skill, dash 등등의 순
}

bool GoblinBossAnimation::Attack()
{
	if (m_bDoAttack == false)
		return false;

	m_bDoAttack = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _goblinBoss = GetGameObject()->GetComponent<GoblinBoss>();

	if (_animator == nullptr
		|| _goblinBoss == nullptr)
		return false;

	// 연속 공격을 위한 클립 전환
	if (_animator->GetCurrentClip() == _goblinBoss->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
		)
	{
		_animator->SetCurrentClip(_goblinBoss->GetIdleAnimClip());
	}

	// 현재 공격 중이 아닐때.. 공격 애니메이션을 재생합니다.
	if (_goblinBoss->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_goblinBoss->GetAttackAnimClip());
		return true;
	}

	return false;
}
