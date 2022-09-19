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

	// �� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� ������Ʈ�� �˻��մϴ�.
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
	 * �ٸ� one shot �ִϸ��̼��� ������̿���
	 * ���� ���ǿ� �ش�ȴٸ� �ٷ� ����Ǿ�� �ϴ�
	 * �켱������ ���� �ִϸ��̼ǵ��� ����մϴ�.
	 */

	 // �״� �ִϸ��̼��� ���� �ٷ� ����
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

	// Idle�� ����..
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
	// �÷��̾��� ��� => attack, skill, dash ����� ��
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

	// ���� ������ ���� Ŭ�� ��ȯ
	if (_animator->GetCurrentClip() == _goblinBoss->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
		)
	{
		_animator->SetCurrentClip(_goblinBoss->GetIdleAnimClip());
	}

	// ���� ���� ���� �ƴҶ�.. ���� �ִϸ��̼��� ����մϴ�.
	if (_goblinBoss->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_goblinBoss->GetAttackAnimClip());
		return true;
	}

	return false;
}
