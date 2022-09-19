#include "pch.h"
#include "PlayerRigidBodyController.h"

#include "ColliderBase.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Input.h"
#include "Platform.h"

PlayerRigidBodyController::PlayerRigidBodyController(GameObject* _gameObject, tstring _typeName) :
	RigidBodyController(_gameObject, _typeName),
	m_MoveSpeed(600.f),
	m_DashForce(1200.f),
	m_MaxJumpInAir(1),
	m_JumpForce(-15.f),
	m_DashDurationMillis(300),
	m_DashCooldownTimeMillis(500),
	m_Horizontal(0),
	m_DesiredDeltaVelocityX(0),
	m_JumpInAirCount(0),
	m_bDoJump(false),
	m_bDoDownJump(false),
	m_bDidJumpThisFrame(false),
	m_JumpDownTarget(nullptr),
	m_bInAirLastFrame(false),
	m_bDoDash(false),
	m_bStartDashThisFrame(false),
	m_bDash(false),
	m_LastDashStartTimeMillis(0),
	m_DashDirection(0),
	m_bAlreadyUseDashInAir(false)
{
}

void PlayerRigidBodyController::OnStart()
{
	if (const auto _rigidBody = GetGameObject()->GetComponent<RigidBody>();
		_rigidBody != nullptr)
	{
		_rigidBody->SetExcludeCollsion({{EGameObjectTag::Player, EGameObjectTag::Enemy}});
	}
}

void PlayerRigidBodyController::PreTick()
{
	RigidBodyController::PreTick();

	m_bDoJump = false;
	m_bDoDownJump = false;
	m_bDidJumpThisFrame = false;

	m_bStartDashThisFrame = false;

	m_Horizontal = 0;
	m_DesiredDeltaVelocityX = 0;
}

void PlayerRigidBodyController::OnPreMove(Vector2& _refDeltaVelocity)
{
	Jump();

	if (_refDeltaVelocity.y < 0)
	{
		// 위로 이동하는 경우
		// 하단 점프를 중단합니다.

		m_JumpDownTarget = nullptr;
	}

	Dash();

	if (m_bDash == true
		&& m_bDidJumpThisFrame == true)
	{
		// 대쉬 도중 점프를 하면 대쉬를 끊습니다.
		m_bDash = false;
	}

	if (m_bDash == true)
	{
		m_DesiredDeltaVelocityX = m_DashDirection * m_DashForce * GameTime::Instance.GetDeltaTime();
		_refDeltaVelocity.y = 0;

		// 대시의 지속시간동안 대시 방향으로 전진을 계속합니다.
		const long long _dashElapsedTimeMillis = GameTime::Instance.GetGameTimeMillis() - m_LastDashStartTimeMillis;

		if (_dashElapsedTimeMillis > m_DashDurationMillis)
			m_bDash = false;
	}
	else
	{
		m_DesiredDeltaVelocityX = m_Horizontal * m_MoveSpeed * GameTime::Instance.GetDeltaTime();
	}

	_refDeltaVelocity.x = m_DesiredDeltaVelocityX;
}

void PlayerRigidBodyController::OnPostMove(Vector2& _refDeltaVelocity)
{
	if (_refDeltaVelocity.y < 0)
	{
		// 공중에 떠있다는 것을 기록합니다.
		// 이 값은 플랫폼에 발을 디딜 때까지 계속 true를 유지하며 바뀌지 않습니다.
		m_bInAirLastFrame = true;
	}
}

bool PlayerRigidBodyController::OnVerticalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	if (_hitInfo.direction.y > 0)
	{
		DownJump(_hitInfo);

		if (_hitInfo.collider == m_JumpDownTarget)
		{
			// 하단 점프중에 같은 플랫폼과 충돌했다면,
			// 이를 무시합니다.

			return true;
		}

		if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
			_tag == EGameObjectTag::Enemy)
		{
			return false;
		}

		// 하단 점프를 중단합니다.
		m_JumpDownTarget = nullptr;

		// 공중 점프 카운트를 초기화합니다.
		m_JumpInAirCount = 0;

		// 공중에 떠있는지에 대한 여부를 초기화합니다.
		m_bInAirLastFrame = false;
		m_bAlreadyUseDashInAir = false;
	}
	else if (_hitInfo.direction.y < 0)
	{
		if (const auto _platform = _hitInfo.collider->GetGameObject()->GetComponent<Platform>();
			_platform != nullptr)
		{
			if (_platform->GetAllowJumpThrough() == false)
			{
				// 점프로 뚫고 올라갈 수 있는 플랫폼이 아니라면,
				// 속도를 강제로 제한하여 떨어뜨립니다.
				_refDeltaVelocity.y = _hitInfo.direction.y * _hitInfo.distance;
				return true;
			}
		}

		if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
			_tag == EGameObjectTag::Enemy)
		{
			return false;
		}
	}

	// 특별한 핸들링이 없는 경우, 기본 타입에서 정의된 동작을 실행합니다.
	return RigidBodyController::OnVerticalCollision(_hitInfo, _refDeltaVelocity);
}

bool PlayerRigidBodyController::OnHorizontalCollision(const RaycastHitInfo& _hitInfo, Vector2& _refDeltaVelocity)
{
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Enemy)
	{
		return false;
	}
	else if (const auto _platform = _hitInfo.collider->GetGameObject()->GetComponent<Platform>();
		_platform != nullptr)
	{
		if (_platform->GetAllowJumpThrough() == true
			|| _platform->GetAllowJumpThrough() == true)
		{
			// 점프로 뚫을 수 있거나 하단 점프 가능한 플랫폼과의
			// 가로축 충돌은 무시합니다.

			return false;
		}
	}

	return RigidBodyController::OnHorizontalCollision(_hitInfo, _refDeltaVelocity);
}

void PlayerRigidBodyController::Jump()
{
	if (m_bDoJump == false)
		return;

	m_bDoJump = false;

	if (m_bInAirLastFrame == true)
	{
		// 공중 점프를 사용할 수 있는 횟수만큼 모두 사용했다면
		// 건너뜁니다.
		if (m_JumpInAirCount >= m_MaxJumpInAir)
			return;

		++m_JumpInAirCount;
	}

	deltaVelocity.y = m_JumpForce;
	m_bDidJumpThisFrame = true;
}

void PlayerRigidBodyController::DownJump(const RaycastHitInfo& _hitInfo)
{
	if (m_bDoDownJump == false)
		return;

	m_bDoDownJump = false;

	if (const auto _platform = _hitInfo.collider->GetGameObject()->GetComponent<Platform>();
		_platform != nullptr)
	{
		if (_platform->GetAllowDownJump() == true)
		{
			// 하단 점프하는 플랫폼 대상 정보를 기억하고,
			// 공중 체공 상태로 변경합니다.
			m_JumpDownTarget = _hitInfo.collider;
			m_bInAirLastFrame = true;
		}
	}
}

void PlayerRigidBodyController::Dash()
{
	if (m_bDoDash == false)
		return;

	m_bDoDash = false;

	if (m_Horizontal == 0)
	{
		// 이동하고 있는 방향이 없는 상태에서 대쉬 입력은 무시됩니다.
		return;
	}

	if (m_bAlreadyUseDashInAir == true)
	{
		// 공중에 떠 있는 상태에서 이미 대쉬를 사용했다면
		// 이번 대쉬 입력을 무시합니다.
		return;
	}

	if (GameTime::Instance.GetGameTimeMillis() - (m_LastDashStartTimeMillis + m_DashDurationMillis)
		< m_DashCooldownTimeMillis)
	{
		// 아직 대쉬 쿨타임이 남아있는 경우라면
		// 이번 대쉬 입력을 무시합니다.
		return;
	}

	// 대시를 시작합니다.
	m_bDash = true;
	m_LastDashStartTimeMillis = GameTime::Instance.GetGameTimeMillis();
	m_DashDirection = m_Horizontal;
	deltaVelocity = Vector2::Zero;

	if (m_bInAirLastFrame == true)
	{
		// 공중에서 대쉬를 사용한 것이라면,
		// 공중 대쉬 사용 여부를 기록합니다.
		m_bAlreadyUseDashInAir = true;
	}

	m_bStartDashThisFrame = true;
}

void PlayerRigidBodyController::Horizontal(int _horizontal)
{
	m_Horizontal = _horizontal;
}

void PlayerRigidBodyController::DoDash()
{
	m_bDoDash = true;
}
