#pragma once


// MapSizeDlg 대화 상자

class MapSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MapSizeDlg)

public:
	MapSizeDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MapSizeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapSizeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 입력받은 Width와 Height정보를 저장할 변수
	int m_DlgWidth;
	int m_DlgHeight;

	CEdit m_EditWidth;
	CEdit m_EditHeight;
	CSpinButtonCtrl m_SpinWidth;
	CSpinButtonCtrl m_SpinHeight;

// 메시지 처리
public:
	
	afx_msg void OnEnChangeEditwidth();
	afx_msg void OnEnChangeEditheight();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDeltaposSpinwidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinheight(NMHDR* pNMHDR, LRESULT* pResult);
};
