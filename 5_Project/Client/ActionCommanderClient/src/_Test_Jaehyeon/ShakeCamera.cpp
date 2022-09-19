#include "pch.h"
#include "ShakeCamera.h"

#include "GameObject.h"
#include "Player.h"
#include "GameTime.h"
#include "Screen.h"
#include "Sound.h"

ShakeCamera::ShakeCamera(SanyoniEngine::GameObject* _gameObject, tstring _typename)
	:ComponentBase(_gameObject, _typename),
	m_XAxisDistribution(std::uniform_real_distribution<float>(0.f, 0.f)),
	m_ShakingDurationMillis(300),
	m_LastShakingStartTimeMillis(0),
	m_bShake(false),
	m_MapSize(Screen::Instance.GetSize()),
	m_bClear(false)
{

}

void ShakeCamera::Tick()
{
	ComponentBase::Tick();

	if (m_Player == nullptr)
		return;

	// ���� ����
	std::random_device rd;
	std::mt19937 rng(rd());

	const auto _transform = m_GameObject->GetComponent<Transform>();
	const auto _playerTansform = m_Player->GetGameObject()->GetComponent<Transform>();
	const Vector2 _screenSize = Screen::Instance.GetSize();
	auto _deltaPosition
		= Vector2::Lerp(_transform->GetWorldPosition(),
			_playerTansform->GetWorldPosition(),
			0.2f);;

	// �÷��̾��� ��ġ�� �� ����� ��
		// �߽ɿ��� �� ������ ��ŭ�� ���� �� 0 ���� �۰ų�, �� ������ ���� ũ��
		// �߽��� 0 + ��ũ�� ������ / 2 or �� ������ - ��ũ�� ������ / 2 �� ���ش�.
	Sound::Instance.UpdateSystem();
	Sound::Instance.PlayLoop("Main_BGM.wav");

	if (!m_bShake)
	{
		// �������� ����.. �ϳ��� ����.

		// ���� Scene�� 4��, ĳ���Ͱ� Ư�� ��ġ�� ���� => ȭ���� ��ġ ũ�������� �ٲ��ش�.
		const auto _nowScene = GetGameObject()->GetScene();
		if (_nowScene == nullptr)
			return;
		
		if (
			(_nowScene->GetName() == TEXT("level_4"))
			&&
			(_playerTansform->GetWorldPosition().x >= 64 * 22.f)
			)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(34 * 64.f, 9 * 64.f), 0.05f);

			_transform->SetWorldPosition(_deltaPosition);

			if(!m_bClear)
			{
				Sound::Instance.StopMusic();
			}

			Sound::Instance.PlayLoop("Clear_BGM.wav");

			m_bClear = true;
			return;
		}
		else
		{
			m_bClear = false;
		}

		// ���� ���
		if (
			(_playerTansform->GetWorldPosition().x - (_screenSize.x / 2) < 0)
			&&
			(_playerTansform->GetWorldPosition().y - (_screenSize.y / 2) < 0)
			)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(_screenSize.x / 2, _screenSize.y / 2), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ���� ���.
		else if (
			(_playerTansform->GetWorldPosition().x + (_screenSize.x / 2) > m_MapSize.x)
			&&
			(_playerTansform->GetWorldPosition().y - (_screenSize.y / 2) < 0)
			)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(m_MapSize.x - (_screenSize.x / 2), _screenSize.y / 2), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ���� �ϴ�.
		else if (
			(_playerTansform->GetWorldPosition().x - (_screenSize.x / 2) < 0)
			&&
			(_playerTansform->GetWorldPosition().y + (_screenSize.y / 2) > m_MapSize.y)
			)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(_screenSize.x / 2, m_MapSize.y - (_screenSize.y / 2)), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ���� �ϴ�
		else if (
			(_playerTansform->GetWorldPosition().x + (_screenSize.x / 2) > m_MapSize.x)
			&&
			(_playerTansform->GetWorldPosition().y + (_screenSize.y / 2) > m_MapSize.y)
			)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(m_MapSize.x - (_screenSize.x / 2), m_MapSize.y - (_screenSize.y / 2)), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ����
		else if (_playerTansform->GetWorldPosition().x - (_screenSize.x / 2) < 0)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(_screenSize.x / 2, _playerTansform->GetWorldPosition().y), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ����
		else if (_playerTansform->GetWorldPosition().x + (_screenSize.x / 2) > m_MapSize.x)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(m_MapSize.x - (_screenSize.x / 2), _playerTansform->GetWorldPosition().y), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// ���
		else if (_playerTansform->GetWorldPosition().y - (_screenSize.y / 2) < 0)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(_playerTansform->GetWorldPosition().x, _screenSize.y / 2), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		// �ϴ�
		else if (_playerTansform->GetWorldPosition().y + (_screenSize.y / 2) > m_MapSize.y)
		{
			_deltaPosition
				= Vector2::Lerp(_transform->GetWorldPosition(),
					Vector2(_playerTansform->GetWorldPosition().x, m_MapSize.y - (_screenSize.y / 2)), 0.2f);

			_transform->SetWorldPosition(_deltaPosition);
		}
		else
		{
			_transform->SetWorldPosition(_deltaPosition);
		}

		


	}
	else
	{
		// ����ŷ�� ���ӽð� ���� ����ŷ�� �����մϴ�.
		const long long _shakeElapsedTimeMillis = GameTime::Instance.GetGameTimeMillis() - m_LastShakingStartTimeMillis;


		float _rdXValue = std::lerp(m_XAxisDistribution(rng), 0, _shakeElapsedTimeMillis / m_ShakingDurationMillis);
		float _rdYValue = std::lerp(m_YAxisDistribution(rng), 0, _shakeElapsedTimeMillis / m_ShakingDurationMillis);

		//float _rdXValue = m_XAxisDistribution(rng);
		//float _rdYValue = m_YAxisDistribution(rng);
		_transform->SetWorldPosition(_transform->GetWorldPosition() + Vector2(_rdXValue, _rdYValue));

		if (_shakeElapsedTimeMillis > m_ShakingDurationMillis)
		{
			m_bShake = false;
		}
	}
}

void ShakeCamera::StartShake(float _xIntensity, float _yIntensity, long long _shakeDuration)
{
	m_XAxisDistribution = std::uniform_real_distribution<float>(-_xIntensity, _xIntensity);
	m_YAxisDistribution = std::uniform_real_distribution<float>(-_yIntensity, _yIntensity);

	m_ShakingDurationMillis = _shakeDuration;

	m_bShake = true;
	m_LastShakingStartTimeMillis = GameTime::Instance.GetGameTimeMillis();
}
