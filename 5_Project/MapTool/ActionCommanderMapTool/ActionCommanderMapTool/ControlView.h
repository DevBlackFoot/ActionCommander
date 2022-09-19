#pragma once



// CControlView 폼 보기
class CControlProperty;
class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CControlView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CControlView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CControlProperty*	m_pControlProperty;

public:

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


