// MonsterPage.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "MonsterPage.h"
#include "afxdialogex.h"
#include "ResourceManager.h"

// CMonsterPage 대화 상자

IMPLEMENT_DYNAMIC(CMonsterPage, CPropertyPage)

CMonsterPage::CMonsterPage()
	: CPropertyPage(IDD_CMonsterPage)
{

}

CMonsterPage::~CMonsterPage()
{
}

void CMonsterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STAGE1MONSTER, m_Stage1Monster);
	DDX_Control(pDX, IDC_STAGE2MONSTER0, m_Stage2Monster0);
	DDX_Control(pDX, IDC_STAGE2MONSTER1, m_Stage2Monster1);
	DDX_Control(pDX, IDC_STAGE3BOSS, m_Stage3Boss);
}


BEGIN_MESSAGE_MAP(CMonsterPage, CPropertyPage)
	ON_BN_CLICKED(IDC_STAGE1MONSTER, &CMonsterPage::OnBnClickedStage1monster)
	ON_BN_CLICKED(IDC_STAGE2MONSTER0, &CMonsterPage::OnBnClickedStage2monster0)
	ON_BN_CLICKED(IDC_STAGE2MONSTER1, &CMonsterPage::OnBnClickedStage2monster1)
	ON_BN_CLICKED(IDC_OBJECTERASER1, &CMonsterPage::OnBnClickedObjecteraser1)
	ON_BN_CLICKED(IDC_STAGE3BOSS, &CMonsterPage::OnBnClickedStage3boss)
END_MESSAGE_MAP()


// CMonsterPage 메시지 처리기


void CMonsterPage::OnBnClickedStage1monster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Stage1Monster;
}


void CMonsterPage::OnBnClickedStage2monster0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Stage2Monster0;
}


void CMonsterPage::OnBnClickedStage2monster1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Stage2Monster1;
}

void CMonsterPage::OnBnClickedObjecteraser1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::None;
}

void CMonsterPage::OnBnClickedStage3boss()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::Stage3Boss;
}

BOOL CMonsterPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 비트맵 추가
	m_Stage1Monster.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage1Monster));
	m_Stage2Monster0.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage2Monster0));
	m_Stage2Monster1.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage2Monster1));
	m_Stage3Boss.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::Stage3Boss));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



