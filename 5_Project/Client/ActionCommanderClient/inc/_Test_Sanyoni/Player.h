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

class CommandQueue;
class Player :
	public SanyoniEngine::ComponentBase
{
public:
	Player(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("Player"));

	void OnStart() override;

	void Tick() override;

	void TakeDamage(int _damage, const Vector2& _push);

	std::vector<Damagable*> TryAttack(int _damage, const Collision::AABBBounds& _bounds, const Vector2& _push);

private:
	AnimationClip* m_IdleAnimClip;
	AnimationClip* m_RunAnimClip;
	AnimationClip* m_DashAnimClip;

	AnimationClip* m_JumpStartAnimClip;
	AnimationClip* m_AirborneAnimClip;

	AnimationClip* m_BasicAttack1AnimClip;
	AnimationClip* m_BasicAttack2AnimClip;
	AnimationClip* m_BasicAttack3AnimClip;

	AnimationClip* m_Skill1AnimClip;
	AnimationClip* m_Skill2AnimClip;
	AnimationClip* m_Skill3AnimClip;
	AnimationClip* m_Skill4AnimClip;

	AnimationClip* m_JumpEffectAnimClip;
	AnimationClip* m_DashEffectAnimClip;
	AnimationClip* m_ChargingEffectAnimClip;
	AnimationClip* m_Skill1_1EffectAnimClip;
	AnimationClip* m_Skill1_2EffectAnimClip;
	AnimationClip* m_Skill2_1EffectAnimClip;
	AnimationClip* m_Skill2_2EffectAnimClip;
	AnimationClip* m_Skill2_3EffectAnimClip;
	AnimationClip* m_Skill3EffectAnimClip;
	AnimationClip* m_Skill4_1EffectAnimClip;
	AnimationClip* m_Skill4_2EffectAnimClip;

	int m_LookDirection;

	bool m_bAttacking;

	int m_BasicAttackStack;

	bool m_bCanGoNextAttack;

	CommandQueue* m_CommandQueue;
	int m_NowCommand = -1;

	// 스킬의 쿨타임을 확인하기 위한 각종 변수입니다.
	bool m_bSkill1Able;
	bool m_bSkill2Able;
	bool m_bSkill3Able;
	bool m_bSkill4Able;

	long long m_LastEndSkill1TimeMillis;
	long long m_LastEndSkill2TimeMillis;
	long long m_LastEndSkill3TimeMillis;
	long long m_LastEndSkill4TimeMillis;

public:
	AnimationClip* GetIdleAnimClip() const { return m_IdleAnimClip; }
	AnimationClip* GetRunAnimClip() const { return m_RunAnimClip; }
	AnimationClip* GetDashAnimClip() const { return m_DashAnimClip; }
	AnimationClip* GetJumpStartAnimClip() const { return m_JumpStartAnimClip; }
	AnimationClip* GetAirborneAnimClip() const { return m_AirborneAnimClip; }
	AnimationClip* GetBasicAttack1AnimClip() const { return m_BasicAttack1AnimClip; }
	AnimationClip* GetBasicAttack2AnimClip() const { return m_BasicAttack2AnimClip; }
	AnimationClip* GetBasicAttack3AnimClip() const { return m_BasicAttack3AnimClip; }
	AnimationClip* GetSkill1AnimClip() const { return m_Skill1AnimClip; }
	AnimationClip* GetSkill2AnimClip() const { return m_Skill2AnimClip; }
	AnimationClip* GetSkill3AnimClip() const { return m_Skill3AnimClip; }
	AnimationClip* GetSkill4AnimClip() const { return m_Skill4AnimClip; }
	AnimationClip* GetJumpEffectAnimClip() const { return m_JumpEffectAnimClip; }
	AnimationClip* GetDashEffectAnimClip() const { return m_DashEffectAnimClip; }
	AnimationClip* GetChargingEffectAnimClip() const { return m_ChargingEffectAnimClip; }
	AnimationClip* GetSkill1_1EffectAnimClip() const { return m_Skill1_1EffectAnimClip; }
	AnimationClip* GetSkill1_2EffectAnimClip() const { return m_Skill1_2EffectAnimClip; }
	AnimationClip* GetSkill2_1EffectAnimClip() const { return m_Skill2_1EffectAnimClip; }
	AnimationClip* GetSkill2_2EffectAnimClip() const { return m_Skill2_2EffectAnimClip; }
	AnimationClip* GetSkill2_3EffectAnimClip() const { return m_Skill2_3EffectAnimClip; }

	int GetLookDirection() const { return m_LookDirection; }

	bool GetAttacking() const { return m_bAttacking; }

	int GetBasicAttackStack() const { return m_BasicAttackStack; }

	bool GetCanGoNextAttack() const { return m_bCanGoNextAttack; }

	void SetLookDirection(int _direction);

	void SetAttacking(bool _attacking) { m_bAttacking = _attacking; }

	void SetBasicAttackStack(int _value) { m_BasicAttackStack = _value; }

	void SetCanGoNextAttack(bool _can) { m_bCanGoNextAttack = _can; }

	void SetSkill1Able(bool _able) { m_bSkill1Able = _able; }

	void SetSkill2Able(bool _able) { m_bSkill2Able = _able; }

	void SetSkill3Able(bool _able) { m_bSkill3Able = _able; }

	void SetSkill4Able(bool _able) { m_bSkill4Able = _able; }

	bool GetSkill1Able() { return m_bSkill1Able; }

	bool GetSkill2Able() { return m_bSkill2Able; }

	bool GetSkill3Able() { return m_bSkill3Able; }

	bool GetSkill4Able() { return m_bSkill4Able; }

	long long GetLastEndSkill1TimeMills() const { return m_LastEndSkill1TimeMillis; }

	long long GetLastEndSkill2TimeMills() const { return m_LastEndSkill2TimeMillis; }

	long long GetLastEndSkill3TimeMills() const { return m_LastEndSkill3TimeMillis; }

	long long GetLastEndSkill4TimeMills() const { return m_LastEndSkill4TimeMillis; }

};
