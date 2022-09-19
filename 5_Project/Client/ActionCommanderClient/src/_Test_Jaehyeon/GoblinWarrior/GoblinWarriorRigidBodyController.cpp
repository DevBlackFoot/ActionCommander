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

	// ���۰� ���õ� �κ� �ʱ�ȭ
	//m_bDoChase = false;

	//m_bStartChaseThisFrame = false;

	//m_Horizontal = 0;
	//m_DesiredDeltaVelocityX = 0;
}

void GoblinWarriorRigidBodyController::OnPreMove(Vector2& _refDeltaVelocity)
{
	// ��Ʈ��, chase�� ���õ� �κ��� ���⼭ ó�����ش�.

	// ��Ʈ�ѵ� �����ؼ� ���ƴٴϵ��� ó���� �� ���� ��.
		// ��ÿ� ����ϰ� => �� �� ��Ʈ��. ����. �ݴ�� ��Ʈ��. // Ư�� ���ǿ����� �ٷ� ����.

	Chase();

	// Chase�� Patroll�� �ƴϸ�. �������.

	if (m_bChase == true)
	{
		// �߰� ���� �ڵ�
		m_DesiredDeltaVelocityX = m_ChaseDirection * m_ChaseSpeed * GameTime::Instance.GetDeltaTime();

	}
	else // ���⿡ ��Ʈ�� �ڵ�
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
	// ������ �������°� �÷��̾��..? false�� ��ȯ�մϴ�.
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
	// ���� �浹 ����� ��� ���� _tag�� �÷��̾ ���͸� false�� ��ȯ�մϴ�.
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

	// �߰��� �����մϴ�.
	m_bChase = true;
	m_lastChaseStartTimeMillis = GameTime::Instance.GetGameTimeMillis();	// ��� �߰��� ���� �ð��� �ʿ� ���� ������.
	m_ChaseDirection = m_Horizontal;
	deltaVelocity = Vector2::Zero;		// ���� �ӷ��� �����ְ�..

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