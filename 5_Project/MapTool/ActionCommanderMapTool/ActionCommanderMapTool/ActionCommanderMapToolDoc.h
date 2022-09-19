
// ActionCommanderMapToolDoc.h: CActionCommanderMapToolDoc 클래스의 인터페이스
//


#pragma once
#include <vector>
#include <list>

class CActionCommanderMapToolDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CActionCommanderMapToolDoc() noexcept;
	DECLARE_DYNCREATE(CActionCommanderMapToolDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CActionCommanderMapToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 내가 사용할 각종 public 멤버 변수
public:
	// 타일맵 벡터
		// 타일맵에 대한 모든 정보를 저장.. => 2차원 배열 처럼 사용
		// 해당 그리드의 타일 정보를 나타냄
		// 구조체 포인터로 기본적으로 nullptr을 넣어줌
	std::vector<Tile*> m_TileMap;

	// 오브젝트 리스트
		// 오브젝트에 대한 모든 정보를 저장
		// 기본적으로 내부에 저장되어 있는 Object에 접근, 센터 데이터를 얻어와서. 그곳을 중심으로 그림을 그린다.
	std::list<Object*> m_ObjectMap;

	// 콜라이더 벡터.
		// 콜라이더의 left top right bottom 정보를 저장하는 벡터
		// 해당 위치에 그 크기의 콜라이더를 생성해준다.
	std::list<CRect> m_Colliders;

	void InitJson(const rapidjson::GenericValue<rapidjson::UTF8<>>& Json);
	void InitJson(const rapidjson::GenericValue<rapidjson::UTF16<>>& Json);
	void SetCollider();


	// 오브젝트 카운터
		// 생성한 오브젝트의 개수를 센다..
	int m_ObjectCount;
	int m_MonsterCount;
	int m_PlayerCount;
	int m_PortalCount;
	int m_BossCount;

	// 클리어 들의 개수를 셉니다.
	bool m_bClearEng;
	bool m_bClearKor;
	bool m_bClearName;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
