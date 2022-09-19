#include "pch.h"

#include "CommandQueue.h"
#include "Input.h"
#include "GameTime.h"

CommandQueue::CommandQueue(int _dequeCycle)
{
	// ��� ���� �ʱ�ȭ.
	m_DeQueueCycle = _dequeCycle;

	m_LastDequeTimeMillis = SanyoniEngine::GameTime::Instance.GetGameTimeMillis();

	m_LastSuccessCommand = -1;

	// Ŀ�ǵ� ����
	m_CommandCount = 7;

	// keyCommand �迭 �ʱ�ȭ
	// ù Ŀ�ǵ� -> -> z => 0
	m_KeyCommandArr[0][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[0][1] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[0][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[0][3] = SanyoniEngine::EVirtualKey::None;

	// �� ��° Ŀ�ǵ� <- <- z => 1
	m_KeyCommandArr[1][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[1][1] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[1][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[1][3] = SanyoniEngine::EVirtualKey::None;

	// ĳ�� ����̴� <- �Ʒ� -> z 2
	m_KeyCommandArr[2][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[2][1] = SanyoniEngine::EVirtualKey::DownArrow;
	m_KeyCommandArr[2][2] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[2][3] = SanyoniEngine::EVirtualKey::Key_Z;

	// ĳ�� ����̴� 3
	m_KeyCommandArr[3][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[3][1] = SanyoniEngine::EVirtualKey::DownArrow;
	m_KeyCommandArr[3][2] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[3][3] = SanyoniEngine::EVirtualKey::Key_Z;

	// ����¡ �ҵ� 4, 5
	m_KeyCommandArr[4][0] = SanyoniEngine::EVirtualKey::LeftArrow;
	m_KeyCommandArr[4][1] = SanyoniEngine::EVirtualKey::UpArrow;
	m_KeyCommandArr[4][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[4][3] = SanyoniEngine::EVirtualKey::None;

	// ����¡ �ҵ� 4, 5
	m_KeyCommandArr[5][0] = SanyoniEngine::EVirtualKey::RightArrow;
	m_KeyCommandArr[5][1] = SanyoniEngine::EVirtualKey::UpArrow;
	m_KeyCommandArr[5][2] = SanyoniEngine::EVirtualKey::Key_Z;
	m_KeyCommandArr[5][3] = SanyoniEngine::EVirtualKey::None;

	// ������ ű 6
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
	// Ư�� ī��Ʈ ���� dequeue �� Ŀ�ǵ� ���� ���� ���з�
	if (SanyoniEngine::GameTime::Instance.GetGameTimeMillis() - m_LastDequeTimeMillis > m_DeQueueCycle)
	{
		m_LastDequeTimeMillis = SanyoniEngine::GameTime::Instance.GetGameTimeMillis();

		DeQueue();
	}
	EnQueue();

	// �޼��� ����Ʈ ������ �ڵ�
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

	// Decode�� ����� -1�� �ƴϸ�(���а� �ƴϸ�) Success command Queue�� �־��ش�.
	m_LastSuccessCommand = Decode();

	// ����׿� �޽��� �Լ�
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
/// �޽����� Ȯ���ؼ� up�̸� keyuplist��
/// down�̸� commandlist�� �־��ش�. 
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
	// ��� ���� ������ Ű�� ������ �ִ��� Ȯ���� ��
		// ������ ������ ť�� �����Ѵ�. ������ ���� ������ pop
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
 *	���� Ŀ�ǵ� ť�� Ŀ�ǵ� Ű �迭�� �����ش�.
 * \return
 *	���н� -1, ������ ������ ����� �ε��� ���� �������ش�.
 */
int CommandQueue::Decode()
{
	// ���ڸ� Ȯ���ϱ� ���� ���ͷ����� ���� ����, ����� ������ result���� ����
	std::list<KeyMessageInfo>::iterator _iter;
	int _result = -1;
	
	for (_iter = m_CommandList.begin(); _iter != m_CommandList.end(); _iter++)
	{
		for (int i = 0; i < m_CommandCount; i++)
		{
			// �迭�� ����Ʈ�� ���Ѵ�. ����� ������ ����Ʈ���� ����ش�.
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
/// ������ ���ͷ����Ϳ� idx�� �ش��ϴ� Ű ���� ��
/// </summary>
int CommandQueue::DecodeList(std::list<KeyMessageInfo>::iterator _iter, int _arrIdx)
{
	// �� ������ �� ����� ������ bool ���� ����
	bool _matchFirstKey = false;
	bool _matchSecondKey = false;
	bool _matchThirdKey = false;
	bool _matchFourthKey = false;

	// ù ���� ��
	if ((*_iter).vKey == m_KeyCommandArr[_arrIdx][0])
	{
		_iter++;
		_matchFirstKey = true;
	}
	else
	{
		return -1;
	}

	// �� ��° ���� ��
	if (_iter != m_CommandList.end() && _matchFirstKey && (*_iter).vKey == m_KeyCommandArr[_arrIdx][1])
	{
		_iter++;
		_matchSecondKey = true;
	}
	else
	{
		return -1;
	}

	// �� ��° ���� �� => �� ��¥�� �������� �ִ�.
		// iter�� end�� �ƴϰ�, second�� �����, �迭�� ����°�� 3��° ����Ʈ�� �°ų�
		// �迭�� �� ��°�� none�� ��� => �迭�� �� ��°�� none�̸� ����Ʈ�� �������̾ ��
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

	// �� ���� ��� Ű�� true�� ��.
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
/// up�� ������ true, ������ false�� ��ȯ
/// </summary>
bool CommandQueue::IsUp(KeyMessageInfo _key)
{
	// for�� ���鼭 �ش� Ű�� up�� �ִ����� ã��, ������ �ش� ���� ����Ʈ���� ����
	
	if (m_KeyUpList.empty())
	{
		return false;
	}
	else
	{
		// �ش� ��ġ�� ���ڸ� �����ϱ� ���� ���ͷ����� ���� ����
		std::list<KeyMessageInfo>::iterator _iter;

		for (_iter = m_KeyUpList.begin(); _iter != m_KeyUpList.end(); _iter++)
		{
			// ���� Ű�� ������ �ִ�? => up�� �ִ�?
			if (_key.vKey == (*_iter).vKey)
			{
				m_KeyUpList.erase(_iter);

				return true;
			}
		}

		// for���� �� ���Ƶ� ������ false�� ��ȯ
		return false;
	}
}
