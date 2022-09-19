#include "pch.h"
#include "GoblinBoss/GoblinBossAnimationEventHandler.h"

#include "Animator.h"
#include "Core.h"
#include "Damagable.h"
#include "GameObject.h"
#include "GameTime.h"
#include "OneShotSpriteAnimationEffect.h"
#include "GoblinBoss/GoblinBoss.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "Sound.h"
#include "Screen.h"

GoblinBossAnimationEventHandler::GoblinBossAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName) :
	UnitAnimationEventHandler(_gameObject, _typeName)
{
}

bool GoblinBossAnimationEventHandler::OnHandle(const tstring& _functionName, const tstring& _parameter)
{
	GoblinBoss* _goblinBoss = GetGameObject()->GetComponent<GoblinBoss>();
	if (_goblinBoss == nullptr)
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
			std::stof(_splittedParams[0]) * _goblinBoss->GetLookDirection(),
			std::stof(_splittedParams[1])
		};

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		GetGameObject()->AddChild(_effect);
		_effect->GetComponent<Transform>()->SetLocalPosition(_offset);

		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			_goblinBoss->GetLookDirection() == 1 ? false : true
		);

		return true;
	}
	else if (_functionName == TEXT("GoblinBoss_Attack"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _goblinBoss->GetLookDirection(),
			_position.y + std::stof(_splittedParams[1])
		};

		const float _halfWidth = std::stof(_splittedParams[2]) / 2;
		const float _halfHeight = std::stof(_splittedParams[3]) / 2;

		const Collision::AABBBounds _bounds{
			-_halfWidth + _offset.x,
			-_halfHeight + _offset.y,
			_halfWidth + _offset.x,
			_halfHeight + _offset.y
		};

		// ���� ���ݷ�
		_goblinBoss->TryAttack(5, _bounds, Vector2::Zero);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("StartEnemyAttack"))
	{
		_goblinBoss->SetAttacking(true);

		return true;
	}
	else if (_functionName == TEXT("EndEnemyAttack"))
	{
		_goblinBoss->SetAttacking(false);
		_goblinBoss->SetLastEndAttackTimeMillis(GameTime::Instance.GetGameTimeMillis());

		return true;
	}

	if (UnitAnimationEventHandler::OnHandle(_functionName, _parameter) == true)
		return true;

	return false;
}
