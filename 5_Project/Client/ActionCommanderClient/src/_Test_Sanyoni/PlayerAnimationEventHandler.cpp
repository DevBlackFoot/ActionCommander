#include "pch.h"
#include "PlayerAnimationEventHandler.h"

#include "Animator.h"
#include "Camera.h"
#include "Core.h"
#include "Damagable.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Input.h"
#include "OneShotSpriteAnimationEffect.h"
#include "Player.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "Sound.h"
#include "Screen.h"
#include "ShakeCamera.h"

using namespace SanyoniEngine;

PlayerAnimationEventHandler::PlayerAnimationEventHandler(GameObject* _gameObject, const tstring& _typeName) :
	UnitAnimationEventHandler(_gameObject, _typeName)
{
}

bool PlayerAnimationEventHandler::OnHandle(const tstring& _functionName, const tstring& _parameter)
{
	Player* _player = GetGameObject()->GetComponent<Player>();
	if (_player == nullptr)
		return false;

	Animator* _animator = GetGameObject()->GetComponent<Animator>();
	if (_animator == nullptr)
		return false;

	RigidBodyController* _controller = GetGameObject()->GetComponent<RigidBodyController>();
	if (_controller == nullptr)
		return false;

	const auto _position = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

	const auto _splittedParams = StringHelper::Split(_parameter, TEXT(" "));

	if (_functionName == TEXT("AddForce"))
	{
		if (_animator->GetCurrentClip() == _player->GetBasicAttack1AnimClip()
			|| _animator->GetCurrentClip() == _player->GetBasicAttack2AnimClip()
			|| _animator->GetCurrentClip() == _player->GetBasicAttack3AnimClip())
		{
			// 기본 공격중일 때에는
			// 공격하는 방향과 일치하는 키를 누른 상태에서만 AddForce를 적용합니다.

			const int _horizontal = Input::Instance.GetAxisHorizontal();
			if (_player->GetLookDirection() != _horizontal)
				return false;
		}


		const Vector2 _push{
			std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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
	else if (_functionName == TEXT("CanGoNextAttack"))
	{
		_player->SetCanGoNextAttack(true);

		return true;
	}
	else if (_functionName == TEXT("StartBasicAttack"))
	{
		_player->SetAttacking(true);

		return true;
	}
	else if (_functionName == TEXT("EndBasicAttack"))
	{
		_player->SetCanGoNextAttack(false);
		_player->SetBasicAttackStack(0);
		_player->SetAttacking(false);

		return true;
	}
	else if (_functionName == TEXT("BasicAttack_1")
		|| _functionName == TEXT("BasicAttack_2"))
	{
		_player->SetBasicAttackStack(_player->GetBasicAttackStack() + 1);


		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 플레이어 공격력
		_player->TryAttack(20, _bounds, Vector2(_player->GetLookDirection(), 0) * 0.01f);


		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}


		return true;
	}
	else if (_functionName == TEXT("BasicAttack_3"))
	{
		_player->SetBasicAttackStack(_player->GetBasicAttackStack() + 1);


		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 플레이어 공격력
		_player->TryAttack(20, _bounds, Vector2(static_cast<float>(_player->GetLookDirection()), 0.f) * 1.f);


		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}


		return true;
	}
	else if (_functionName == TEXT("StartSkillAttack"))
	{
		_player->SetAttacking(true);

		return true;
	}
	else if (_functionName == TEXT("EndSkillAttack"))
	{
		_player->SetAttacking(false);

		return true;
	}
	else if (_functionName == TEXT("Skill1_Attack1"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		_player->TryAttack(20, _bounds, Vector2::Right * _player->GetLookDirection() * 10);


		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}


		return true;
	}
	else if (_functionName == TEXT("Skill1_Attack2"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		_player->TryAttack(
			30,
			_bounds,
			Vector2{ static_cast<float>(_player->GetLookDirection()), -1.f } *10
		);


		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}


		return true;
	}
	// Skill2
	else if (_functionName == TEXT("Skill2_Attack1_Left"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 세 번째 인자 push 조정 필요
		_player->TryAttack(
			20,
			_bounds,
			Vector2(_player->GetLookDirection(), 0) * -5.f
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill2_Attack1_Right"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 세 번째 인자 push 조정 필요
		_player->TryAttack(
			20,
			_bounds,
			Vector2(_player->GetLookDirection(), 0) * 5.f
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill2_Attack2_Left"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 세 번째 인자 push 조정 필요
		_player->TryAttack(
			20,
			_bounds,
			Vector2(_player->GetLookDirection(), 0) * -5.f
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill2_Attack2_Right"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 세 번째 인자 push 조정 필요
		_player->TryAttack(
			20,
			_bounds,
			Vector2(_player->GetLookDirection(), 0) * 5.f
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill2_Attack3"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		_player->SetLookDirection(_player->GetLookDirection() == 1 ? -1 : 1);

		// 세 번째 인자 push 조정 필요 => 휘두르는 공격. 주인공의 방향쪽으로 밀리게
		_player->TryAttack(
			40,
			_bounds,
			Vector2(_player->GetLookDirection(), 0) * 10.f
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill2_Attack4"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		// 세 번째 인자 push 조정 필요 => 돌.
		_player->TryAttack(
			30,
			_bounds,
			Vector2{ static_cast<float>(_player->GetLookDirection()), -1.f } *10
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	// Skill3
	else if (_functionName == TEXT("Skill3_Attack1"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		_player->TryAttack(
			30,
			_bounds,
			Vector2{ static_cast<float>(_player->GetLookDirection()), -1.5f } *10
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}
	else if (_functionName == TEXT("Skill4_Attack1"))
	{
		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		_player->TryAttack(
			40,
			_bounds,
			Vector2{ static_cast<float>(_player->GetLookDirection()), -1.5f } *10
		);

		if (Core::GetEnableDebug())
		{
			// debug draw
			Screen::Instance.DrawRectangleWorldSpace(Color::White, { _bounds.GetLeft(), _bounds.GetTop(), _bounds.GetRight(), _bounds.GetBottom() });
		}

		return true;
	}

	else if (_functionName == TEXT("CameraShake_Rect"))
	{
		// TODO: Player::TryAttack과 상당히 많은 부분이 중복되어 있습니다.
		// 또한 지금의 Rect 검사는 Attack에 이어서 다시 한 번 Damagable 충돌체를 감지하는 것이기 때문에 비효율적입니다.
		// 이러한 문제점은 Attack 이벤트 매개변수에 옵션으로서 제공되는 것으로 개선할 수 있을 것이라 생각됩니다.


		const Vector2 _offset{
			_position.x + std::stof(_splittedParams[0]) * _player->GetLookDirection(),
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

		std::vector<ColliderBase*> _colliders;
		CollisionManager::Instance.OverlapAABB(
			{ _bounds },
			_colliders
		);

		if (const auto _iter = std::ranges::find(_colliders, GetGameObject()->GetComponent<ColliderBase>());
			_iter != _colliders.end())
		{
			// 공격 범위 내에서 만약 자기 자신이 검출되었다면,
			// 이를 무시하기 위해 리스트에서 제외합니다.

			_colliders.erase(_iter);
		}

		for (auto _collider : _colliders)
		{
			if (const auto _damagable = _collider->GetGameObject()->GetComponent<Damagable>();
				_damagable != nullptr)
			{
				SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ShakeCamera>()->StartShake(
					std::stof(_splittedParams[4]),
					std::stof(_splittedParams[5]),
					std::stof(_splittedParams[6]) * GameTime::SECOND_TO_MILLIS
				);
				break;
			}
		}
		return true;
	}
	else if (_functionName == TEXT("CameraShake"))
	{

		SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ShakeCamera>()->StartShake(
			std::stof(_splittedParams[0]),
			std::stof(_splittedParams[1]),
			std::stof(_splittedParams[2]) * GameTime::SECOND_TO_MILLIS
		);
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
			std::stof(_splittedParams[0]) * _player->GetLookDirection(),
			std::stof(_splittedParams[1])
		};

		const Vector2 _worldPosition =
			_player->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + _offset;

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		_effect->GetComponent<Transform>()->SetWorldPosition(_worldPosition);

		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			_player->GetLookDirection() == 1 ? false : true
		);

		return true;
	}
	else if (_functionName == TEXT("Effect_Follow"))
	{
		const Vector2 _offset{
			std::stof(_splittedParams[0]) * _player->GetLookDirection(),
			std::stof(_splittedParams[1])
		};

		GameObject* _effect = new GameObject(GetGameObject()->GetScene());

		GetGameObject()->AddChild(_effect);
		_effect->GetComponent<Transform>()->SetLocalPosition(_offset);

		_effect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
			Resources::Instance.LoadAnimationClipFromJsonFile(_splittedParams[2]),
			_player->GetLookDirection() == 1 ? false : true
		);

		return true;
	}


	if (UnitAnimationEventHandler::OnHandle(_functionName, _parameter) == true)
		return true;


	return false;
}
