#include "SanyoniEngine_pch.h"
#include "Animator.h"

#include "AnimationEventHandlerBase.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Resources.h"
#include "RigidBodyController.h"
#include "SpriteRenderer.h"

using namespace SanyoniEngine;

Animator::Animator(GameObject* _gameObject, const tstring& _typeName) :
	ComponentBase(_gameObject, _typeName),
	m_CurrentClip(nullptr),
	m_ClipStartTime(0.f),
	m_ClipTimeLastFrame(0),
	m_ClipLoopCountLastFrame(0),
	m_bReachedEnd(false),
	m_PositionLastFrame()
{
}

void Animator::PreTick()
{
	ComponentBase::PreTick();

	// TODO: 위치 이동을 반영합니다.
}

void Animator::Tick()
{
	ComponentBase::Tick();

	if (m_CurrentClip == nullptr)
		return;

	if (m_CurrentClip->bIsLooping == false
		&& m_bReachedEnd == true)
	{
		// 루프되지 않는 애니메이션이 종료되었다면
		// 더 이상 실행하지 않습니다.
		return;
	}


	const float _playingClipElapsedTime = GameTime::Instance.GetGameTime() - m_ClipStartTime;
	const float _clipTime = std::fmod(_playingClipElapsedTime, m_CurrentClip->length);
	const int _clipLoopCount = _playingClipElapsedTime / m_CurrentClip->length;


	if (m_CurrentClip->bIsLooping == false
		&& _playingClipElapsedTime >= m_CurrentClip->length)
	{
		// 논 루프 애니메이션 클립을 재생할 때,
		// 클립의 길이만큼 모두 재생이 되었다면 플래그를 활성화합니다.
		m_bReachedEnd = true;
	}


	if (const auto _eventHandler = GetGameObject()->GetComponent<AnimationEventHandlerBase>();
		_eventHandler != nullptr)
	{
		// 이벤트를 검사하고, 있다면 호출합니다.


		// 지난 프레임의 재생 시간과 이번 프레임의 재생 시간 사이의 이벤트 목록을 얻어옵니다.
		std::vector<AnimationEventInfo> _events;
		if (m_CurrentClip->bIsLooping == true)
		{
			// 루프 애니메이션 클립을 재생할 때,
			// 이번 프레임에서 루프를 도는 지 그 여부를 확인하고
			// 돌았다면 이벤트 정보를 빠짐없이 가져올 수 있도록 특별히 처리합니다.

			if (_clipLoopCount != m_ClipLoopCountLastFrame)
			{
				const auto _events1 = GetEvents(m_ClipTimeLastFrame, std::numeric_limits<float>::max());
				const auto _events2 = GetEvents(0, _clipTime);

				_events = _events1;
				_events.insert(_events.end(), _events2.begin(), _events2.end());
			}
			else
			{
				_events = GetEvents(m_ClipTimeLastFrame, _clipTime);
			}
		}
		else
		{
			// 이번 프레임에서 클립의 재생이 끝났다면,
			// 저번 프레임의 clip time 이후의 모든 이벤트를 가져옵니다.
			// 그렇지 않은 경우, 일반적인 방법으로 재생 시간 범위 내의 이벤트를 가져옵니다.

			_events = m_bReachedEnd == true
				          ? GetEvents(m_ClipTimeLastFrame, std::numeric_limits<float>::max())
				          : GetEvents(m_ClipTimeLastFrame, _clipTime);
		}


		// 이벤트 핸들러에게 이벤트 실행을 맡깁니다.
		for (auto _e : _events)
		{
			_eventHandler->OnHandle(_e.functionName, _e.parameter);
		}
	}

	if (m_CurrentClip->bIsLooping == false)
	{
		// TODO: 루프 애니메이션이 아닐 때에만 움직임이 반영되도록 하였습니다. 만약 루프 애니메이션에 움직임 값이 포함된다면 무시되는 상태입니다.

		if (HasFloat(TEXT("UnityEngine.Transform"), TEXT("m_LocalPosition.x"))
			|| HasFloat(TEXT("UnityEngine.Transform"), TEXT("m_LocalPosition.y")))
		{
			// transform.position 키프레임이 있을 때에만 반영합니다.

			if (const auto _controller = GetGameObject()->GetComponent<RigidBodyController>();
				_controller != nullptr)
			{
				// 이동 값을 반영합니다.
				// TODO: 루프 애니메이션일 때, 새로운 루프로 돌아오기 직전의 값들이 손실되지 않고 잘 적용되도록 수정해주어야 합니다.

				float _positionX = GetFloat(TEXT("UnityEngine.Transform"), TEXT("m_LocalPosition.x"), _clipTime);
				float _positionY = GetFloat(TEXT("UnityEngine.Transform"), TEXT("m_LocalPosition.y"), _clipTime);
				const Vector2 _position{_positionX, _positionY};
				const Vector2 _deltaPosition = _position - m_PositionLastFrame;

				_controller->SetOverrideDeltaVelocity(_deltaPosition * GameTime::Instance.GetDeltaTime());

				m_PositionLastFrame = _deltaPosition;
			}
		}
	}


	if (const auto _renderer = GetGameObject()->GetComponent<SpriteRenderer>();
		_renderer != nullptr)
	{
		// 스프라이트를 변경합니다.

		if (m_bReachedEnd == false)
		{
			_renderer->SetSprite(
				Resources::Instance.GetSpriteByName(
					GetSprite(_clipTime))
			);
		}
	}


	m_ClipLastFrame = m_CurrentClip;
	m_ClipTimeLastFrame = _clipTime;
	m_ClipLoopCountLastFrame = _clipLoopCount;
}

float Animator::GetFloat(const tstring& _typeName, const tstring& _propertyName, float _time) const
{
	if (m_CurrentClip == nullptr)
		return 0.f;

	// 이름에 해당하는 float value가 있는지 검사합니다.
	auto _valueCurveIter = std::ranges::find_if(m_CurrentClip->valueCurves, [&](const FloatValueCurve& _curve)
	{
		return _curve.typeName._Equal(_typeName) && _curve.propertyName._Equal(_propertyName);
	});

	if (_valueCurveIter == m_CurrentClip->valueCurves.end())
	{
		// 이름에 해당하는 curve가 없을 때에는 더 이상 진행하지 않습니다.
		return 0.f;
	}

	if (_valueCurveIter->keyframes.empty() == true)
	{
		// 같은 이름의 curve를 찾았으나, 어떠한 값도 기록되어 있지 않을 때에는 더 이상 진행하지 않습니다.
		return 0.f;
	}
	else if (_valueCurveIter->keyframes.size() == 1)
	{
		// 한 개의 프레임밖에 없는 경우, 선형보간하지 않습니다.
		return _valueCurveIter->keyframes[0].value;
	}


	// TODO: 시간대로 오름차순 정렬되어 있다는 것을 보장해주어야 합니다.

	// 재생 위치보다 가장 가까우면서 다음에 위치한 프레임을 찾습니다.
	auto _frameIter = std::lower_bound(
		_valueCurveIter->keyframes.begin(), _valueCurveIter->keyframes.end(),
		_time,
		[](const FloatValueCurve::FloatValueCurveKeyframe& _keyframe, float _targetTime)
		{
			return _keyframe.time <= _targetTime;
		});

	if (_frameIter == _valueCurveIter->keyframes.end())
	{
		// 만약 가장 마지막 프레임의 시간보다 초과한 시간대라면
		// 선형 보간하지 않고 가장 마지막 프레임의 값을 그대로 반환합니다.
		return (_frameIter - 1)->value;
	}


	// 이전 위치의 프레임과 다음 위치의 프레임의 값을 선형보간한 값을 반환합니다.

	const auto _prevFrameIter = _frameIter - 1;
	const float _timeDifference = _frameIter->time - _prevFrameIter->time;
	const float _timeAfterPrevFrame = _time - _prevFrameIter->time;
	const float _lerpedValue = std::lerp(
		_prevFrameIter->value,
		_frameIter->value,
		_timeAfterPrevFrame / _timeDifference
	);

	return _lerpedValue;
}


std::vector<AnimationEventInfo> Animator::GetEvents(float _rangeStartTime, float _rangeEndTime) const
{
	if (m_CurrentClip == nullptr)
		return {};

	if (m_CurrentClip->eventInfos.empty() == true)
	{
		// clip 내 event 정보가 없을 때에는 더 이상 진행하지 않습니다.
		return {};
	}

	// 실행시킬 이벤트 목록의 시작 iterator를 구합니다.
	auto _eventRangeStart = m_CurrentClip->eventInfos.begin();
	for (; _eventRangeStart != m_CurrentClip->eventInfos.end();
	       ++_eventRangeStart)
	{
		if (_eventRangeStart->time >= _rangeStartTime)
			break;
	}

	// 실행시킬 이벤트 목록의 끝 iterator를 구합니다.
	// 끝 iterator가 가리키는 이벤트는 실행 대상에 포함되지 않습니다.
	auto _eventRangeEnd = _eventRangeStart;
	for (; _eventRangeEnd != m_CurrentClip->eventInfos.end();
	       ++_eventRangeEnd)
	{
		if (_eventRangeEnd->time >= _rangeEndTime)
			break;
	}

	return std::vector<AnimationEventInfo>{_eventRangeStart, _eventRangeEnd};
}

float Animator::GetPositionX(float _time) const
{
	return 0.f;
}

float Animator::GetPositionY(float _time) const
{
	return 0.f;
}

float Animator::GetPositionZ(float _time) const
{
	return 0.f;
}

tstring Animator::GetSprite(float _time) const
{
	if (m_CurrentClip == nullptr)
		return {};

	if (m_CurrentClip->spriteCurve.keyframes.empty() == true)
		return {};

	if (m_CurrentClip->spriteCurve.keyframes.size() == 1)
		return m_CurrentClip->spriteCurve.keyframes[0].value;


	// TODO: 시간대로 오름차순 정렬되어 있다는 것을 보장해주어야 합니다.

	// 재생 위치보다 가장 가까우면서 다음에 위치한 프레임을 찾습니다.
	auto _frameIter = std::lower_bound(
		m_CurrentClip->spriteCurve.keyframes.begin(), m_CurrentClip->spriteCurve.keyframes.end(),
		_time,
		[](const StringValueCurve::StringValueCurveKeyframe& _keyframe, float _targetTime)
		{
			return _keyframe.time <= _targetTime;
		});

	if (_frameIter != m_CurrentClip->spriteCurve.keyframes.begin())
		--_frameIter;

	return _frameIter->value;
}

bool Animator::HasFloat(const tstring& _typeName, const tstring& _propertyName) const
{
	// 이름에 해당하는 float value가 있는지 검사합니다.
	auto _valueCurveIter = std::ranges::find_if(m_CurrentClip->valueCurves, [&](const FloatValueCurve& _curve)
	{
		return _curve.typeName._Equal(_typeName) && _curve.propertyName._Equal(_propertyName);
	});

	return _valueCurveIter != m_CurrentClip->valueCurves.end();
}

void Animator::SetCurrentClip(AnimationClip* _clip)
{
	if (_clip == m_CurrentClip)
		return;

	m_CurrentClip = _clip;

	// 이번 프레임에 새로운 클립의 재생이 시작되었다면
	// 멤버들을 초기화합니다.
	m_ClipStartTime = GameTime::Instance.GetGameTime();
	m_ClipTimeLastFrame = 0;
	m_ClipLoopCountLastFrame = 0;
	m_bReachedEnd = false;
	m_PositionLastFrame = Vector2::Zero;
}
