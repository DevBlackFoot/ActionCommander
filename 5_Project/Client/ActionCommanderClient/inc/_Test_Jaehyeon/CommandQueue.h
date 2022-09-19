#pragma once
#include <queue>
#include "KeyMessageInfo.h"

using namespace SanyoniEngine;

/// <summary>
/// 커맨드 입력을 받아 저장 및 해석을 해주는 큐 클래스입니다.
/// 
/// 내부적으로 up정보를 담고 있는리스트(Press 정보를 얻기 위함)와 
/// 커맨드해석에 필요한 리스트
/// 성공한 커맨드를 순서대로 저장하는 큐를 담고 있습니다.
/// 
/// 2022. 02. 07 Dev.Blackfoot
/// </summary>
class CommandQueue
{
public:
	CommandQueue(int _dequeCycle);
	~CommandQueue();

	// 매 업데이트 마다 작동될 함수
		// 기본적으로
		// 1. 일정 시간마다 DeQueue로 Front의 값을 제거해주고
		// 2. 매 업데이트마다 EnQueue로 새로운 키 입력을 큐에 넣어주며
		// 3. 마지막으로 큐와 배열을 비교해서 해석합니다.
	void Update();
	int GetSuccessCommand();

private:
	// 특정 조건에 맞춰 큐에 값을 넣어주는 함수
		// key Down일 때.
	void EnQueue();

	// 일정 시간마다 front의 값을 없애주는 함수
		// 특정 조건(키를 계속 누르고 있을 떄)에는 바로 리턴을 해준다
	void DeQueue();

	// 커맨드를 해석하는 함수
		// 어떤 식으로 해석할지는 연구가 필요
		// 각 경우에 int를 반환..
	int Decode();
	// 각 커맨드의 인덱스, list의 iterator를 넣어서 큐를 해석하는 함수
	int DecodeList(std::list<KeyMessageInfo>::iterator _iter, int _arrIdx);

	// up을 확인하는 함수
	bool IsUp(KeyMessageInfo _key);

	// 키업 메시지만 저장해두는 list
	std::list<KeyMessageInfo> m_KeyUpList;
	// 키 다운 메시지만 저장할 커맨드 리스트
		// decode를 위해 큐에서 리스트로 변경..
	std::list<KeyMessageInfo> m_CommandList;
	// Inqueue / Dequeue시 해당 리스트들을 활용해준다.
		// 키 다운은 CommandList로, 키 업은 KeyUpList로..

	// 커맨드를 저장해둘 2차원 배열
		// 4개짜리 커맨드 7개를 저장하는 배열.
	EVirtualKey m_KeyCommandArr[7][4];
	// 커맨드 개수를 저장하는 변수
	int m_CommandCount;

	// Dequeue 시간을 정해주는 주기 변수
	int m_DeQueueCycle;
	// 마지막에 어떤 커맨드가 성공했는지를 저장해두는 int 값
	int m_LastSuccessCommand;

	long long m_LastDequeTimeMillis;

	// 디버그용 변수 생성
	//int m_DebugCount;
};

