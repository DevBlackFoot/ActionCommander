#include "pch.h"
#include "SanyoniEngine.h"
#include "SanyoniEngine_types.h"
#include "SanyoniTestApplication.h"

#include "AABBCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include "FollowMouse.h"
#include "FollowPlayer.h"
#include "GameTime.h"
#include "Helper.h"
#include "Scene.h"
#include "Screen.h"
#include "Transform.h"
#include "SceneManager.h"
#include "ObjectPool.h"
#include "OneShotSpriteAnimationEffect.h"
#include "Platform.h"
#include "Player.h"
#include "Resources.h"
#include "Sound.h"

namespace SanyoniEngine
{
	class SpriteRenderer;
}

using namespace SanyoniEngine;
using namespace SanyoniEngine::Collision;

void SanyoniTestApplication::OnStart()
{
	Application::OnStart();

	ObjectPool<int> _testPool;
	auto _new = _testPool.Get();

	m_Scene = new Scene(TEXT("testScene"));
	SceneManager::Instance.ReserveLoadScene(m_Scene);


	m_FollowMouse = new GameObject(m_Scene);
	m_FollowMouse->AddComponent<FollowMouse>();


	for (int i = 0; i < 50; ++i)
	{
		if (i & 0x01)
		{
			const auto _boxObject = new GameObject(m_Scene, EGameObjectTag::Platform);
			const auto _boxComponent = _boxObject->AddComponent<AABBCollider>();

			_boxComponent->SetWidth(std::rand() % 200);
			_boxComponent->SetHeight(std::rand() % 200);

			_boxObject->GetComponent<Transform>()->SetLocalPosition({
				static_cast<float>(std::rand() % 1500), static_cast<float>(std::rand() % 1000)
			});

			m_Colliders.push_back(_boxObject);
		}
		else
		{
			const auto _circleObject = new GameObject(m_Scene);
			const auto _circleComponent = _circleObject->AddComponent<CircleCollider>();
			_circleComponent->SetRadius(std::rand() % 50);

			_circleObject->GetComponent<Transform>()->SetLocalPosition({
				static_cast<float>(std::rand() % 1500), static_cast<float>(std::rand() % 1000)
			});
			_circleObject->SetScene(m_Scene);

			m_Colliders.push_back(_circleObject);
		}
	}


	m_Player = new GameObject(m_Scene, EGameObjectTag::Player);
	m_Player->AddComponent<Player>();

	m_Platform = new GameObject(m_Scene, EGameObjectTag::Platform);
	m_Platform->GetComponent<Transform>()->SetWorldPosition({0, 50});
	m_Platform->AddComponent<AABBCollider>()->SetWidth(2000);
	m_Platform->GetComponent<AABBCollider>()->SetHeight(20);
	m_Platform->AddComponent<Platform>()->SetAllowJumpThrough(true);

	m_Camera = new GameObject(m_Scene);
	auto _camera = m_Camera->AddComponent<Camera>();
	m_Scene->SetMainCamera(_camera);
	// m_Camera->AddComponent<TransformCamera>();
	m_Camera->AddComponent<FollowPlayer>()->SetPlayer(
		m_Player->GetComponent<Player>());

	const auto _font = Resources::Instance.LoadFontCollectionFromFile(TEXT("Assets/Font/hy크리스탈m-yoond1004_0.ttf"));
	m_TextFormat = Resources::Instance.CreateTextFormat(_font, 30);

	auto _testEventRange1 = m_Player->GetComponent<Animator>()->GetEvents(0, 0.5f);
	auto _testEventRange2 = m_Player->GetComponent<Animator>()->GetEvents(0, 1.f);
	auto _testEventRange3 = m_Player->GetComponent<Animator>()->GetEvents(1.f, 2.f);

	m_OneShotEffect = new GameObject(m_Scene);
	m_OneShotEffect->AddComponent<OneShotSpriteAnimationEffect>()->Set(
		Resources::Instance.LoadAnimationClipFromJsonFile(TEXT("Assets/Animation/Player/Player_BasicAttack1.json"))
	);

	Sound::Instance.InitSound();
	Sound::Instance.PlayLoop("Main_BGM.wav");
}

void SanyoniTestApplication::OnUpdate()
{
	Application::OnUpdate();

	/* change transform of colliders */
	static float _transformFactor = 0;
	_transformFactor += 0.005f;

	const float _cos = cos(_transformFactor);
	const float _sin = sin(_transformFactor);

	for (auto _collider : m_Colliders)
	{
		_collider->GetComponent<Transform>()->SetLocalScale({_cos, _cos});
		_collider->GetComponent<Transform>()->SetLocalRotationRad(_sin);
	}

	/* draw grids */
	const auto _camera = m_Camera->GetComponent<Camera>();

	const int _startX = 0;
	const int _endX = 1280;
	const int _startY = 0;
	const int _endY = 720;

	for (int _y = _startY; _y < _endY; _y = _y + 10)
	{
		const Color& _color = _y % 100 == 0 ? Color::White : Color::Gray;
		const float _strokeWidth = _y % 100 == 0 ? .2f : .1f;
		Screen::Instance.DrawLineScreenSpace(_color, Point2(_startX, _y), Point2(_endX, _y),
		                                     _camera->GetCameraWorldTM(),
		                                     _strokeWidth);
	}

	for (int _x = _startX; _x < _endX; _x = _x + 10)
	{
		const Color& _color = _x % 100 == 0 ? Color::White : Color::Gray;
		const float _strokeWidth = _x % 100 == 0 ? .2f : .1f;
		Screen::Instance.DrawLineScreenSpace(_color, Point2(_x, _startY), Point2(_x, _endY),
		                                     _camera->GetCameraWorldTM(),
		                                     _strokeWidth);
	}
	/* end draw grids */

	/* ray vs ray collision */

	const float _length = 300;
	const Vector2 _line1Start{100, 100};
	const Vector2 _line1Direction = Vector2{_cos, _sin}.Normalize();
	const Vector2 _line1End = _line1Start + _line1Direction * _length;

	RaycastHitInfo _hitInfo;
	CollisionManager::Instance.Raycast(_line1Start, _line1Direction, _length, _hitInfo);

	bool _collision = _hitInfo.collider != nullptr;
	Vector2 _outPosition = _hitInfo.point;

	Screen::Instance.DrawLineWorldSpace(_collision ? Color::Red : Color::White,
	                                    {_line1Start.x, _line1Start.y}, {_line1End.x, _line1End.y});

	if (_collision)
		Screen::Instance.DrawCircleWorldSpace(Color::Blue, {{_outPosition.x, _outPosition.y}, 10, 10});


	Screen::Instance.DrawTextScreenSpace(TEXT("안녕하세요 산요니예요 저는 스크린에 띄워지구요"), m_TextFormat, Vector2::Zero, Color::White);
	Screen::Instance.DrawTextWorldSpace(TEXT("저는 월드에 띄워진답니다"), m_TextFormat, Vector2::Zero, Color::White);

	Screen::Instance.DrawTextScreenSpace(
		StringHelper::Format(TEXT("%f: %f"),
		                     GameTime::Instance.GetGameTime(),
		                     m_Player->GetComponent<Animator>()->GetFloat(
			                     TEXT("UnityEngine.Transform"), TEXT("m_LocalPosition.x"),
			                     GameTime::Instance.GetGameTime())
		),
		m_TextFormat,
		{500, 10},
		Color::White
	);

	Screen::Instance.DrawTextScreenSpace(
		m_Player->GetComponent<Animator>()->GetSprite(GameTime::Instance.GetGameTime()),
		m_TextFormat,
		{10, 200},
		Color::White
	);

	Screen::Instance.DrawTextScreenSpace(
		StringHelper::Format(TEXT("%d"), m_Player->GetComponent<Player>()->GetAttacking()),
		m_TextFormat,
		{10, 300},
		Color::White
	);

	auto _playerPosition = m_Player->GetComponent<Transform>()->GetWorldPosition();
	const float _offsetX = 100;
	const float _offsetY = 0;
	const float _rectHalfWidth = 50 / 2;
	const float _rectHalfHeight = 50 / 2;
	Screen::Instance.DrawRectangleWorldSpace(
		Color::White,
		{
			_playerPosition.x + _offsetX - _rectHalfWidth,
			_playerPosition.y + _offsetY - _rectHalfHeight,
			_playerPosition.x + _offsetX + _rectHalfWidth,
			_playerPosition.y + _offsetY + _rectHalfHeight
		}
	);

	Screen::Instance.DrawLineWorldSpace(
		Color::Red,
		{_playerPosition.x, _playerPosition.y},
		{_playerPosition.x + _offsetX, _playerPosition.y + _offsetY}
	);
}
