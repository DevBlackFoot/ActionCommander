#pragma once


// CMonsterPage 대화 상자

class CMonsterPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMonsterPage)

public:
	CMonsterPage();
	virtual ~CMonsterPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMonsterPage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStage1monster();
	afx_msg void OnBnClickedStage2monster0();
	afx_msg void OnBnClickedStage2monster1();
	CButton m_Stage1Monster;
	CButton m_Stage2Monster0;
	CButton m_Stage2Monster1;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedObjecteraser1();
	afx_msg void OnBnClickedStage3boss();
	CButton m_Stage3Boss;
};
