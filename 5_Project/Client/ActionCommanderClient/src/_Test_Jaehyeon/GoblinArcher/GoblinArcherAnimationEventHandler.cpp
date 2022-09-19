#include "pch.h"
#include "GoblinArcher/GoblinArcherAnimationEventHandler.h"

#include "Animator.h"
#include "Arrow.h"
#include "Core.h"
#include "Damagable.h"
#include "GameObject.h"
#include "OneShotSpriteAnimationEffect.h"
#include "GoblinArcher/GoblinArcher.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "Sound.h"
#include "Screen.h"
#include "GameTime.h"

GoblinArcherAnimationEventHandler::GoblinArcherAnimationEventHandler(GameObject* _gameObject,
                                                                     const tstring& _typeName) :
	UnitAnimationEventHandler(_gameObject, _typeName)
{
}

bool GoblinArcherAnimationEventHandler::OnHandle(const tstring& _functionName, const tstring& _parameter)
{
	GoblinArcher* _goblinArcher = GetGameObject()->GetComponent<GoblinArcher>();
	if (_goblinArcher == nullptr)
		return false;

	Animator* _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return false;

	RigidBodyController* _controller = GetGameObject()->GetComponent<RigidBodyController>();
	if (_controller == nullptr)
		return false;

	const auto _position = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

	const auto _splittedParams = StringHelper::Split(_parameter, TEXT(" "));

	// �Ʒ����� json���Ϸ� ���޹޴� �̺�Ʈ�� ���� ó���� �մϴ�.
	// if�� ã�Ƽ� ó���� return true
	if (_functionName == TEXT("Effect_Follow"))
	{
		const Vector2 _offset{
			std::stof(_splittedParams[0]) * _goblinArcher->GetLookDirection(),
			std::stof(_splittedParams[1])
		};

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		GetGameObject()->AddChild(_effect);
		_effect->GetComponent<Transform>()->SetLocalPosition(_offset);

		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			_goblinArcher->GetLookDirection() == 1 ? false : true
		);

		return true;
	}
	else if (_functionName == TEXT("GoblinArcher_Attack"))
	{
		// TODO: ȭ���� �ʱ� ��ġ�� ��� ��ó�� ��ġ�κ��� ��� ��ġ�κ��� �����ǰ� �Ǵµ�, �� ���� �ϵ��ڵ��Ǿ� �ֽ��ϴ�.
		// TODO: ȭ���� �ӵ� ���� �ϵ��ڵ��Ǿ� �ֽ��ϴ�.

		const auto _transform = _goblinArcher->GetGameObject()->GetComponent<Transform>();

		GameObject* _arrow = new GameObject(GetGameObject()->GetScene());
		_arrow->GetComponent<Transform>()->SetWorldPosition(
			_transform->GetWorldPosition() + Vector2::Up * 40
		);
		_arrow->AddComponent<Arrow>()->Set(
			{
				static_cast<float>(_goblinArcher->GetLookDirection()), 0
			},
			950, 5
		);

		return true;
	}
	else if (_functionName == TEXT("StartEnemyAttack"))
	{
		_goblinArcher->SetAttacking(true);

		return true;
	}
	else if (_functionName == TEXT("EndEnemyAttack"))
	{
		_goblinArcher->SetAttacking(false);
		_goblinArcher->SetLastEndAttackTimeMillis(GameTime::Instance.GetGameTimeMillis());

		return true;
	}

	if (UnitAnimationEventHandler::OnHandle(_functionName, _parameter) == true)
		return true;

	return false;
}
