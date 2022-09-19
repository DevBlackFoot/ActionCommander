#include "pch.h"
#include "GoblinArcher/GoblinArcherAnimation.h"

#include "Animator.h"
#include "Core.h"
#include "GameObject.h"
#include "GoblinArcher/GoblinArcher.h"
#include "GoblinArcher/GoblinArcherRigidBodyController.h"

GoblinArcherAnimation::GoblinArcherAnimation(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void GoblinArcherAnimation::PostTick()
{
	ComponentBase::PostTick();

	// �� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� ������Ʈ�� �˻��մϴ�.
	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _goblinArcher = GetGameObject()->GetComponent<GoblinArcher>();
	if (_goblinArcher == nullptr)
		return;

	const auto _controller = GetGameObject()->GetComponent<GoblinArcherRigidBodyController>();
	if (_controller == nullptr)
		return;

	if (_animator->GetCurrentClip() == nullptr)
	{
		_animator->SetCurrentClip(_goblinArcher->GetIdleAnimClip());
	}

	/*
	 * �ٸ� one shot �ִϸ��̼��� ������̿���
	 * ���� ���ǿ� �ش�ȴٸ� �ٷ� ����Ǿ�� �ϴ�
	 * �켱������ ���� �ִϸ��̼ǵ��� ����մϴ�.
	 */

	 // �״� �ִϸ��̼��� ���� �ٷ� ����
	if (_animator->GetCurrentClip() == _goblinArcher->GetDeadAnimClip())
		return;

	if (_animator->GetCurrentClip() == _goblinArcher->GetHitAnimClip()
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}
	else
	{
		_controller->SetChaseSpeed(200.f);
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
		_animator->SetCurrentClip(_goblinArcher->GetIdleAnimClip());
	}*/

	if (MathHelper::AlmostEquals(_controller->deltaVelocity.x, 0) == true)
	{
		_animator->SetCurrentClip(_goblinArcher->GetIdleAnimClip());
	}

	if (_controller->deltaVelocity.x > 0)
	{
		_animator->SetCurrentClip(_goblinArcher->GetWalkAnimClip());
	}
	else if (_controller->deltaVelocity.x < 0)
	{
		_animator->SetCurrentClip(_goblinArcher->GetWalkAnimClip());

	}
	 // �÷��̾��� ��� => attack, skill, dash ����� ��
}

bool GoblinArcherAnimation::Attack()
{
	if (m_bDoAttack == false)
		return false;

	m_bDoAttack = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _goblinArcher = GetGameObject()->GetComponent<GoblinArcher>();

	if (_animator == nullptr
		|| _goblinArcher == nullptr)
		return false;

	// ���� ������ ���� Ŭ�� ��ȯ
	if (_animator->GetCurrentClip() == _goblinArcher->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
		)
	{
		_animator->SetCurrentClip(_goblinArcher->GetIdleAnimClip());
	}

	// ���� ���� ���� �ƴҶ�.. ���� �ִϸ��̼��� ����մϴ�. => �ִϸ��̼��� ������ ���� ���� ������
		// true�� ���·� �ٲ��� ����.
	if (_goblinArcher->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_goblinArcher->GetAttackAnimClip());
		return true;
	}

	return false;
}