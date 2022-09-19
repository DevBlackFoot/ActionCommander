#include "pch.h"
#include "BackgroundMovement.h"

#include "AABBCollider.h"
#include "Camera.h"
#include "GameObject.h"
#include "Platform.h"
#include "Resources.h"
#include "Screen.h"
#include "SpriteRenderer.h"
#include "Transform.h"

BackgroundMovement::BackgroundMovement(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void BackgroundMovement::Make(MapData const* _map, Scene* _scene)
{
	m_MapSize = _map->mapSize;
	m_TileSize = _map->tileSize;
	int _bitmapWidth = 1534; // 방법 몰라서 일단은 하드코딩

	// 배경 그려주기.
	// 한 칸 더 많이
	// Level1
	for (int i = 0; i < (m_MapSize.x / _bitmapWidth) + 1; i++)
	{
		GameObject* temp = new GameObject(_scene);
		m_BGBack.push_back(temp);
		m_BGBack[i]->GetComponent<Transform>()->SetWorldPosition(
			{static_cast<float>(_bitmapWidth * i), m_MapSize.y / 2});
		m_BGBack[i]->SetScene(_scene);
		m_BGBack[i]->AddComponent<SpriteRenderer>()->SetSprite(
			Resources::Instance.GetSpriteByName(TEXT("Assets/Map/BG/background.png")));
	}
	for (int i = 0; i < (m_MapSize.x / _bitmapWidth) + 1; i++)
	{
		GameObject* temp = new GameObject(_scene);
		m_BGMiddle.push_back(temp);
		m_BGMiddle[i]->GetComponent<Transform>()->SetWorldPosition(
			{static_cast<float>(_bitmapWidth * i), m_MapSize.y / 2});
		m_BGMiddle[i]->SetScene(_scene);
		m_BGMiddle[i]->AddComponent<SpriteRenderer>()->SetSprite(
			Resources::Instance.GetSpriteByName(TEXT("Assets/Map/BG/backgroundBackTree.png")));
	}
	for (int i = 0; i < (m_MapSize.x / _bitmapWidth) + 1; i++)
	{
		GameObject* temp = new GameObject(_scene);
		m_BGFront.push_back(temp);
		m_BGFront[i]->GetComponent<Transform>()->SetWorldPosition(
			{static_cast<float>(_bitmapWidth * i), m_MapSize.y / 2});
		m_BGFront[i]->SetScene(_scene);
		m_BGFront[i]->AddComponent<SpriteRenderer>()->SetSprite(
			Resources::Instance.GetSpriteByName(TEXT("Assets/Map/BG/backgroundFrontTree.png")));
	}

	// 왼쪽 벽
	m_Field[0] = new GameObject(_scene);
	m_Field[0]->GetComponent<Transform>()->SetWorldPosition({-16.f, m_MapSize.y / 2});
	m_Field[0]->AddComponent<AABBCollider>()->SetWidth(32.f);
	m_Field[0]->GetComponent<AABBCollider>()->SetHeight(m_MapSize.y);

	// 오른쪽 벽
	m_Field[1] = new GameObject(_scene);
	m_Field[1]->GetComponent<Transform>()->SetWorldPosition({(m_MapSize.x + 16.f), m_MapSize.y / 2});
	m_Field[1]->AddComponent<AABBCollider>()->SetWidth(32.f);
	m_Field[1]->GetComponent<AABBCollider>()->SetHeight(m_MapSize.y);

	// 천장
	m_Field[2] = new GameObject(_scene);
	m_Field[2]->GetComponent<Transform>()->SetWorldPosition({m_MapSize.x / 2, -16.f});
	m_Field[2]->AddComponent<AABBCollider>()->SetWidth(m_MapSize.x);
	m_Field[2]->GetComponent<AABBCollider>()->SetHeight(32.f);
	m_Field[2]->AddComponent<Platform>()->SetAllowJumpThrough(false);
}

void BackgroundMovement::Tick()
{
	ComponentBase::Tick();

	int _bitmapHieght = 861; // 방법 몰라서 일단은 하드코딩
	Vector2 _screenSize = Screen::Instance.GetSize();

	// 배경 이동 (세로로 카메라를 따라가도록)

	const auto _mainCamera = SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetGameObject();

	if (_mainCamera == nullptr)
		return;

	if (_mainCamera->GetComponent<Transform>()->GetWorldPosition().y - _bitmapHieght / 2 < 0)
	{
		for (int i = 0; i < m_BGBack.size(); i++)
		{
			const auto _deltaPosBack = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x,
					static_cast<float>(_bitmapHieght / 2)
				}, 0.8f);
			const auto _deltaPosMiddle = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x
					+ _mainCamera->GetComponent<Transform>()->GetWorldPosition().x,
					static_cast<float>(_bitmapHieght / 2)
				}, 0.8f);

			m_BGBack[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
			m_BGMiddle[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosMiddle);
			m_BGFront[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
		}
	}
	// Level1은 하한이 3 Grid, 2는 6그리드
	else if (_mainCamera->GetComponent<Transform>()->GetWorldPosition().y + _bitmapHieght / 2 > m_MapSize.y + (m_TileSize * 2))
	{
		for (int i = 0; i < m_BGBack.size(); i++)
		{
			const auto _deltaPosBack = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x,
					_screenSize.y + (_bitmapHieght / 2)
				}, 0.8f);
			const auto _deltaPosMiddle = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x
					+ _mainCamera->GetComponent<Transform>()->GetWorldPosition().x,
					_screenSize.y + (_bitmapHieght / 2)
				}, 0.8f);

			m_BGBack[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
			m_BGMiddle[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosMiddle);
			m_BGFront[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
		}
	}
	else
	{
		for (int i = 0; i < m_BGBack.size(); i++)
		{
			const auto _deltaPosBack = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x,
					_mainCamera->GetComponent<Transform>()->GetWorldPosition().y
				}, 0.8f);
			const auto _deltaPosMiddle = Vector2::Lerp(
				m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition(),
				{
					m_BGBack[i]->GetComponent<Transform>()->GetWorldPosition().x
					+ _mainCamera->GetComponent<Transform>()->GetWorldPosition().x,
					_mainCamera->GetComponent<Transform>()->GetWorldPosition().y
				}, 0.8f);

			m_BGBack[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
			m_BGMiddle[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosMiddle);
			m_BGFront[i]->GetComponent<Transform>()->SetWorldPosition(_deltaPosBack);
		}
	}
}
