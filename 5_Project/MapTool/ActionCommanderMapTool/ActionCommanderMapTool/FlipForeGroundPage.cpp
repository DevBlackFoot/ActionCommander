// FlipForeGroundPage.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "FlipForeGroundPage.h"
#include "afxdialogex.h"
#include "ResourceManager.h"

// CFlipForeGroundPage 대화 상자

IMPLEMENT_DYNAMIC(CFlipForeGroundPage, CPropertyPage)

CFlipForeGroundPage::CFlipForeGroundPage()
	: CPropertyPage(IDD_CFlipForeGroundPage)
{

}

CFlipForeGroundPage::~CFlipForeGroundPage()
{
}

void CFlipForeGroundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEAF0FLIP, m_Leaf0Flip);
	DDX_Control(pDX, IDC_LEAF1FLIP, m_Leaf1Flip);
	DDX_Control(pDX, IDC_LEAF2FLIP, m_Leaf2Flip);
	DDX_Control(pDX, IDC_LEAF3FLIP, m_Leaf3Flip);
	DDX_Control(pDX, IDC_LEAF4FLIP, m_Leaf4Flip);
	DDX_Control(pDX, IDC_ROCK0FLIP, m_Rock0Flip);
	DDX_Control(pDX, IDC_TREE0FLIP, m_Tree0Flip);
	DDX_Control(pDX, IDC_TREE1FLIP, m_Tree1Flip);
	DDX_Control(pDX, IDC_TREE2FLIP, m_Tree2Flip);
	DDX_Control(pDX, IDC_ROCK1FLIP, m_Rock1Flip);
}


BEGIN_MESSAGE_MAP(CFlipForeGroundPage, CPropertyPage)
	ON_BN_CLICKED(IDC_LEAF0FLIP, &CFlipForeGroundPage::OnBnClickedLeaf0flip)
	ON_BN_CLICKED(IDC_LEAF1FLIP, &CFlipForeGroundPage::OnBnClickedLeaf1flip)
	ON_BN_CLICKED(IDC_LEAF2FLIP, &CFlipForeGroundPage::OnBnClickedLeaf2flip)
	ON_BN_CLICKED(IDC_LEAF3FLIP, &CFlipForeGroundPage::OnBnClickedLeaf3flip)
	ON_BN_CLICKED(IDC_LEAF4FLIP, &CFlipForeGroundPage::OnBnClickedLeaf4flip)
	ON_BN_CLICKED(IDC_ROCK0FLIP, &CFlipForeGroundPage::OnBnClickedRock0flip)
	ON_BN_CLICKED(IDC_TREE0FLIP, &CFlipForeGroundPage::OnBnClickedTree0flip)
	ON_BN_CLICKED(IDC_TREE1FLIP, &CFlipForeGroundPage::OnBnClickedTree1flip)
	ON_BN_CLICKED(IDC_TREE2FLIP, &CFlipForeGroundPage::OnBnClickedTree2flip)

	ON_BN_CLICKED(IDC_OBJECTERASER3, &CFlipForeGroundPage::OnBnClickedObjecteraser3)
	ON_BN_CLICKED(IDC_ROCK1FLIP, &CFlipForeGroundPage::OnBnClickedRock1flip)
END_MESSAGE_MAP()


// CFlipForeGroundPage 메시지 처리기


void CFlipForeGroundPage::OnBnClickedLeaf0flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf0Flip;
}


void CFlipForeGroundPage::OnBnClickedLeaf1flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf1Flip;
}


void CFlipForeGroundPage::OnBnClickedLeaf2flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf2Flip;
}


void CFlipForeGroundPage::OnBnClickedLeaf3flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf3Flip;
}


void CFlipForeGroundPage::OnBnClickedLeaf4flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf4Flip;
}


void CFlipForeGroundPage::OnBnClickedRock0flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectRock0Flip;
}

void CFlipForeGroundPage::OnBnClickedRock1flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectRock1Flip;
}


void CFlipForeGroundPage::OnBnClickedTree0flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree0Flip;
}


void CFlipForeGroundPage::OnBnClickedTree1flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree1Flip;
}


void CFlipForeGroundPage::OnBnClickedTree2flip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree2Flip;
}


void CFlipForeGroundPage::OnBnClickedObjecteraser3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::None;
}


BOOL CFlipForeGroundPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Leaf0Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf0Flip));
	m_Leaf1Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf1Flip));
	m_Leaf2Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf2Flip));
	m_Leaf3Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf3Flip));
	m_Leaf4Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf4Flip));

	m_Rock0Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock0Flip));
	m_Rock1Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock1Flip));

	m_Tree0Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree0Flip));
	m_Tree1Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree1Flip));
	m_Tree2Flip.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree2Flip));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





