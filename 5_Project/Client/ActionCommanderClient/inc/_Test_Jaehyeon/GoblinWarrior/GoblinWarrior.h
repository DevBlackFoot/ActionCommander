#pragma once

#include "CollisionBounds.h"
#include "SanyoniEngine_types.h"
#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;
	class AnimationClip;
	class Damagable;
}

class GoblinWarrior :
	public SanyoniEngine::ComponentBase
{
public:
	GoblinWarrior(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("GoblinWarrior"));

	void OnStart() override;

	void Tick() override;

	void TakeDamage(int _damage, const Vector2& _push);

	std::vector<Damagable*> TryAttack(int _damage, const Collision::AABBBounds& _bounds, const Vector2& _push);

private:
	AnimationClip* m_IdleAnimClip;
	AnimationClip* m_WalkAnimClip;

	AnimationClip* m_AttackAnimClip;

	AnimationClip* m_HitAnimClip;
	AnimationClip* m_DeadAnimClip;

	AnimationClip* m_HitEffectClip;

	int m_LookDirection;

	bool m_bAttacking;

	// HP�� ǥ�ø� ���� ���� �ð� �������Դϴ�.
	bool m_bShowHPBar;

	long long m_LastShowHPBarTimeMillis;

	// ���� ���� �����ϱ� ���� �ð� ����
	long long m_LastEndAttackTimeMillis;

public:
	AnimationClip* GetIdleAnimClip() const { return m_IdleAnimClip; }
	AnimationClip* GetWalkAnimClip() const { return m_WalkAnimClip; }
	AnimationClip* GetAttackAnimClip() const { return m_AttackAnimClip; }
	AnimationClip* GetHitAnimClip() const { return m_HitAnimClip; }
	AnimationClip* GetDeadAnimClip() const { return m_DeadAnimClip; }

	int GetLookDirection() const { return m_LookDirection; }

	bool GetAttacking() const { return m_bAttacking; }

	void SetLookDirection(int _lookDirection);

	void SetAttacking(bool _attacking) { m_bAttacking = _attacking; }

	long long GetLastEndAttackTimeMillis() const { return m_LastEndAttackTimeMillis; }

	void SetLastEndAttackTimeMillis(long long _lastEndAttackTimeMillis) { m_LastEndAttackTimeMillis = _lastEndAttackTimeMillis; }

};

