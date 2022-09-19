#pragma once


// CForeGroundPage 대화 상자

class CForeGroundPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CForeGroundPage)

public:
	CForeGroundPage();
	virtual ~CForeGroundPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CForeGroundPage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLeaf0();
	afx_msg void OnBnClickedLeaf1();
	afx_msg void OnBnClickedLeaf2();
	afx_msg void OnBnClickedLeaf3();
	afx_msg void OnBnClickedLeaf4();
	afx_msg void OnBnClickedRock0();
	afx_msg void OnBnClickedRock1();
	afx_msg void OnBnClickedSign();
	afx_msg void OnBnClickedTree0();
	afx_msg void OnBnClickedTree1();
	afx_msg void OnBnClickedTree2();
	afx_msg void OnBnClickedObjecteraser0();
	virtual BOOL OnInitDialog();

	CButton m_Leaf0;
	CButton m_Leaf1;
	CButton m_Leaf2;
	CButton m_Leaf3;
	CButton m_Leaf4;
	CButton m_Rock1;
	CButton m_Rock0;
	CButton m_Sign;
	CButton m_Tree0;
	CButton m_Tree1;
	CButton m_Tree2;
	afx_msg void OnBnClickedSign1();
	afx_msg void OnBnClickedSign2();
	afx_msg void OnBnClickedSign3();
	CButton m_Sign1;
	CButton m_Sign2;
	CButton m_Sign3;
};
