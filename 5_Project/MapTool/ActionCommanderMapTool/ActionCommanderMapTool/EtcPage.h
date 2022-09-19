#pragma once


// CEtcPage 대화 상자

class CEtcPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEtcPage)

public:
	CEtcPage();
	virtual ~CEtcPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CEtcPage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHeart();
	afx_msg void OnBnClickedPortal();
	afx_msg void OnBnClickedPlayer();
	CButton m_Heart;
	CButton m_Portal;
	CButton m_Player;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedObjecteraser2();
	afx_msg void OnBnClickedCleareng();
	afx_msg void OnBnClickedClearkor();
	afx_msg void OnBnClickedClearname();
	CButton m_ClearEng;
	CButton m_ClearKor;
	CButton m_ClearName;
};
