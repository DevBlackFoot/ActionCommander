#include "pch.h"
#include "Nepen/NepenAnimation.h"

#include "Animator.h"
#include "Core.h"
#include "GameObject.h"
#include "Health.h"
#include "Nepen/Nepen.h"
#include "Nepen/NepenRigidBodyController.h"

NepenAnimation::NepenAnimation(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void NepenAnimation::PostTick()
{
	ComponentBase::PostTick();

	// �� ������Ʈ�� ������ �ִ� ���ӿ�����Ʈ�� ������Ʈ�� �˻��մϴ�.
	const auto _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _nepen = GetGameObject()->GetComponent<Nepen>();
	if (_nepen == nullptr)
		return;

	const auto _controller = GetGameObject()->GetComponent<NepenRigidBodyController>();
	if(_controller == nullptr)
		return;
	

	if(_animator->GetCurrentClip() == nullptr)
	{
		_animator->SetCurrentClip(_nepen->GetIdleAnimClip());
	}

	/*
	 * �ٸ� one shot �ִϸ��̼��� ������̿���
	 * ���� ���ǿ� �ش�ȴٸ� �ٷ� ����Ǿ�� �ϴ�
	 * �켱������ ���� �ִϸ��̼ǵ��� ����մϴ�.
	 */

	if(_animator->GetCurrentClip() == _nepen->GetDeadAnimClip())
	{
		return;
	}


	if (Attack() == true)
		return;
	
	if (_animator->GetCurrentClip() == _nepen->GetHitAnimClip()
		&& _animator->GetReachedEnd() == false)
	{
		return;
	}
	// Idle�� ����..
	if(_animator->GetCurrentClip()->bIsLooping == false
		&& _animator->GetReachedEnd() == true)
	{
		_animator->SetCurrentClip(_nepen->GetIdleAnimClip());
	}


	// �÷��̾��� ��� => attack, skill, dash ����� ��
}

bool NepenAnimation::Attack()
{
	if (m_bDoAttack == false)
		return false;

	m_bDoAttack = false;

	const auto _animator = GetGameObject()->GetComponent<Animator>();
	const auto _nepen = GetGameObject()->GetComponent<Nepen>();

	if (_animator == nullptr
		|| _nepen == nullptr)
		return false;

	// ���� ������ ���� Ŭ�� ��ȯ
	if(_animator->GetCurrentClip() == _nepen->GetAttackAnimClip()
		&& _animator->GetReachedEnd() == true
	)
	{
		_animator->SetCurrentClip(_nepen->GetIdleAnimClip());
	}

	// ���� ���� ���� �ƴҶ�.. ���� �ִϸ��̼��� ����մϴ�.
	if(_nepen->GetAttacking() == false)
	{
		_animator->SetCurrentClip(_nepen->GetAttackAnimClip());
		return true;
	}

	return false;
}

