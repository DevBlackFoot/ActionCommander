#include "pch.h"
#include "UnitAnimationEventHandler.h"

#include "Animator.h"
#include "GameObject.h"
#include "OneShotSpriteAnimationEffect.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "Sound.h"

using namespace SanyoniEngine;

UnitAnimationEventHandler::UnitAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName) :
	AnimationEventHandlerBase(_gameObject, _typeName)
{
}

bool UnitAnimationEventHandler::OnHandle(const tstring& _functionName, const tstring& _parameter)
{
	Animator* _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return false;

	RigidBodyController* _controller = GetGameObject()->GetComponent<RigidBodyController>();
	if (_controller == nullptr)
		return false;

	const auto _splittedParams = StringHelper::Split(_parameter, TEXT(" "));

	if (_functionName == TEXT("SayHello"))
	{
		DebugHelper::PrintDebugString(TEXT("Hello, %s"), _parameter);

		return true;
	}
	else if (_functionName == TEXT("AddForce"))
	{
		// TODO: unit이 바라보고 있는 방향에 의존되어 force를 주어야 합니다.

		const Vector2 _push{
			std::stof(_splittedParams[0]),
			std::stof(_splittedParams[1])
		};

		_controller->AddForce(_push);

		return true;
	}
	else if (_functionName == TEXT("Freeze"))
	{
		_controller->Stop();

		return true;
	}
	else if (_functionName == TEXT("Sound"))
	{
		std::string _str = StringHelper::ToString(_splittedParams[0]);

		Sound::Instance.PlayEffect(_str);

		return true;
	}
	else if (_functionName == TEXT("Effect_Stay"))
	{
		const Vector2 _offset{
			std::stof(_splittedParams[0]),
			std::stof(_splittedParams[1])
		};

		const Vector2 _worldPosition =
			GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + _offset;

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		_effect->GetComponent<Transform>()->SetWorldPosition(_worldPosition);

		// TODO: unit이 바라보고 있는 방향에 따라 flip을 설정해주어야 합니다.
		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			false
		);

		return true;
	}
	else if (_functionName == TEXT("Effect_Follow"))
	{
		// TODO: unit이 바라보고 있는 방향에 의존된 위치에 생성해야 합니다.

		const Vector2 _offset{
			std::stof(_splittedParams[0]),
			std::stof(_splittedParams[1])
		};

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		GetGameObject()->AddChild(_effect);
		_effect->GetComponent<Transform>()->SetLocalPosition(_offset);

		// TODO: unit이 바라보고 있는 방향에 따라 flip을 설정해주어야 합니다.
		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			false
		);

		return true;
	}

	return false;
}
