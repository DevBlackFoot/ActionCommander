#include "pch.h"
#include "GoblinWarrior/GoblinWarrior.h"

#include "AABBCollider.h"
#include "Animator.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Resources.h"
#include "Damagable.h"
#include "Health.h"
#include "GoblinWarrior/GoblinWarriorAnimation.h"
#include "GoblinWarrior/GoblinWarriorAnimationEventHandler.h"
#include "GoblinWarrior/GoblinWarriorRigidBodyController.h"
#include "SpriteRenderer.h"
#include "DetectTriggerCollider.h"
#include "AttackTriggerCollider.h"
#include "Camera.h"
#include "ActionCommanderGameManager.h"

GoblinWarrior::GoblinWarrior(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_IdleAnimClip(nullptr),
	m_WalkAnimClip(nullptr),
	m_AttackAnimClip(nullptr),
	m_HitAnimClip(nullptr),
	m_DeadAnimClip(nullptr),
	m_LookDirection(0),
	m_bAttacking(false),
	m_LastEndAttackTimeMillis(0)
{
	GetGameObject()->AddComponent<AABBCollider>()->SetWidth(50);
	GetGameObject()->GetComponent<AABBCollider>()->SetHeight(80);

	GetGameObject()->GetComponent<AABBCollider>()->SetOffset({ 0, -80 / 2.f });

	// 태그 설정을 해줍니다.
	GetGameObject()->SetTag(EGameObjectTag::Enemy);

	GetGameObject()->AddComponent<RigidBody>();
	GetGameObject()->AddComponent<GoblinWarriorRigidBodyController>();
	GetGameObject()->AddComponent<Health>();
	GetGameObject()->GetComponent<Health>()->SetMaxHealth(80);
	GetGameObject()->AddComponent<Damagable>();
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
	GetGameObject()->AddComponent<GoblinWarriorAnimation>();
	GetGameObject()->AddComponent<GoblinWarriorAnimationEventHandler>();

	GameObject* _detectTriggerCollider = new GameObject(GetGameObject()->GetScene());

	GetGameObject()->AddChild(_detectTriggerCollider);
	_detectTriggerCollider->GetComponent<Transform>()->SetLocalPosition({ 0, -60.f });
	_detectTriggerCollider->AddComponent<DetectTriggerCollider>()->SetColliderSize(500, 50);

	// 공격용 트리거 설치
	GameObject* _attackTriggerCollider = new GameObject(GetGameObject()->GetScene());

	GetGameObject()->AddChild(_attackTriggerCollider);
	//_attackTriggerCollider->GetComponent<Transform>()->SetLocalPosition({ 25.f * m_LookDirection, -35.f });
	_attackTriggerCollider->AddComponent<AttackTriggerCollider>();

	// HP바
	GameObject* _hpBarUnder = new GameObject(GetGameObject()->GetScene());
	GetGameObject()->AddChild(_hpBarUnder);
	_hpBarUnder->AddComponent<SpriteRenderer>()->SetSprite(
		Resources::Instance.GetSpriteByName(TEXT("Assets/UI/EnemyHPBarUnder.png")));
	_hpBarUnder->GetComponent<Transform>()->SetLocalPosition({ 0, -95.f });
	_hpBarUnder->GetComponent<SpriteRenderer>()->SetOpacity(0);

	GameObject* _hpBar = new GameObject(GetGameObject()->GetScene());
	_hpBarUnder->AddChild(_hpBar);
	_hpBar->AddComponent<SpriteRenderer>()->SetSprite(
		Resources::Instance.GetSpriteByName(TEXT("Assets/UI/EnemyHPBar.png")));
	_hpBar->GetComponent<SpriteRenderer>()->SetOpacity(0);
	_hpBar->GetComponent<SpriteRenderer>()->SetEnableFillMode(true);
	_hpBar->GetComponent<SpriteRenderer>()->SetFillAmount(1.0f);

	GetGameObject()->GetComponent<Health>()->SetOnHealthChanged([&](int _amount)
		{
			GetGameObject()->GetComponent<Animator>()->SetCurrentClip(m_HitAnimClip);

			m_bAttacking = false;

			// 속력자체를 줄여야..
			GetGameObject()->GetComponent<GoblinWarriorRigidBodyController>()->SetChaseSpeed(0);

			const auto _hpBarUnder = GetGameObject()->GetChild(2)->GetComponent<SpriteRenderer>();
			const auto _hpBar = GetGameObject()->GetChild(2)->GetChild(0)->GetComponent<SpriteRenderer>();
			if (_hpBarUnder == nullptr || _hpBar == nullptr)
				return;

			float _nowHPPercentage = static_cast<float>(GetGameObject()->GetComponent<Health>()->GetHealth()) /
				static_cast<float>(GetGameObject()->GetComponent<Health>()->GetMaxHealth());
			_hpBar->SetFillAmount(_nowHPPercentage);
			_hpBarUnder->SetOpacity(1);
			_hpBar->SetOpacity(1);

			m_bShowHPBar = true;

			m_LastShowHPBarTimeMillis = GameTime::Instance.GetGameTimeMillis();
		});

	GetGameObject()->GetComponent<Health>()->SetOnHealthZero([&]()
		{
			const auto _animator = m_GameObject->GetComponent<Animator>();
			if (_animator == nullptr)
				return;

			_animator->SetCurrentClip(m_DeadAnimClip);

			if (const auto _gameManager = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject()->GetComponent<ActionCommanderGameManager>();
				_gameManager != nullptr)
			{
				_gameManager->MonsterDead();
			}
		});
}

void GoblinWarrior::OnStart()
{
	ComponentBase::OnStart();

	// 애니메이션 클립을 가져옵니다.
	m_IdleAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinWarrior_Idle.json"));
	m_AttackAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinWarrior_Attack.json"));

	m_DeadAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinWarrior_Dead.json"));
	m_HitAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinWarrior_Hit.json"));

	m_WalkAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinWarrior_Walk.json"));

	m_HitEffectClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Hit.json"));


	// 클립에 사용되는 스프라이트 시트를 불러옵니다.
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinWarrior_Idle.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinWarrior_Attack.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinWarrior_Dead.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinWarrior_Walk.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Hit1.json"));

	// 체력바와 관련된 스프라이트 시트를 불러옵니다.
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/EnemyHPBarUnder.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/EnemyHPBar.png"));
}

void GoblinWarrior::Tick()
{
	ComponentBase::Tick();

	const auto _controller = m_GameObject->GetComponent<GoblinWarriorRigidBodyController>();
	if (_controller == nullptr)
		return;

	const auto _goblinWarriorAnimation = m_GameObject->GetComponent<GoblinWarriorAnimation>();
	if (_goblinWarriorAnimation == nullptr)
		return;

	const auto _animator = m_GameObject->GetComponent<Animator>();
	if (_animator == nullptr)
		return;

	const auto _attackTriggerCollider = m_GameObject->GetChild(1)->GetComponent<AttackTriggerCollider>();
	if (_attackTriggerCollider == nullptr)
		return;

	if (_animator->GetCurrentClip() == m_DeadAnimClip
		&& _animator->GetReachedEnd() == true)
	{
		GetGameObject()->ReserveDestroy();
		return;
	}

	_attackTriggerCollider->GetGameObject()->GetComponent<Transform>()->SetLocalPosition({ 25.f * m_LookDirection, -35.f });

	// 일정 시간 지나면 HP바 사라지게
	if (m_bShowHPBar && GameTime::Instance.GetGameTimeMillis() - m_LastShowHPBarTimeMillis > 1000)
	{
		const auto _hpBarUnder = GetGameObject()->GetChild(2)->GetComponent<SpriteRenderer>();
		const auto _hpBar = GetGameObject()->GetChild(2)->GetChild(0)->GetComponent<SpriteRenderer>();
		if (_hpBarUnder == nullptr || _hpBar == nullptr)
			return;

		_hpBarUnder->SetOpacity(0);
		_hpBar->SetOpacity(0);

		m_bShowHPBar = false;
	}

	// 기본적인 중력 영향 받을 수 있음
	_controller->Move();
}

void GoblinWarrior::TakeDamage(int _damage, const Vector2& _push)
{
}

std::vector<Damagable*> GoblinWarrior::TryAttack(int _damage, const Collision::AABBBounds& _bounds,
	const Vector2& _push)
{
	// Rect Overlap으로 공격 찾기
	std::vector<Damagable*> _outDamagables;

	std::vector<ColliderBase*> _colliders;
	CollisionManager::Instance.OverlapAABB(
		{ _bounds }, _colliders);

	// GetGameObject. GetComponent(ColliderBase) => 나의 콜라이더를 발견하면.
	if (const auto _iter = std::ranges::find(_colliders, GetGameObject()->GetComponent<ColliderBase>());
		_iter != _colliders.end())
	{
		_colliders.erase(_iter);
	}

	// 전체를 돌면서 데미지 줄 수 있는 대상에 데미지를 준다. => 플레이어한테만 데미지를 준다.
		// 같은 적을 공격하면 안되기 때문입니다.
	for (auto _collider : _colliders)
	{
		if (const auto _damagable = _collider->GetGameObject()->GetComponent<Damagable>();
			_damagable != nullptr && _collider->GetGameObject()->GetTag() == EGameObjectTag::Player)
		{
			_damagable->Take(_damage, _push);

			_outDamagables.push_back(_damagable);
		}
	}

	return _outDamagables;
}

void GoblinWarrior::SetLookDirection(int _lookDirection)
{
	if (_lookDirection == m_LookDirection)
		return;

	m_LookDirection = _lookDirection;

	if (const auto _renderer = GetGameObject()->GetComponent<SpriteRenderer>();
		_renderer != nullptr)
	{
		_renderer->SetFlip(_lookDirection > 0 ? false : true);
	}
}