#include "SanyoniEngine_pch.h"
#include "Transform.h"

#include "GameObject.h"
#include "Matrix3x2.h"

using namespace SanyoniEngine;

Transform::Transform(GameObject* _gameObject, tstring _typeName) :
	Transform(_gameObject, Vector2::Zero, 0.f, Vector2::One, _typeName)
{
}

Transform::Transform(GameObject* _gameObject, const Vector2& _position, float rotation, const Vector2 _scale, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_LocalPosition(_position),
	m_LocalRotationRad(rotation),
	m_LocalScale(_scale),
	m_LocalTM(Matrix3x2::Identity),
	m_WorldTM(Matrix3x2::Identity),
	m_bWorldTMDirt(true),
	m_bLocalTMDirt(true)
{
}

void Transform::UpdateWorldMatrix(Matrix3x2 _parentTM)
{
	m_WorldTM = GetLocalTM() * _parentTM;

	m_bWorldTMDirt = false;
}

void Transform::UpdateLocalMatrix()
{
	m_LocalTM =
		Matrix3x2::MakeScaleMatrix(m_LocalScale)
		* Matrix3x2::MakeRotateMatrixWithRad(m_LocalRotationRad)
		* Matrix3x2::MakeTranslateMatrix(m_LocalPosition);

	m_bLocalTMDirt = false;
}

void Transform::SetLocalPosition(const Vector2& _newPosition)
{
	m_LocalPosition = _newPosition;

	m_bLocalTMDirt = true;
	MarkWorldTMDirt();
}

void Transform::SetLocalRotationRad(const float _newRotationRad)
{
	m_LocalRotationRad = _newRotationRad;

	m_bLocalTMDirt = true;
	MarkWorldTMDirt();
}

void Transform::SetLocalRotationDeg(const float _newRotationDeg)
{
	m_LocalRotationRad = _newRotationDeg * DEG_TO_RAD;

	m_bLocalTMDirt = true;
	MarkWorldTMDirt();
}

void Transform::SetLocalScale(const Vector2& _newScale)
{
	m_LocalScale = _newScale;

	m_bLocalTMDirt = true;
	MarkWorldTMDirt();
}

void Transform::SetWorldPosition(const Vector2& _newPosition)
{
	const Vector2 _parentWorldPosition = m_GameObject->GetParent() == nullptr
		                                     ? Vector2::Zero
		                                     : m_GameObject->GetParent()->GetComponent<Transform>()->GetWorldPosition();

	const Vector2 _offset = _newPosition - _parentWorldPosition;
	SetLocalPosition(_offset);
}

void Transform::SetWorldRotationRad(const float _newRotationRad)
{
	const float _parentWorldRotationRad = m_GameObject->GetParent() == nullptr
		                                      ? 0
		                                      : m_GameObject->GetParent()->GetComponent<Transform>()->
		                                                      GetWorldRotationRad();

	const float _offset = _newRotationRad - _parentWorldRotationRad;
	SetLocalRotationRad(_offset);
}

void Transform::SetWorldRotationDeg(const float _newRotationDeg)
{
	SetWorldRotationRad(_newRotationDeg * DEG_TO_RAD);
}

void Transform::SetWorldScale(const Vector2& _newScale)
{
	const Vector2 _parentWorldScale = m_GameObject->GetParent() == nullptr
		                                  ? Vector2::One
		                                  : m_GameObject->GetParent()->GetComponent<Transform>()->GetWorldScale();

	const Vector2 _ratio = Vector2{_newScale.x / _parentWorldScale.x, _newScale.y / _parentWorldScale.y};
	SetLocalScale(_ratio);
}

Vector2 Transform::GetWorldPosition()
{
	const Matrix3x2 _worldTM = GetWorldTM();

	// { _worldTM.dx, _worldTM.dy }
	return {m_WorldTM._31, m_WorldTM._32};
}

float Transform::GetWorldRotationRad()
{
	const Matrix3x2 _worldTM = GetWorldTM();
	const auto _direction = Vector2(_worldTM._22, _worldTM._21).Normalize();
	const float _rad = _direction.y >= 0
		                   ? std::acos(_direction.x)
		                   : std::acos(_direction.x) * -1;

	return _rad;
}

float Transform::GetWorldRotationDeg()
{
	const float _rad = GetWorldRotationRad();
	return _rad * RAD_TO_DEG;
}

Vector2 Transform::GetWorldScale()
{
	const Matrix3x2 _worldTM = GetWorldTM();
	const float _xScale = sqrt(_worldTM._11 * _worldTM._11 + _worldTM._12 * _worldTM._12);
	const float _yScale = sqrt(_worldTM._21 * _worldTM._21 + _worldTM._22 * _worldTM._22);

	return Vector2{_xScale, _yScale};
}

Matrix3x2 Transform::GetLocalTM()
{
	if (m_bLocalTMDirt == true)
		UpdateLocalMatrix();

	return m_LocalTM;
}

Matrix3x2 Transform::GetWorldTM()
{
	if (m_bWorldTMDirt == true)
	{
		// 부모의 WorldMatrix를 가져와 새로고칩니다.
		if (m_GameObject->GetParent() == nullptr)
			UpdateWorldMatrix(Matrix3x2::Identity);
		else
			UpdateWorldMatrix(m_GameObject->GetParent()->GetComponent<Transform>()->GetWorldTM());
	}

	return m_WorldTM;
}

void Transform::MarkWorldTMDirt()
{
	this->m_bWorldTMDirt = true;

	// 이 게임 오브젝트의 하위 자식 오브젝트들의 TM도 모두 바뀌었기 때문에
	// MarkWorldTMDirt를 전파합니다.
	const auto& _childs = m_GameObject->GetChilds();
	for (auto _child : _childs)
		_child->GetComponent<Transform>()->MarkWorldTMDirt();
}
