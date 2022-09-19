#include "pch.h"
#include "GoblinBoss/GoblinBoss.h"

#include "AABBCollider.h"
#include "Animator.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Resources.h"
#include "Damagable.h"
#include "Health.h"
#include "GoblinBoss/GoblinBossAnimation.h"
#include "GoblinBoss/GoblinBossAnimationEventHandler.h"
#include "GoblinBoss/GoblinBossRigidBodyController.h"
#include "SpriteRenderer.h"
#include "DetectTriggerCollider.h"
#include "AttackTriggerCollider.h"
#include "Camera.h"
#include "ActionCommanderGameManager.h"

GoblinBoss::GoblinBoss(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
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
	GetGameObject()->AddComponent<AABBCollider>()->SetWidth(100);
	GetGameObject()->GetComponent<AABBCollider>()->SetHeight(140);

	GetGameObject()->GetComponent<AABBCollider>()->SetOffset({ 0, -140 / 2.f });

	// �±� ������ ���ݴϴ�.
	GetGameObject()->SetTag(EGameObjectTag::Enemy);

	GetGameObject()->AddComponent<RigidBody>();
	GetGameObject()->AddComponent<GoblinBossRigidBodyController>();
	GetGameObject()->AddComponent<Health>();
	GetGameObject()->GetComponent<Health>()->SetMaxHealth(200);
	GetGameObject()->AddComponent<Damagable>();
	GetGameObject()->AddComponent<SpriteRenderer>();
	GetGameObject()->AddComponent<Animator>();
	GetGameObject()->AddComponent<GoblinBossAnimation>();
	GetGameObject()->AddComponent<GoblinBossAnimationEventHandler>();

	GameObject* _detectTriggerCollider = new GameObject(GetGameObject()->GetScene());

	GetGameObject()->AddChild(_detectTriggerCollider);
	_detectTriggerCollider->GetComponent<Transform>()->SetLocalPosition({ 0, -60.f });
	_detectTriggerCollider->AddComponent<DetectTriggerCollider>()->SetColliderSize(200, 50);

	// ���ݿ� Ʈ���� ��ġ
	GameObject* _attackTriggerCollider = new GameObject(GetGameObject()->GetScene());

	GetGameObject()->AddChild(_attackTriggerCollider);
	//_attackTriggerCollider->GetComponent<Transform>()->SetLocalPosition({ 25.f * m_LookDirection, -35.f });
	_attackTriggerCollider->AddComponent<AttackTriggerCollider>()->SetColliderSize(100, 150);

	// HP��
	
	GameObject* _hpBarUnder = new GameObject(GetGameObject()->GetScene());
	GetGameObject()->AddChild(_hpBarUnder);
	_hpBarUnder->AddComponent<SpriteRenderer>()->SetSprite(
		Resources::Instance.GetSpriteByName(TEXT("Assets/UI/BossHPBarUnder.png")));
	_hpBarUnder->GetComponent<Transform>()->SetLocalPosition({ 0, -155.f });
	_hpBarUnder->GetComponent<SpriteRenderer>()->SetOpacity(0);

	GameObject* _hpBar = new GameObject(GetGameObject()->GetScene());
	_hpBarUnder->AddChild(_hpBar);
	_hpBar->AddComponent<SpriteRenderer>()->SetSprite(
		Resources::Instance.GetSpriteByName(TEXT("Assets/UI/BossHPBar.png")));
	_hpBar->GetComponent<Transform>()->SetLocalPosition({0, 10.f});
	_hpBar->GetComponent<SpriteRenderer>()->SetOpacity(0);
	_hpBar->GetComponent<SpriteRenderer>()->SetEnableFillMode(true);
	_hpBar->GetComponent<SpriteRenderer>()->SetFillAmount(1.0f);


	GetGameObject()->GetComponent<Health>()->SetOnHealthChanged([&](int _amount)
		{
			// ���� �Ƹ�?
			if(GetGameObject()->GetComponent<Animator>()->GetCurrentClip() != m_AttackAnimClip)
			{
				GetGameObject()->GetComponent<Animator>()->SetCurrentClip(m_HitAnimClip);
			}

			GetGameObject()->GetComponent<GoblinBossRigidBodyController>()->SetChaseSpeed(0);


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

void GoblinBoss::OnStart()
{
	ComponentBase::OnStart();

	// �ִϸ��̼� Ŭ���� �����ɴϴ�.
	m_IdleAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinBoss_Idle.json"));
	m_AttackAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinBoss_Attack.json"));

	m_DeadAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinBoss_Dead.json"));
	m_HitAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinBoss_Hit.json"));

	m_WalkAnimClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Enemy/GoblinBoss_Walk.json"));

	m_HitEffectClip = Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Effect/Effect_Hit.json"));


	// Ŭ���� ���Ǵ� ��������Ʈ ��Ʈ�� �ҷ��ɴϴ�.
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinBoss_Idle.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinBoss_Attack.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinBoss_Dead.json"));
	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Enemy/GoblinBoss_Walk.json"));

	Resources::Instance.LoadSpriteSheetFromJsonFile(TEXT("Assets/SpriteSheet/Effect/Effect_Hit1.json"));

	// ü�¹ٿ� ���õ� ��������Ʈ ��Ʈ�� �ҷ��ɴϴ�.
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/BossHPBarUnder.png"));
	Resources::Instance.LoadSpriteSheetFromFile(TEXT("Assets/UI/BossHPBar.png"));
}

void GoblinBoss::Tick()
{
	ComponentBase::Tick();
	const auto _controller = m_GameObject->GetComponent<GoblinBossRigidBodyController>();
	if (_controller == nullptr)
		return;

	const auto _goblinBossAnimation = m_GameObject->GetComponent<GoblinBossAnimation>();
	if (_goblinBossAnimation == nullptr)
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

	// �Ǻ� �������� _horizontal�� �޾������
		// Ư�� ������ ���� ������, �� ������ �ٶ󺸰�.
	int _horizontal = 0;

	// ������ �ƴ� ���¿��� �̵� ���� ����
		// �÷��̾���� �� ������ �Է�, ���� ���� ������
		// ������ ������ ���. Ư�� ������ ���� ������ => ����
	if (GetAttacking() == false)
	{
		// TODO �̵� ���� �ڵ�
			// ��Ʈ��

		if (_horizontal != 0)
			SetLookDirection(_horizontal);

		_controller->Horizontal(_horizontal);
	}

	_attackTriggerCollider->GetGameObject()->GetComponent<Transform>()->SetLocalPosition({ 40.f * m_LookDirection, -50.f });

	// ���� �ð� ������ HP�� �������
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

	// �⺻���� �߷� ���� ���� �� ����
	_controller->Move();
}

void GoblinBoss::TakeDamage(int _damage, const Vector2& _push)
{

}

std::vector<Damagable*> GoblinBoss::TryAttack(int _damage, const Collision::AABBBounds& _bounds,
	const Vector2& _push)
{
	// Rect Overlap���� ���� ã��
	std::vector<Damagable*> _outDamagables;

	std::vector<ColliderBase*> _colliders;
	CollisionManager::Instance.OverlapAABB(
		{ _bounds }, _colliders);

	// GetGameObject. GetComponent(ColliderBase) => ���� �ݶ��̴��� �߰��ϸ�.
	if (const auto _iter = std::ranges::find(_colliders, GetGameObject()->GetComponent<ColliderBase>());
		_iter != _colliders.end())
	{
		_colliders.erase(_iter);
	}

	// ��ü�� ���鼭 ������ �� �� �ִ� ��� �������� �ش�. => �÷��̾����׸� �������� �ش�.
		// ���� ���� �����ϸ� �ȵǱ� �����Դϴ�.
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

void GoblinBoss::SetLookDirection(int _lookDirection)
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