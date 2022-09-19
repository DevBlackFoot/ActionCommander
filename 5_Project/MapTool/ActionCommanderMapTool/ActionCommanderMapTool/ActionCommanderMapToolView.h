
// ActionCommanderMapToolView.h: CActionCommanderMapToolView 클래스의 인터페이스
//

#pragma once
#include <unordered_map>

class CActionCommanderMapToolDoc;
class CActionCommanderMapToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CActionCommanderMapToolView() noexcept;
	DECLARE_DYNCREATE(CActionCommanderMapToolView)

// 특성입니다.
public:
	CActionCommanderMapToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CActionCommanderMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 내가 사용할 멤버 변수
protected:
// 커서 그리기 함수
	void DrawCursor();

// DOC의 정보를 받아서 타일을 그리는 함수
	void DrawTileMap();
	void DrawCollider();
	void DrawObject();

// 리소스 init
	void InitResource();
	bool m_loadBitmap = false;

// 마우스 커서 변수
	CPoint m_MousePos = {-64, -64};

// 스크롤바 위치에 대한 변수
	CPoint m_ScrollPos = {0, 0};

// 렌더 타겟
	CDCRenderTarget* m_pDCRenderTarget;
	CD2DSolidColorBrush* m_pGridPen;
	CD2DSolidColorBrush* m_pRedPen;
	CD2DSolidColorBrush* m_pBluePen;
	CD2DSolidColorBrush* m_pColliderPen;

// 기본 비트맵 정보
	// CImage를 렌더 타겟에 맞는 CD2D이미지로 변환해서 가지고 있는 변수
	CD2DBitmap* m_BG;
	std::unordered_map<TileAttribute, CD2DBitmap*> m_TileBitmap;
	std::unordered_map<ObjectAttribute, CD2DBitmap*> m_ObjectBitmap;


// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	// D2D Draw를 위한 메시지 처리기
	afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ActionCommanderMapToolView.cpp의 디버그 버전
inline CActionCommanderMapToolDoc* CActionCommanderMapToolView::GetDocument() const
   { return reinterpret_cast<CActionCommanderMapToolDoc*>(m_pDocument); }
#endif

