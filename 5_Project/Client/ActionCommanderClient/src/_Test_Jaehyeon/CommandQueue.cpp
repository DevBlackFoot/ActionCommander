#include "pch.h"

#include "CommandQueue.h"
#include "Input.h"
#include "GameTime.h"

CommandQueue::CommandQueue(int _dequeCycle)
{
	// 멤버 변수 초기화.
	m_DeQueueCycle = _dequeCycle;

	m_LastDequeTimeMillis = SanyoniEngine::GameTime::Instance.GetGameTimeMillis();

	m_LastSuccessCommand = -1;

	// 커맨드 개수
	m_CommandCount = 7;

	// keyCommand 배열 초기화
	// 첫 커맨드 -> -> z => 0
	m_KeyCommandArr[0][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[0][1] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[0][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[0][3] = SanyoniEngine::EVirtualKey::None;

	// 두 번째 커맨드 <- <- z => 1
	m_KeyCommandArr[1][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[1][1] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[1][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[1][3] = SanyoniEngine::EVirtualKey::None;

	// 캐넌 디바이더 <- 아래 -> z 2
	m_KeyCommandArr[2][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[2][1] = SanyoniEngine::EVirtualKey::DownArrow;
	m_KeyCommandArr[2][2] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[2][3] = SanyoniEngine::EVirtualKey::Key_Z;

	// 캐넌 디바이더 3
	m_KeyCommandArr[3][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[3][1] = SanyoniEngine::EVirtualKey::DownArrow;
	m_KeyCommandArr[3][2] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[3][3] = SanyoniEngine::EVirtualKey::Key_Z;

	// 라이징 소드 4, 5
	m_KeyCommandArr[4][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[4][1] = SanyoniEngine::EVirtualKey::UpArrow;
	m_KeyCommandArr[4][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[4][3] = SanyoniEngine::EVirtualKey::None;

	// 라이징 소드 4, 5
	m_KeyCommandArr[5][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[5][1] = SanyoniEngine::EVirtualKey::UpArrow;
	m_KeyCommandArr[5][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[5][3] = SanyoniEngine::EVirtualKey::None;

	// 버스터 킥 6
	m_KeyCommandArr[6][0] = SanyoniEngine::EVirtualKey::DownArrow;
	m_KeyCommandArr[6][1] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[6][2] = SanyoniEngine::EVirtualKey::None;
	m_KeyCommandArr[6][3] = SanyoniEngine::EVirtualKey::None;
}

CommandQueue::~CommandQueue()
{

}

void CommandQueue::Update()
{
	// 특정 카운트 마다 dequeue 및 커맨드 성공 변수 실패로
	if (SanyoniEngine::GameTime::Instance.GetGameTimeMillis() - m_LastDequeTimeMillis > m_DeQueueCycle)
	{
		m_LastDequeTimeMillis = SanyoniEngine::GameTime::Instance.GetGameTimeMillis();

		DeQueue();
	}
	EnQueue();

	// 메세지 리스트 디버깅용 코드
	std::list<KeyMessageInfo>::iterator iter;

	/*
	int _debugCount = 0;
	
	for(iter = m_CommandList.begin(); iter != m_CommandList.end(); iter++)
	{
		DebugHelper::PrintDebugString(TEXT("%d "), *(iter));
	}

	if(_debugCount % 15 == 0)
	{
		DebugHelper::PrintDebugString(TEXT("\n"));

	}
	_debugCount++;

	if(_debugCount > 100000)
	{
		_debugCount = 0;
	}
	*/

	// Decode의 결과가 -1이 아니면(실패가 아니면) Success command Queue에 넣어준다.
	m_LastSuccessCommand = Decode();

	// 디버그용 메시지 함수
	/*
	if (m_DebugCount % 15 == 0)
	{
		DebugHelper::PrintDebugString(TEXT("%d "), m_CommandList.size());
		DebugHelper::PrintDebugString(TEXT("%d "), _commandIdx);
		DebugHelper::PrintDebugString(TEXT("\n"));
	}
	*/
}

int CommandQueue::GetSuccessCommand()
{
	return m_LastSuccessCommand;
}

/// <summary>
/// 메시지를 확인해서 up이면 keyuplist에
/// down이면 commandlist에 넣어준다. 
/// </summary>
void CommandQueue::EnQueue()
{
	KeyMessageInfo _temp;

	if (Input::Instance.PopKeyMessage(_temp))
	{
		if (_temp.isDown)
		{
			m_CommandList.push_back(_temp);
		}
		else
		{
			m_KeyUpList.push_back(_temp);
		}
	}
	else
	{
		return;
	}
}

void CommandQueue::DeQueue()
{
	if (m_CommandList.empty())
	{
		return;
	}
	// 비어 있지 않으면 키를 누르고 있는지 확인한 뒤
		// 누르고 있으면 큐를 유지한다. 누르고 있지 않으면 pop
	else
	{
		if (IsUp(m_CommandList.front()))
		{
			m_CommandList.pop_front();
		}
		else
		{
			return;
		}
	}
}

/**
 * \brief
 *	현재 커맨드 큐와 커맨드 키 배열을 비교해준다.
 * \return
 *	실패시 -1, 성공시 성공한 기술의 인덱스 값을 리턴해준다.
 */
int CommandQueue::Decode()
{
	// 인자를 확인하기 위한 이터레이터 변수 선언, 결과를 저장할 result변수 선언
	std::list<KeyMessageInfo>::iterator _iter;
	int _result = -1;
	
	for (_iter = m_CommandList.begin(); _iter != m_CommandList.end(); _iter++)
	{
		for (int i = 0; i < m_CommandCount; i++)
		{
			// 배열과 리스트를 비교한다. 결과가 있으면 리스트들을 비워준다.
			_result = DecodeList(_iter, i);

			if (_result != -1)
			{
				m_CommandList.clear();
				m_KeyUpList.clear();
				return _result;
			}
		}
	}

	return -1;
}

/// <summary>
/// 들어오는 이터레이터와 idx에 해당하는 키 값을 비교
/// </summary>
int CommandQueue::DecodeList(std::list<KeyMessageInfo>::iterator _iter, int _arrIdx)
{
	// 각 인자의 비교 결과를 저장할 bool 변수 선언
	bool _matchFirstKey = false;
	bool _matchSecondKey = false;
	bool _matchThirdKey = false;
	bool _matchFourthKey = false;

	// 첫 인자 비교
	if ((*_iter).vKey == m_KeyCommandArr[_arrIdx][0])
	{
		_iter++;
		_matchFirstKey = true;
	}
	else
	{
		return -1;
	}

	// 두 번째 인자 비교
	if (_iter != m_CommandList.end() && _matchFirstKey && (*_iter).vKey == m_KeyCommandArr[_arrIdx][1])
	{
		_iter++;
		_matchSecondKey = true;
	}
	else
	{
		return -1;
	}

	// 세 번째 인자 비교 => 두 개짜리 있을수도 있다.
		// iter가 end가 아니고, second를 맞췄고, 배열의 세번째가 3번째 리스트와 맞거나
		// 배열의 세 번째가 none인 경우 => 배열의 세 번째가 none이면 리스트가 마지막이어도 됨
	if ((_iter != m_CommandList.end() && _matchSecondKey &&	((*_iter).vKey == m_KeyCommandArr[_arrIdx][2]))
	|| (m_KeyCommandArr[_arrIdx][2] == SanyoniEngine::EVirtualKey::None))
	{
		if(_iter != m_CommandList.end())
		{
			_iter++;
		}
		_matchThirdKey = true;
	}
	else
	{
		return -1;
	}
	
	if ((_iter != m_CommandList.end() && _matchThirdKey && ((*_iter).vKey == m_KeyCommandArr[_arrIdx][3]))
	|| (m_KeyCommandArr[_arrIdx][3] == SanyoniEngine::EVirtualKey::None))
	{
		_matchFourthKey = true;
	}
	else
	{
		return -1;
	}

	// 네 개의 모든 키가 true일 때.
	if (_matchFirstKey && _matchSecondKey && _matchThirdKey && _matchFourthKey)
	{
		return _arrIdx;
	}
	else
	{
		return -1;
	}
}

/// <summary>
/// up이 있으면 true, 없으면 false를 반환
/// </summary>
bool CommandQueue::IsUp(KeyMessageInfo _key)
{
	// for를 돌면서 해당 키의 up이 있는지를 찾고, 있으면 해당 값을 리스트에서 제거
	
	if (m_KeyUpList.empty())
	{
		return false;
	}
	else
	{
		// 해당 위치의 인자를 제거하기 위한 이터레이터 변수 선언
		std::list<KeyMessageInfo>::iterator _iter;

		for (_iter = m_KeyUpList.begin(); _iter != m_KeyUpList.end(); _iter++)
		{
			// 만약 키가 같은게 있다? => up이 있다?
			if (_key.vKey == (*_iter).vKey)
			{
				m_KeyUpList.erase(_iter);

				return true;
			}
		}

		// for문을 다 돌아도 없으면 false를 반환
		return false;
	}
}
