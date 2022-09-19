#include "pch.h"
#include "GoblinWarrior/GoblinWarriorRigidBodyController.h"

#include "ColliderBase.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Platform.h"


GoblinWarriorRigidBodyController::GoblinWarriorRigidBodyController(GameObject* _gameObject, tstring _typeName) :
	RigidBodyController(_gameObject, _typeName),
	m_WalkSpeed(400.f),
	m_ChaseSpeed(250.f),
	m_Horizontal(0),
	m_DesiredDeltaVelocityX(0),
	m_bDoChase(false),
	m_bChase(false),
	m_bStartChaseThisFrame(false),
	m_lastChaseStartTimeMillis(0),
	m_ChaseDirection(0)
{
}

void GoblinWarriorRigidBodyController::OnStart()
{
	if (const auto _rigidBody = GetGameObject()->GetComponent<RigidBody>();
		_rigidBody != nullptr)
	{
		_rigidBody->SetExcludeCollsion({ {EGameObjectTag::Player, EGameObjectTag::Enemy} });
	}
}

void GoblinWarriorRigidBodyController::PreTick()
{
	RigidBodyController::PreTick();

	// 시작과 관련된 부분 초기화
	//m_bDoChase = false;

	//m_bStartChaseThisFrame = false;

	//m_Horizontal = 0;
	//m_DesiredDeltaVelocityX = 0;
}

void GoblinWarriorRigidBodyController::OnPreMove(Vector2& _refDeltaVelocity)
{
	// 패트롤, chase와 관련된 부분을 여기서 처리해준다.

	// 패트롤도 설정해서 돌아다니도록 처리할 수 있을 듯.
		// 대시와 비슷하게 => 몇 초 패트롤. 정지. 반대로 패트롤. // 특정 조건에서는 바로 정지.

	Chase();

	// Chase나 Patroll이 아니면. 멈춰야함.

	if (m_bChase == true)
	{
		// 추격 관련 코드
		m_DesiredDeltaVelocityX = m_ChaseDirection * m_ChaseSpeed * GameTime::Instance.GetDeltaTime();

	}
	else // 여기에 패트롤 코드
	{
		//m_DesiredDeltaVelocityX = m_Horizontal * m_WalkSpeed * GameTime::Instance.GetDeltaTime();
		m_DesiredDeltaVelocityX = 0;
	}

	_refDeltaVelocity.x = m_DesiredDeltaVelocityX;

	RigidBodyController::OnPreMove(_refDeltaVelocity);
}

void GoblinWarriorRigidBodyController::OnPostMove(Vector2& _refDeltaVelocity)
{
	RigidBodyController::OnPostMove(_refDeltaVelocity);
}

bool GoblinWarriorRigidBodyController::OnVerticalCollision(const RaycastHitInfo& _hitInfo,
	Vector2& _refDeltaVelocity)
{
	// 위에서 떨어지는게 플레이어면..? false를 반환합니다.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnVerticalCollision(_hitInfo, _refDeltaVelocity);
}

bool GoblinWarriorRigidBodyController::OnHorizontalCollision(const RaycastHitInfo& _hitInfo,
	Vector2& _refDeltaVelocity)
{
	// 옆의 충돌 방향과 상관 없이 _tag가 플레이어나 몬스터면 false를 반환합니다.
	if (const auto _tag = _hitInfo.collider->GetGameObject()->GetTag();
		_tag == EGameObjectTag::Player || _tag == EGameObjectTag::Enemy)
	{
		return false;
	}

	return RigidBodyController::OnHorizontalCollision(_hitInfo, _refDeltaVelocity);
}

void GoblinWarriorRigidBodyController::Chase()
{
	if (m_bDoChase == false)
		return;

	m_bDoChase = false;

	// 추격을 시작합니다.
	m_bChase = true;
	m_lastChaseStartTimeMillis = GameTime::Instance.GetGameTimeMillis();	// 사실 추격은 시작 시간이 필요 없지 않을까.
	m_ChaseDirection = m_Horizontal;
	deltaVelocity = Vector2::Zero;		// 현재 속력은 없애주고..

	m_bStartChaseThisFrame = true;
}

void GoblinWarriorRigidBodyController::Horizontal(int _horizontal)
{
	m_Horizontal = _horizontal;
}

void GoblinWarriorRigidBodyController::DoChase()
{
	m_bDoChase = true;
}