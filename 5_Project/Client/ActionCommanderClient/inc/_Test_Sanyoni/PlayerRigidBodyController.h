#pragma once

#include "RigidBodyController.h"

namespace SanyoniEngine
{
	class ColliderBase;
}

class PlayerRigidBodyController :
	public RigidBodyController
{
public:
	PlayerRigidBodyController(GameObject* _gameObject, tstring _typeName = TEXT("PlayerRigidBodyController"));

	void OnStart() override;

	void PreTick() override;

	void OnPreMove(Vector2& _refDeltaVelocity) override;

	void OnPostMove(Vector2& _refDeltaVelocity) override;

	bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

	bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

private:
	void Jump();

	void DownJump(const RaycastHitInfo& _hitInfo);

	void Dash();

private:
	/*
	 * Properties
	 */

	float m_MoveSpeed;

	float m_DashForce;

	int m_MaxJumpInAir;

	float m_JumpForce;

	long long m_DashDurationMillis;

	long long m_DashCooldownTimeMillis;

	/*
	 * Movement
	 */

	int m_Horizontal;

	float m_DesiredDeltaVelocityX;

	/*
	 * Jump
	 */

	int m_JumpInAirCount;

	bool m_bDoJump;

	bool m_bDoDownJump;

	bool m_bDidJumpThisFrame;

	ColliderBase* m_JumpDownTarget;

	bool m_bInAirLastFrame;

	/*
	 * Dash
	 */

	bool m_bDoDash;

	bool m_bStartDashThisFrame;

	bool m_bDash;

	long long m_LastDashStartTimeMillis;

	int m_DashDirection;

	bool m_bAlreadyUseDashInAir;

public:
	void Horizontal(int _horizontal);

	void DoJump() { m_bDoJump = true; }

	void DoDownJump() { m_bDoDownJump = true; }

	void DoDash();

	bool GetDidJumpThisFrame() const { return m_bDidJumpThisFrame; }

	bool GetStartDashThisFrame() const { return m_bStartDashThisFrame; }

	bool GetInAirLastFrame() const { return m_bInAirLastFrame; }
};
