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

	// �� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� ������Ʈ�� �˻��մϴ�.
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
	 * �ٸ� one shot �ִϸ��̼��� ������̿���
	 * ���� ���ǿ� �ش�ȴٸ� �ٷ� ����Ǿ�� �ϴ�
	 * �켱������ ���� �ִϸ��̼ǵ��� ����մϴ�.
	 */

	 // �״� �ִϸ��̼��� ���� �ٷ� ����
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
	// Idle�� ����..

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
	 // �÷��̾��� ��� => attack, skill, dash ����� ��
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

	// ���� ������ ���� Ŭ�� ��ȯ
	if (_animator->GetCurrentClip() == _goblinWarrior->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
		)
	{
		_animator->SetCurrentClip(_goblinWarrior->GetIdleAnimClip());
	}

	// ���� ���� ���� �ƴҶ�.. ���� �ִϸ��̼��� ����մϴ�.
	if (_goblinWarrior->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_goblinWarrior->GetAttackAnimClip());
		return true;
	}

	return false;
}