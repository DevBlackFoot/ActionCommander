#pragma once

#include <unordered_map>

/**
 * \brief �̱������� ��� ���ҽ��� ������ �ִ� �Ŵ���
 *
 * ���� Ÿ��, ���, ���� �� �� ���� �ʿ��� ��Ʈ�� �̹����� �� ���۶� �����ϴ� ����
 *
 * �ʱ�ȭ �� �ʿ��� �� �ش� ��Ʈ���� ������ ����
 */
using namespace std;
class CResourceManager
{
public:
	static CResourceManager* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new CResourceManager();
		}

		return m_Instance;
	}

	void Init();
	// i��° Ÿ�� �̹����� �����´�.
	CImage* GetTileImage(TileAttribute _attribute);
	CImage* GetBGImage(int i);
	CImage* GetObjectImage(ObjectAttribute _attribute);
	
	// ���� ���ο� ������ �߰��Ǹ�.. ������Ʈ
	void Update();

	// ���α׷��� ���� �� finalize�� ���� �Ҵ�� �޸� ���� ����.
	void Finalize();
	CResourceManager()
	{
		
		
	}

private:
	static CResourceManager* m_Instance;

	// ��Ʈ�� ���� ������ ������ �ִ� �ʵ�..
	// Ÿ��
	unordered_map<TileAttribute, CImage*> m_TileImages;
	// ���
	vector<CImage*> m_BGImages;
	// ������Ʈ �̹��� => ���� + ���� + ��Ÿ���
	unordered_map<ObjectAttribute, CImage*> m_ObjectImages;

};

