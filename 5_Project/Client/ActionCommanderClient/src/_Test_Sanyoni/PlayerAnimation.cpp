#include "pch.h"
#include "PlayerAnimation.h"

#include "Animator.h"
#include "Core.h"
#include "GameObject.h"
#include "Player.h"
#include "PlayerRigidBodyController.h"

PlayerAnimation::PlayerAnimation(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void PlayerAnimation::PostTick() //포스틱 케챱 찍어먹으면 맛있음 by 안재우
{
	ComponentBase::PostTick();

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _player = GetGameObject()->GetComponent<Player>();
	if (_player == nullptr)
		return;

	const auto _controller = GetGameObject()->GetComponent<PlayerRigidBodyController>();
	if (_controller == nullptr)
		return;

	if (_animator->GetCurrentClip() == nullptr)
	{
		// 초기에 아무런 클립도 재생하지 않는다면,
		// Idle clip을 재생합니다.
		_animator->SetCurrentClip(_player->GetIdleAnimClip());
	}

	/*
	 * 다른 one shot 애니메이션이 재생중이여도
	 * 실행 조건에 해당된다면 바로 재생되어야 하는
	 * 우선순위가 높은 애니메이션들을 재생합니다.
	 */

	if (BasicAttack() == true)
		return;

	if (Skill1() == true)
		return;

	if (Skill2() == true)
		return;

	if (Skill3() == true)
		return;

	if (Skill4() == true)
		return;

	if (_controller->GetStartDashThisFrame() == true)
	{
		_animator->SetCurrentClip(_player->GetDashAnimClip());
		return;
	}

	if (_controller->GetDidJumpThisFrame() == true)
	{
		_animator->SetCurrentClip(_player->GetJumpStartAnimClip());
		return;
	}


	if (_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}


	if (_controller->GetInAirLastFrame())
	{
		// 캐릭터가 공중에 떠 있을 때의 애니메이션을 재생합니다.
		_animator->SetCurrentClip(_player->GetAirborneAnimClip());
		return;
	}


	if (MathHelper::AlmostEquals(_controller->deltaVelocity.x, 0) == true)
	{
		_animator->SetCurrentClip(_player->GetIdleAnimClip());
	}
	if (_controller->deltaVelocity.x > 0)
	{
		// 오른쪽으로 이동하는 애니메이션을 재생합니다.
		_animator->SetCurrentClip(_player->GetRunAnimClip());
	}
	else if (_controller->deltaVelocity.x < 0)
	{
		// 왼쪽으로 이동하는 애니메이션을 재생합니다.
		_animator->SetCurrentClip(_player->GetRunAnimClip());
	}
}

bool PlayerAnimation::BasicAttack()
{
	if (m_bDoBasicAttack == false)
		return false;

	m_bDoBasicAttack = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _player = GetGameObject()->GetComponent<Player>();

	if (_animator == nullptr
		|| _player == nullptr)
		return false;

	if (_player->GetAttacking() == false
		|| (_player->GetAttacking() == true && _player->GetCanGoNextAttack() == true))
	{
		// 공격중이 아니거나
		// 공격 중이지만 다음 공격으로 넘어갈 수 있을 때
		// 다음 공격을 실행합니다.

		const auto _targetClip = _player->GetBasicAttackStack() == 0
			                         ? _player->GetBasicAttack1AnimClip()
			                         : _player->GetBasicAttackStack() == 1
			                         ? _player->GetBasicAttack2AnimClip()
			                         : _player->GetBasicAttack3AnimClip();

		_animator->SetCurrentClip(_targetClip);
		return true;
	}

	return false;
}

bool PlayerAnimation::Skill1()
{
	if (m_bDoSkill1 == false)
		return false;

	m_bDoSkill1 = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _player = GetGameObject()->GetComponent<Player>();

	if (_animator == nullptr
		|| _player == nullptr)
		return false;

	if (_player->GetAttacking() == false
		|| (_player->GetAttacking() == true && _player->GetCanGoNextAttack() == true))
	{
		// 공격중이 아니거나
		// 공격 중이지만 다음 공격으로 넘어갈 수 있을 때에만 재생됩니다.

		_animator->SetCurrentClip(_player->GetSkill1AnimClip());
		return true;
	}

	return false;
}

bool PlayerAnimation::Skill2()
{
	if (m_bDoSkill2 == false)
		return false;

	m_bDoSkill2 = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _player = GetGameObject()->GetComponent<Player>();

	if (_animator == nullptr
		|| _player == nullptr)
		return false;

	if (_player->GetAttacking() == false
		|| (_player->GetAttacking() == true && _player->GetCanGoNextAttack() == true))
	{
		// 공격중이 아니거나
		// 공격 중이지만 다음 공격으로 넘어갈 수 있을 때에만 재생됩니다.

		_animator->SetCurrentClip(_player->GetSkill2AnimClip());
		return true;
	}

	return false;
}

bool PlayerAnimation::Skill3()
{
	if (m_bDoSkill3 == false)
		return false;

	m_bDoSkill3 = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _player = GetGameObject()->GetComponent<Player>();

	if (_animator == nullptr
		|| _player == nullptr)
		return false;

	if (_player->GetAttacking() == false
		|| (_player->GetAttacking() == true && _player->GetCanGoNextAttack() == true))
	{
		// 공격중이 아니거나
		// 공격 중이지만 다음 공격으로 넘어갈 수 있을 때에만 재생됩니다.

		_animator->SetCurrentClip(_player->GetSkill3AnimClip());
		return true;
	}

	return false;
}

bool PlayerAnimation::Skill4()
{
	if (m_bDoSkill4 == false)
		return false;

	m_bDoSkill4 = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _player = GetGameObject()->GetComponent<Player>();

	if (_animator == nullptr
		|| _player == nullptr)
		return false;

	if (_player->GetAttacking() == false
		|| (_player->GetAttacking() == true && _player->GetCanGoNextAttack() == true))
	{
		// 공격중이 아니거나
		// 공격 중이지만 다음 공격으로 넘어갈 수 있을 때에만 재생됩니다.

		_animator->SetCurrentClip(_player->GetSkill4AnimClip());
		return true;
	}

	return false;
}
