#include "SanyoniEngine_pch.h"
#include "Camera.h"

#include "Core.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Screen.h"

using namespace SanyoniEngine;

Camera::Camera(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_bDebug(true),
	m_LeftTopWorldPosition(Vector2::Zero),
	m_RightBottomWorldPosition(Vector2::Zero)
{
}

Matrix3x2 Camera::GetCameraWorldTM() const
{
	const auto _transform = m_GameObject->GetComponent<Transform>();

	const auto _screenSize = Screen::Instance.GetSize();
	const float _halfScreenWidth = _screenSize.x / 2;
	const float _halfScreenHeight = _screenSize.y / 2;

	const auto _tmPosition = Vector2(
		_transform->GetWorldPosition().x * -1 + _halfScreenWidth,
		_transform->GetWorldPosition().y * -1 + _halfScreenHeight
	);

	const auto _tmRotation = _transform->GetWorldRotationRad();
	const auto _tmScale = _transform->GetWorldScale();

	// TODO: Scale과 Rotation을 고려한 카메라 행렬이 아닙니다. 오로지 Translation만 적용된 행렬입니다.
	const auto _cameraWorldTM =
		Matrix3x2::MakeTranslateMatrix(_tmPosition);

	return _cameraWorldTM;
}

void Camera::Update()
{
	const auto _screenSize = Screen::Instance.GetSize();

	// TODO: CameraTM을 고려하지 않은 위치로 초기화하고 있습니다.
	m_LeftTopWorldPosition = ((GetCameraWorldTM() * -1) * Matrix3x2::Identity).GetPosition();
	m_RightBottomWorldPosition = ((GetCameraWorldTM() * -1) * Matrix3x2::MakeTranslateMatrix(_screenSize)).GetPosition();
}

void Camera::OnRender()
{
	if (Core::GetEnableDebug() == true)
	{
		auto _position = m_GameObject->GetComponent<Transform>()->GetWorldPosition();
		float _lineLength = 10;

		const Point2 _line1Point1 = {_position.x - _lineLength, _position.y};
		const Point2 _line1Point2 = {_position.x + _lineLength, _position.y};
		const Point2 _line2Point1 = {_position.x, _position.y - _lineLength};
		const Point2 _line2Point2 = {_position.x, _position.y + _lineLength};

		Screen::Instance.DrawLineWorldSpace(Color::White, _line1Point1, _line1Point2);
		Screen::Instance.DrawLineWorldSpace(Color::White, _line2Point1, _line2Point2);

		const float _left = GetLeftTopWorldPosition().x;
		const float _top = GetLeftTopWorldPosition().y;
		const float _right = GetRightBottomWorldPosition().x;
		const float _bottom = GetRightBottomWorldPosition().y;

		const Point2 _leftTopPoint = {_left, _top};
		const Point2 _leftTopPointRight = {_left + _lineLength, _top};
		const Point2 _leftTopPointDown = {_left, _top + _lineLength};

		const Point2 _leftBottomPoint = {_left, _bottom};
		const Point2 _leftBottomPointRight = {_left + _lineLength, _bottom};
		const Point2 _leftBottomPointUp = {_left, _bottom - _lineLength};

		const Point2 _rightTopPoint = {_right, _top};
		const Point2 _rightTopPointLeft = {_right - _lineLength, _top};
		const Point2 _rightTopPointDown = {_right, _top + _lineLength};

		const Point2 _rightBottomPoint = {_right, _bottom};
		const Point2 _rightBottomPointLeft = {_right - _lineLength, _bottom};
		const Point2 _rightBottomPointUp = {_right, _bottom - _lineLength};

		Screen::Instance.DrawLineWorldSpace(Color::White, _leftTopPoint, _leftTopPointRight, Matrix3x2::Identity, 5);
		Screen::Instance.DrawLineWorldSpace(Color::White, _leftTopPoint, _leftTopPointDown, Matrix3x2::Identity, 5);

		Screen::Instance.DrawLineWorldSpace(Color::White, _leftBottomPoint, _leftBottomPointRight, Matrix3x2::Identity, 5);
		Screen::Instance.DrawLineWorldSpace(Color::White, _leftBottomPoint, _leftBottomPointUp, Matrix3x2::Identity, 5);

		Screen::Instance.DrawLineWorldSpace(Color::White, _rightTopPoint, _rightTopPointLeft, Matrix3x2::Identity, 5);
		Screen::Instance.DrawLineWorldSpace(Color::White, _rightTopPoint, _rightTopPointDown, Matrix3x2::Identity, 5);

		Screen::Instance.DrawLineWorldSpace(Color::White, _rightBottomPoint, _rightBottomPointLeft, Matrix3x2::Identity, 5);
		Screen::Instance.DrawLineWorldSpace(Color::White, _rightBottomPoint, _rightBottomPointUp, Matrix3x2::Identity, 5);
	}
}
