#include "pch.h"
#include "Player.h"

#include "AABBCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "CommandQueue.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Input.h"
#include "PlayerRigidBodyController.h"
#include "Resources.h"
#include "Damagable.h"
#include "GameSceneFactory.h"
#include "Health.h"
#include "PlayerAnimation.h"
#include "PlayerAnimationEventHandler.h"
#include "SpriteRenderer.h"
#include "StateMachine.h"
#include "ActionCommanderGameManager.h"
#include "SpriteFlicker.h"

Player::Player(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_IdleAnimClip(nullptr),
	m_RunAnimClip(nullptr),
	m_JumpStartAnimClip(nullptr),
	m_AirborneAnimClip(nullptr),
	m_BasicAttack1AnimClip(nullptr),
	m_BasicAttack2AnimClip(nullptr),
	m_BasicAttack3AnimClip(nullptr),
	m_LookDirection(0),
	m_bAttacking(false),
	m_BasicAttackStack(0),
	m_bCanGoNextAttack(false),
	m_bSkill1Able(true),
	m_bSkill2Able(true),
	m_bSkill3Able(true),
	m_bSkill4Able(true),
	m_LastEndSkill1TimeMillis(0),
	m_LastEndSkill2TimeMillis(0),
	m_LastEndSkill3TimeMillis(0),
	m_LastEndSkill4TimeMillis(0)
{
	GetGameObject()->AddComponent<AABBCollider>()->SetWidth(50);
	GetGameObject()->GetComponent<AABBCollider>()->SetHeight(64);

	// height의 절반을 offset으로 해야, collider의 밑이 오브젝트의 발 밑에 정렬됩니다.
	GetGameObject()->GetComponent<AABBCollider>()->SetOffset({ 0, -64 / 2.f });

	GetGameObject()->AddComponent<RigidBody>();
	GetGameObject()->AddComponent<PlayerRigidBodyController>();
	GetGameObject()->AddComponent<Health>()->SetMaxHealth(130);
	GetGameObject()->AddComponent<Damagable>();
	GetGameObject()->AddComponent<StateMachine<Player>>();
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
	GetGameObject()->AddComponent<PlayerAnimationEventHandler>();
	GetGameObject()->AddComponent<PlayerAnimation>();
	GetGameObject()->AddComponent<SpriteFlicker>()->SetEnable(false);

	const auto _spriteFilcker = GetGameObject()->GetComponent<SpriteFlicker>();

	// 커맨드 큐 생성
	// 디큐 범위 1초
	m_CommandQueue = new CommandQueue(500);

	// 헬스 변화 파악
	GetGameObject()->GetComponent<Health>()->SetOnHealthChanged([&](int _amount)
		{
			//GetGameObject()->GetComponent<SpriteFlicker>()->SetDurationMillis(500);
			GetGameObject()->GetComponent<SpriteFlicker>()->Start(700);

			// 맞았으면 현재 하고 있는 행동을 취소 => 현재 상태 초기화.
			m_bAttacking = false;
		});

	GetGameObject()->GetComponent<Health>()->SetOnHealthZero([]()
		{
			const auto _gameManager = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ActionCommanderGameManager>();
			if (_gameManager != nullptr)
			{
				_gameManager->GoGameOverLevel();
			}

		});
}

void Player::OnStart()
{
	ComponentBase::OnStart();

	m_IdleAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Idle.json"));
	m_RunAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Run.json"));
	m_DashAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Dash.json"));

	m_JumpStartAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_JumpStart.json"));
	m_AirborneAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Airborne.json"));

	m_BasicAttack1AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_BasicAttack1.json"));
	m_BasicAttack2AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_BasicAttack2.json"));
	m_BasicAttack3AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_BasicAttack3.json"));

	m_Skill1AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Skill1.json"));
	m_Skill2AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Skill2.json"));
	m_Skill3AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Skill3.json"));
	m_Skill4AnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_Skill4.json"));

	m_JumpEffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Jump.json"));
	m_JumpEffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Dash.json"));
	m_ChargingEffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Charging.json"));
	m_Skill1_1EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill1_1.json"));
	m_Skill1_2EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill1_2.json"));
	m_Skill2_1EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill2_1.json"));
	m_Skill2_2EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill2_2.json"));
	m_Skill2_3EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill2_3.json"));
	m_Skill3EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill3.json"));
	m_Skill3EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill4_1.json"));
	m_Skill3EffectAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Skill4_2.json"));

	// 애니메이션 클립에 사용되는 스프라이트의 시트를 불러옵니다.
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Idle.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Run.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Dash.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Jump.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_BasicAttack1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_BasicAttack2.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_BasicAttack3.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Skill1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Skill2.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Skill3.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Player/Player_Skill4.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Jump.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Dash.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Charging.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill1_2.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill2_1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill2_2.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill2_3.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill3.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill4_1.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Skill4_2.json"));
}

void Player::Tick()
{
	ComponentBase::Tick();

	// 커맨드 큐는 여기서만 불리게.
	m_CommandQueue->Update();

	const auto _controller = m_GameObject->GetComponent<PlayerRigidBodyController>();
	if (_controller == nullptr)
		return;

	const auto _playerAnim = m_GameObject->GetComponent<PlayerAnimation>();
	if (_playerAnim == nullptr)
		return;

	const auto _animator = m_GameObject->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const int _horizontal = Input::Instance.GetAxisHorizontal();

	if (GetAttacking() == false)
	{
		// 공격중이지 않은 상태에서만 이동할 수 있습니다.

		// 이동 상태일 때의 horizontal을 바라보는 방향으로 설정합니다.
		// note: 아무 키도 누르지 않을 때의 입력은 무시해야 합니다.
		if (_horizontal != 0)
			SetLookDirection(_horizontal);


		_controller->Horizontal(_horizontal);

		if (Input::Instance.GetKeyState(EVirtualKey::LeftControl) & EKeyStateBit::Bit_Down)
		{
			// 대시를 실시합니다.
			_controller->DoDash();
		}

		if (Input::Instance.GetKeyPress(EVirtualKey::DownArrow)
			&& Input::Instance.GetKeyState(EVirtualKey::Key_C) & EKeyStateBit::Bit_Down)
		{
			// 하단 점프를 실시합니다.
			_controller->DoDownJump();
		}
		else if (Input::Instance.GetKeyState(EVirtualKey::Key_C) & EKeyStateBit::Bit_Down)
		{
			// 점프를 실시합니다.
			_controller->DoJump();
		}
	}

	if (Input::Instance.GetKeyPress(EVirtualKey::Key_X))
	{
		// 기본 공격입니다.
		_playerAnim->DoBasicAttack();
	}

	if (m_CommandQueue->GetSuccessCommand() != -1)
	{
		m_NowCommand = m_CommandQueue->GetSuccessCommand();
	}

	// TODO: 테스트 코드입니다.
	// 스킬이 쿨타임 중이면 nowCommand 무효화
	if (
		((m_NowCommand == 0 || m_NowCommand == 1) && !m_bSkill1Able)
		||
		((m_NowCommand == 2 || m_NowCommand == 3) && !m_bSkill2Able)
		||
		((m_NowCommand == 4 || m_NowCommand == 5) && !m_bSkill3Able)
		||
		(m_NowCommand == 6 && !m_bSkill4Able)
		)
	{
		m_NowCommand = -1;
	}

	if (
		(m_NowCommand == 0 && m_LookDirection == -1 && m_bSkill1Able)
		||
		(m_NowCommand == 1 && m_LookDirection == 1 && m_bSkill1Able)
		)
	{
		_playerAnim->DoSkill1();
		m_NowCommand = -1;
		m_bSkill1Able = false;
		m_LastEndSkill1TimeMillis = GameTime::Instance.GetGameTimeMillis();
	}

	if (
		(m_NowCommand == 2 && m_LookDirection == -1 && m_bSkill2Able)
		||
		(m_NowCommand == 3 && m_LookDirection == 1 && m_bSkill2Able)
		)
	{
		_playerAnim->DoSkill2();
		m_NowCommand = -1;
		m_bSkill2Able = false;
		m_LastEndSkill2TimeMillis = GameTime::Instance.GetGameTimeMillis();
	}

	if (
		(m_NowCommand == 4 && m_LookDirection == -1 && m_bSkill3Able)
		||
		(m_NowCommand == 5 && m_LookDirection == 1 && m_bSkill3Able)
		)
	{
		_playerAnim->DoSkill3();
		m_NowCommand = -1;
		m_bSkill3Able = false;
		m_LastEndSkill3TimeMillis = GameTime::Instance.GetGameTimeMillis();
	}

	if (
		(m_NowCommand == 6 && _controller->GetInAirLastFrame() && m_bSkill4Able)
		)
	{
		_playerAnim->DoSkill4();
		m_NowCommand = -1;
		m_bSkill4Able = false;
		m_LastEndSkill4TimeMillis = GameTime::Instance.GetGameTimeMillis();
	}

	// 커맨드를 잘 입력하지 못하는 사용자들을 위한
	// 편의성 쇼트컷 입력 키를 제공합니다.
	if (Input::Instance.GetKeyState(EVirtualKey::Key_1) & EKeyStateBit::Bit_Down)
	{
		_playerAnim->DoSkill1();
	}
	else if (Input::Instance.GetKeyState(EVirtualKey::Key_2) & EKeyStateBit::Bit_Down)
	{
		_playerAnim->DoSkill2();
	}
	else if (Input::Instance.GetKeyState(EVirtualKey::Key_3) & EKeyStateBit::Bit_Down)
	{
		_playerAnim->DoSkill3();
	}
	else if ((Input::Instance.GetKeyState(EVirtualKey::Key_4) & EKeyStateBit::Bit_Down) && _controller->GetInAirLastFrame())
	{
		_playerAnim->DoSkill4();
	}

	_controller->Move();
}

std::vector<Damagable*> Player::TryAttack(int _damage, const Collision::AABBBounds& _bounds, const Vector2& _push)
{
	std::vector<Damagable*> _outDamagables;

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
			_damagable->Take(_damage, _push);

			_outDamagables.push_back(_damagable);
		}
	}

	return _outDamagables;
}

void Player::SetLookDirection(int _direction)
{
	if (_direction == m_LookDirection)
		return;

	m_LookDirection = _direction;

	if (const auto _renderer = GetGameObject()->GetComponent<SpriteRenderer>();
		_renderer != nullptr)
	{
		// 스프라이트의 flip을 설정합니다.
		_renderer->SetFlip(_direction > 0 ? false : true);
	}
}
