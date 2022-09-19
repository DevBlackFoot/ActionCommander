#pragma once

#include "RigidBodyController.h"

namespace SanyoniEngine
{
	class ColliderBase;
}

class GoblinBossRigidBodyController :
	public RigidBodyController
{
public:
	GoblinBossRigidBodyController(GameObject* _gameObject, tstring _typeName = TEXT("GoblinBossRigidBodyController"));

	void OnStart() override;

	void PreTick() override;

	void OnPreMove(Vector2& _refDeltaVelocity) override;

	void OnPostMove(Vector2& _refDeltaVelocity) override;

	bool OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

	bool OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity) override;

private:
	void Chase();

private:
	// Properties
	float m_WalkSpeed;		// �⺻ �̵� �ӵ�

	float m_ChaseSpeed;		// �߰� �̵� �ӵ�

	// Movement
	int m_Horizontal;
	float m_DesiredDeltaVelocityX;

	// Chase
	bool m_bDoChase;

	bool m_bStartChaseThisFrame;

	bool m_bChase;

	long long m_lastChaseStartTimeMillis;

	int m_ChaseDirection;

public:
	void Horizontal(int _horizontal);

	void DoChase();

	bool GetStartChaseThisFrame() const { return m_bStartChaseThisFrame; }

	void SetChase(bool _bChase) { m_bChase = _bChase; };

	void SetDesiredDeltaVelocityX(float _velocityX) { m_DesiredDeltaVelocityX = _velocityX; }

	void SetChaseSpeed(float _chaseSpeed) { m_ChaseSpeed = _chaseSpeed; }
};

