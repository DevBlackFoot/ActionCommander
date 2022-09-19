#pragma once

#include <unordered_map>

/**
 * \brief 싱글톤으로 모든 리소스를 가지고 있는 매니저
 *
 * 각종 타일, 배경, 몬스터 등 맵 툴에 필요한 비트맵 이미지를 앱 시작때 생성하는 역할
 *
 * 초기화 후 필요할 때 해당 비트맵의 정보를 전달
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
	// i번째 타일 이미지를 가져온다.
	CImage* GetTileImage(TileAttribute _attribute);
	CImage* GetBGImage(int i);
	CImage* GetObjectImage(ObjectAttribute _attribute);
	
	// 만약 새로운 파일이 추가되면.. 업데이트
	void Update();

	// 프로그램이 끝날 때 finalize를 통해 할당된 메모리 공간 해제.
	void Finalize();
	CResourceManager()
	{
		
		
	}

private:
	static CResourceManager* m_Instance;

	// 비트맵 파일 정보를 가지고 있는 맵들..
	// 타일
	unordered_map<TileAttribute, CImage*> m_TileImages;
	// 배경
	vector<CImage*> m_BGImages;
	// 오브젝트 이미지 => 전경 + 몬스터 + 기타요소
	unordered_map<ObjectAttribute, CImage*> m_ObjectImages;

};

