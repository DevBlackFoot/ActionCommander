// ForeGroundPage.cpp: 구현 파일
//

#include "pch.h"
#include "ActionCommanderMapTool.h"
#include "ForeGroundPage.h"
#include "afxdialogex.h"
#include "ResourceManager.h"


// CForeGroundPage 대화 상자

IMPLEMENT_DYNAMIC(CForeGroundPage, CPropertyPage)

CForeGroundPage::CForeGroundPage()
	: CPropertyPage(IDD_CForeGroundPage)
{

}

CForeGroundPage::~CForeGroundPage()
{
}

void CForeGroundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEAF0, m_Leaf0);
	DDX_Control(pDX, IDC_LEAF1, m_Leaf1);
	DDX_Control(pDX, IDC_LEAF2, m_Leaf2);
	DDX_Control(pDX, IDC_LEAF3, m_Leaf3);
	DDX_Control(pDX, IDC_LEAF4, m_Leaf4);
	DDX_Control(pDX, IDC_ROCK0, m_Rock0);
	DDX_Control(pDX, IDC_SIGN, m_Sign);

	DDX_Control(pDX, IDC_TREE0, m_Tree0);
	DDX_Control(pDX, IDC_TREE1, m_Tree1);
	DDX_Control(pDX, IDC_TREE2, m_Tree2);
	DDX_Control(pDX, IDC_ROCK1, m_Rock1);
	DDX_Control(pDX, IDC_SIGN1, m_Sign1);
	DDX_Control(pDX, IDC_SIGN2, m_Sign2);
	DDX_Control(pDX, IDC_SIGN3, m_Sign3);
}


BEGIN_MESSAGE_MAP(CForeGroundPage, CPropertyPage)
	ON_BN_CLICKED(IDC_LEAF0, &CForeGroundPage::OnBnClickedLeaf0)
	ON_BN_CLICKED(IDC_LEAF1, &CForeGroundPage::OnBnClickedLeaf1)
	ON_BN_CLICKED(IDC_LEAF2, &CForeGroundPage::OnBnClickedLeaf2)
	ON_BN_CLICKED(IDC_LEAF3, &CForeGroundPage::OnBnClickedLeaf3)
	ON_BN_CLICKED(IDC_LEAF4, &CForeGroundPage::OnBnClickedLeaf4)
	ON_BN_CLICKED(IDC_ROCK0, &CForeGroundPage::OnBnClickedRock0)
	ON_BN_CLICKED(IDC_SIGN, &CForeGroundPage::OnBnClickedSign)
	ON_BN_CLICKED(IDC_TREE0, &CForeGroundPage::OnBnClickedTree0)
	ON_BN_CLICKED(IDC_TREE1, &CForeGroundPage::OnBnClickedTree1)
	ON_BN_CLICKED(IDC_TREE2, &CForeGroundPage::OnBnClickedTree2)
	ON_BN_CLICKED(IDC_OBJECTERASER0, &CForeGroundPage::OnBnClickedObjecteraser0)
	ON_BN_CLICKED(IDC_ROCK1, &CForeGroundPage::OnBnClickedRock1)
	ON_BN_CLICKED(IDC_SIGN1, &CForeGroundPage::OnBnClickedSign1)
	ON_BN_CLICKED(IDC_SIGN2, &CForeGroundPage::OnBnClickedSign2)
	ON_BN_CLICKED(IDC_SIGN3, &CForeGroundPage::OnBnClickedSign3)
END_MESSAGE_MAP()


// CForeGroundPage 메시지 처리기


void CForeGroundPage::OnBnClickedLeaf0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf0;
}


void CForeGroundPage::OnBnClickedLeaf1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf1;
}


void CForeGroundPage::OnBnClickedLeaf2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf2;
}


void CForeGroundPage::OnBnClickedLeaf3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf3;
}


void CForeGroundPage::OnBnClickedLeaf4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectLeaf4;
}



void CForeGroundPage::OnBnClickedRock0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectRock0;
}


void CForeGroundPage::OnBnClickedRock1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectRock1;
}

void CForeGroundPage::OnBnClickedSign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectSign0;
}


void CForeGroundPage::OnBnClickedObjecteraser0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::None;
}

void CForeGroundPage::OnBnClickedTree0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree0;
}


void CForeGroundPage::OnBnClickedTree1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree1;
}

void CForeGroundPage::OnBnClickedTree2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectTree2;
}

void CForeGroundPage::OnBnClickedSign1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectSign1;
}


void CForeGroundPage::OnBnClickedSign2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectSign2;
}


void CForeGroundPage::OnBnClickedSign3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	theApp.isTileCursor = false;
	theApp.m_ObjCursorState = ObjectAttribute::ObjectSign3;
}


BOOL CForeGroundPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Leaf0.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf0));
	m_Leaf1.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf1));
	m_Leaf2.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf2));
	m_Leaf3.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf3));
	m_Leaf4.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectLeaf4));

	m_Rock0.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock0));
	m_Rock1.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectRock1));

	m_Sign.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign0));
	m_Sign1.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign1));
	m_Sign2.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign2));
	m_Sign3.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectSign3));

	m_Tree0.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree0));
	m_Tree1.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree1));
	m_Tree2.SetBitmap(*CResourceManager::GetInstance()->GetObjectImage(ObjectAttribute::ObjectTree2));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
