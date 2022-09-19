#include "pch.h"
#include "TransformCamera.h"

#include "GameObject.h"
#include "GameTime.h"
#include "Input.h"

TransformCamera::TransformCamera(SanyoniEngine::GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void TransformCamera::Tick()
{
	auto _transform = m_GameObject->GetComponent<Transform>();


	if (Input::Instance.GetKeyPress(EVirtualKey::MouseMiddle))
	{
		Vector2 _movement = Input::Instance.GetMouseMovementDelta();

		_transform->SetLocalPosition(_transform->GetLocalPosition() + _movement * 2);
	}

	float _rotationDeg = _transform->GetWorldRotationDeg();

	if (Input::Instance.GetKeyPress(EVirtualKey::Key_W))
		_rotationDeg = _rotationDeg + 1;

	if (Input::Instance.GetKeyPress(EVirtualKey::Key_S))
		_rotationDeg = _rotationDeg - 1;

	_transform->SetWorldRotationDeg(_rotationDeg);
}
