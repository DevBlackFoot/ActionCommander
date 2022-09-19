#pragma once
#include <queue>
#include "KeyMessageInfo.h"

using namespace SanyoniEngine;

/// <summary>
/// Ŀ�ǵ� �Է��� �޾� ���� �� �ؼ��� ���ִ� ť Ŭ�����Դϴ�.
/// 
/// ���������� up������ ��� �ִ¸���Ʈ(Press ������ ��� ����)�� 
/// Ŀ�ǵ��ؼ��� �ʿ��� ����Ʈ
/// ������ Ŀ�ǵ带 ������� �����ϴ� ť�� ��� �ֽ��ϴ�.
/// 
/// 2022. 02. 07 Dev.Blackfoot
/// </summary>
class CommandQueue
{
public:
	CommandQueue(int _dequeCycle);
	~CommandQueue();

	// �� ������Ʈ ���� �۵��� �Լ�
		// �⺻������
		// 1. ���� �ð����� DeQueue�� Front�� ���� �������ְ�
		// 2. �� ������Ʈ���� EnQueue�� ���ο� Ű �Է��� ť�� �־��ָ�
		// 3. ���������� ť�� �迭�� ���ؼ� �ؼ��մϴ�.
	void Update();
	int GetSuccessCommand();

private:
	// Ư�� ���ǿ� ���� ť�� ���� �־��ִ� �Լ�
		// key Down�� ��.
	void EnQueue();

	// ���� �ð����� front�� ���� �����ִ� �Լ�
		// Ư�� ����(Ű�� ��� ������ ���� ��)���� �ٷ� ������ ���ش�
	void DeQueue();

	// Ŀ�ǵ带 �ؼ��ϴ� �Լ�
		// � ������ �ؼ������� ������ �ʿ�
		// �� ��쿡 int�� ��ȯ..
	int Decode();
	// �� Ŀ�ǵ��� �ε���, list�� iterator�� �־ ť�� �ؼ��ϴ� �Լ�
	int DecodeList(std::list<KeyMessageInfo>::iterator _iter, int _arrIdx);

	// up�� Ȯ���ϴ� �Լ�
	bool IsUp(KeyMessageInfo _key);

	// Ű�� �޽����� �����صδ� list
	std::list<KeyMessageInfo> m_KeyUpList;
	// Ű �ٿ� �޽����� ������ Ŀ�ǵ� ����Ʈ
		// decode�� ���� ť���� ����Ʈ�� ����..
	std::list<KeyMessageInfo> m_CommandList;
	// Inqueue / Dequeue�� �ش� ����Ʈ���� Ȱ�����ش�.
		// Ű �ٿ��� CommandList��, Ű ���� KeyUpList��..

	// Ŀ�ǵ带 �����ص� 2���� �迭
		// 4��¥�� Ŀ�ǵ� 7���� �����ϴ� �迭.
	EVirtualKey m_KeyCommandArr[7][4];
	// Ŀ�ǵ� ������ �����ϴ� ����
	int m_CommandCount;

	// Dequeue �ð��� �����ִ� �ֱ� ����
	int m_DeQueueCycle;
	// �������� � Ŀ�ǵ尡 �����ߴ����� �����صδ� int ��
	int m_LastSuccessCommand;

	long long m_LastDequeTimeMillis;

	// ����׿� ���� ����
	//int m_DebugCount;
};

