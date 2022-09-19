#include "pch.h"
#include "Nepen/NepenAnimationEventHandler.h"

#include "Animator.h"
#include "Core.h"
#include "Damagable.h"
#include "GameObject.h"
#include "GameTime.h"
#include "OneShotSpriteAnimationEffect.h"
#include "Nepen/Nepen.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "Sound.h"
#include "Screen.h"

NepenAnimationEventHandler::NepenAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName) :
	UnitAnimationEventHandler(_gameObject, _typeName)
{
}

bool NepenAnimationEventHandler::OnHandle(const tstring& _functionName, const tstring& _parameter)
{
	Nepen* _nepen = GetGameObject()->GetComponent<Nepen>();
	if (_nepen == nullptr)
		return false;

	Animator* _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return false;

	RigidBodyController* _controller = GetGameObject()->GetComponent<RigidBodyController>();
	if (_controller == nullptr)
		return false;

	const auto _position = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

	const auto _splittedParams = StringHelper::Split(_parameter, TEXT(" "));

	// 아래부터 json파일로 전달받는 이벤트에 대한 처리를 합니다.
		// if로 찾아서 처리후 return true
	if (_functionName == TEXT("Effect_Follow"))
	{
		const Vector2 _offset{
			std::stof(_splittedParams[0]) * _nepen->GetLookDirection(),
			std::stof(_splittedParams[1])
		};

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		GetGameObject()->AddChild(_effect);
		_effect->GetComponent<Transform>()->SetLocalPosition(_offset);

		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			_nepen->GetLookDirection() == 1 ? false : true
		);

		return true;
	}
	else if (_functionName == TEXT("NepenAttack"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _nepen->GetLookDirection(),
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

		// 네펜데스 공격력
		_nepen->TryAttack(5, _bounds, Vector2::Zero);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("StartEnemyAttack"))
	{
		_nepen->SetAttacking(true);

		return true;
	}
	else if (_functionName == TEXT("EndEnemyAttack"))
	{
		_nepen->SetAttacking(false);
		_nepen->SetLastEndAttackTimeMillis(GameTime::Instance.GetGameTimeMillis());
		return true;
	}

	if (UnitAnimationEventHandler::OnHandle(_functionName, _parameter) == true)
		return true;

	return false;
}
