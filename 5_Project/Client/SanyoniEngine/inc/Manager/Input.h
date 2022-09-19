#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"
#include "EVirtualKey.h";
#include "EKeyState.h"
#include "KeyMessageInfo.h"
#include "MouseWheelMessageInfo.h"

namespace SanyoniEngine
{
	constexpr int VKEY_COUNT = 256;

	inline bool operator&(EKeyState _lhs, byte _rhs)
	{
		return static_cast<int>(_lhs) & _rhs;
	}

	inline bool operator&(EKeyState _state, EKeyStateBit _bit)
	{
		return static_cast<int>(_state) & static_cast<int>(_bit);
	}

	class Input final
	{
		DECLARE_SINGLETON_CLASS(Input)

	public:
		/**
		 * \brief 추적 대상의 키의 상태와 마지막으로 갱신된 프레임 시간을 포함하는 구조체입니다.
		 */
		struct KeyInfo
		{
			EKeyState state;
			std::chrono::system_clock::time_point lastUpdatedTime;
		};

	public:
		void Initialize(HWND _hWnd);

		void Tick();

		/**
		 * \brief 현재 키의 상태를 반환합니다.
		 */
		EKeyState GetKeyState(EVirtualKey _vKey);

		/*
		 * 현재 키가 눌려있는지에 대한 상태를 반환합니다.
		 * 다른 말로, 키의 상태가 Down 또는 Hold일 때 true를 반환합니다.
		 */
		bool GetKeyPress(EVirtualKey _vKey);

		int GetAxisHorizontal();

		int GetAxisVertical();

	private:
		static constexpr long long INPUT_TRACE_THRESHOLD_TIME = 500;

		static constexpr int MAX_KEY_MESSAGE_QUEUE_SIZE = 50;

		static constexpr int MAX_MOUSE_WHEEL_MESSAGE_QUEUE_SIZE = 50;

		HWND m_hWnd;

		/**
		 * \brief 지금까지 추적된 키의 정보를 저장하는 배열입니다.
		 *
		 * 추적된 상태들은 항상 현재 프레임에서 갱신되었다는 것을 보장하지 않습니다.
		 * 만약, 이번 프레임에서 처음으로 키의 상태가 요청되었을 때
		 * 키의 정보의 상태를 갱신하고, 또한 가장 마지막으로 갱신된 시간을 이번 프레임의 시간으로 갱신합니다.
		 * 그렇지 않다면 이 키의 상태는 이번 프레임에서 이미 추적된 것이므로, 이 배열의 값은 갱신되지 않습니다.
		 */
		KeyInfo m_KeyInfos[VKEY_COUNT];

		bool m_LastKeyMessageDowns[VKEY_COUNT];

		std::queue<KeyMessageInfo> m_KeyMessageQueue;
		std::queue<MouseWheelMessageInfo> m_MouseWheelMessageQueue;

		Vector2 m_CurrentMousePosition;
		Vector2 m_PreviousMousePosition;
		Vector2 m_MouseMovementDelta;
		Vector2 m_LastMouseDragStartPosition;
		Vector2 m_LastMouseDragEndPosition;
		bool m_bMouseMove;

		int m_MouseWheelDelta;
		Vector2 m_MouseWheelPosition;

	public:
		inline Vector2 GetMousePosition() const { return m_CurrentMousePosition; }

		inline Vector2 GetMouseMovementDelta() const { return m_MouseMovementDelta; }

		inline Vector2 GetLastMouseDragStartPosition() const { return m_LastMouseDragStartPosition; }

		inline Vector2 GetLastMouseDragEndPosition() const { return m_LastMouseDragEndPosition; }

	public:
		/// <summary>
		/// (not_for_clients) KeyMessageQueue에 메세지 정보를 등록합니다.
		///	이 메소드는 WinProc에서 WM_KEYDOWN과 WM_KEYUP 메세지를 받았을 때만 호출해야 합니다.
		/// </summary>
		inline void PushKeyMessage(const KeyMessageInfo& _messageInfo)
		{
			// 중복 키 메세지를 추가하지 않도록
			// message의 keyDown 상태가 이미 추적된 상태와 일치한지 여부를 검사하여 필터링합니다.
			if (m_LastKeyMessageDowns[static_cast<int>(_messageInfo.vKey)] == _messageInfo.isDown)
				return;

			m_LastKeyMessageDowns[static_cast<int>(_messageInfo.vKey)] = _messageInfo.isDown;

			m_KeyMessageQueue.push(_messageInfo);

			if (m_KeyMessageQueue.size() > MAX_KEY_MESSAGE_QUEUE_SIZE)
				m_KeyMessageQueue.pop();
		}

		/// <summary>
		/// keyMessageQueue에서 메세지를 가져옵니다.
		/// </summary>
		inline bool PopKeyMessage(KeyMessageInfo& _outInfo)
		{
			if (m_KeyMessageQueue.empty())
				return false;

			_outInfo = m_KeyMessageQueue.front();
			m_KeyMessageQueue.pop();
			return true;
		}

		/**
		 * \brief (not_for_clients) MouseWheelMessageQueue에 메세지 정보를 등록합니다.
		 * 이 메소드는 WinProc에서 WM_MOUSEWHEEL 메세지를 받았을 때만 호출해야 합니다.
		 */
		inline void PushMouseWheelMessage(const MouseWheelMessageInfo& _messageInfo)
		{
			m_MouseWheelMessageQueue.push(_messageInfo);

			if (m_MouseWheelMessageQueue.size() > MAX_MOUSE_WHEEL_MESSAGE_QUEUE_SIZE)
				m_MouseWheelMessageQueue.pop();
		}

		/**
		 * \brief MouseWheelMessageQueue에서 메세지를 가져옵니다.
		 */
		inline bool PopMouseWheelMessage(MouseWheelMessageInfo& _outInfo)
		{
			if (m_MouseWheelMessageQueue.empty())
				return false;

			_outInfo = m_MouseWheelMessageQueue.front();
			m_MouseWheelMessageQueue.pop();
			return true;
		}
	};
}
