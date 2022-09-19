#pragma once


// CMiniMapView 보기
class CActionCommanderDoc;
class CMiniMapView : public CView
{
	DECLARE_DYNCREATE(CMiniMapView)

protected:
	CMiniMapView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMiniMapView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	// D2D를 위한 멤버 변수
	CD2DTextFormat* m_pTextFormat;
	CD2DSolidColorBrush* m_pBlackPen;
	CDCRenderTarget* m_pDCRenderTarget;

	void DrawTextJH(int x, int y, const char* pch, ...);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	afx_msg LRESULT OnDraw2d(WPARAM wParam, LPARAM lParam);
};


