#include "pch.h"
#include "ResourceManager.h"

CResourceManager* CResourceManager::m_Instance = nullptr;

void CResourceManager::Init()
{
	// 이미지의 장수에 맞춰 이미지 로드 및 pushback
	// 각각의 CImage 파일을 만들어줘야함.
		// 주소 값을 가지고 있기 때문에 주소가 사라지면 이미지 정보도 사라짐.

#pragma region BG
	CImage* BG1 = new CImage;
	if(SUCCEEDED(BG1->Load("./Image/BG/background.png")))
	{
		m_BGImages.push_back(BG1);
	}

	CImage* BG2 = new CImage;
	if(SUCCEEDED(BG2->Load("./Image/BG/background2.png")))
	{
		m_BGImages.push_back(BG2);
	}
#pragma endregion

#pragma region Tile

	CImage* _groundLeftTop = new CImage;
	if (SUCCEEDED(_groundLeftTop->Load("./Image/Tiles/top_L.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundLeftTop , _groundLeftTop });
	}
	CImage* _groundTop = new CImage;
	if(SUCCEEDED(_groundTop->Load("./Image/Tiles/top.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundTop , _groundTop });
	}
	CImage* _groundRightTop = new CImage;
	if (SUCCEEDED(_groundRightTop->Load("./Image/Tiles/top_R.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundRightTop , _groundRightTop });
	}
	CImage* _groundLeft = new CImage;
	if (SUCCEEDED(_groundLeft->Load("./Image/Tiles/middle_L.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundLeft , _groundLeft });
	}
	CImage* _groundCenter = new CImage;
	if (SUCCEEDED(_groundCenter->Load("./Image/Tiles/middle.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundCenter , _groundCenter });
	}
	CImage* _groundRight = new CImage;
	if (SUCCEEDED(_groundRight->Load("./Image/Tiles/middle_R.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundRight , _groundRight });
	}
	CImage* _groundLeftBottom = new CImage;
	if (SUCCEEDED(_groundLeftBottom->Load("./Image/Tiles/bottom_L.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundLeftBottom , _groundLeftBottom });
	}
	CImage* _groundBottom = new CImage;
	if (SUCCEEDED(_groundBottom->Load("./Image/Tiles/bottom.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundBottom , _groundBottom });
	}
	CImage* _groundRightBottom = new CImage;
	if (SUCCEEDED(_groundRightBottom->Load("./Image/Tiles/bottom_R.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundRightBottom , _groundRightBottom });
	}
	CImage* _groundDiaLeftTop = new CImage;
	if (SUCCEEDED(_groundDiaLeftTop->Load("./Image/Tiles/diaLT.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundDiaLeftTop , _groundDiaLeftTop });
	}
	CImage* _groundDiaLeftBottom = new CImage;
	if (SUCCEEDED(_groundDiaLeftBottom->Load("./Image/Tiles/diaLB.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundDiaLeftBottom , _groundDiaLeftBottom });
	}
	CImage* _groundDiaRightTop = new CImage;
	if (SUCCEEDED(_groundDiaRightTop->Load("./Image/Tiles/diaRT.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundDiaRightTop , _groundDiaRightTop });
	}
	CImage* _groundDiaRightBottom = new CImage;
	if (SUCCEEDED(_groundDiaRightBottom->Load("./Image/Tiles/diaRB.png")))
	{
		m_TileImages.insert({ TileAttribute::GroundDiaRightBottom , _groundDiaRightBottom });
	}

	CImage* _notJumpPlatLeft = new CImage;
	if (SUCCEEDED(_notJumpPlatLeft->Load("./Image/Tiles/H_l.png")))
	{
		m_TileImages.insert({ TileAttribute::NotJumpPlatLeft , _notJumpPlatLeft });
	}
	CImage* _notJumpPlatCenter = new CImage;
	if (SUCCEEDED(_notJumpPlatCenter->Load("./Image/Tiles/H_m.png")))
	{
		m_TileImages.insert({ TileAttribute::NotJumpPlatCenter , _notJumpPlatCenter });
	}
	CImage* _notJumpPlatRight = new CImage;
	if (SUCCEEDED(_notJumpPlatRight->Load("./Image/Tiles/H_r.png")))
	{
		m_TileImages.insert({ TileAttribute::NotJumpPlatRight , _notJumpPlatRight });
	}

	CImage* _jumpPlatLeft = new CImage;
	if (SUCCEEDED(_jumpPlatLeft->Load("./Image/Tiles/L_l.png")))
	{
		m_TileImages.insert({ TileAttribute::JumpPlatLeft , _jumpPlatLeft });
	}
	CImage* _jumpPlatCenter = new CImage;
	if (SUCCEEDED(_jumpPlatCenter->Load("./Image/Tiles/L_m.png")))
	{
		m_TileImages.insert({ TileAttribute::JumpPlatCenter , _jumpPlatCenter });
	}
	CImage* _jumpPlatRight = new CImage;
	if (SUCCEEDED(_jumpPlatRight->Load("./Image/Tiles/L_r.png")))
	{
		m_TileImages.insert({ TileAttribute::JumpPlatRight , _jumpPlatRight });
	}
#pragma endregion

#pragma region Monster
	CImage* _stage1Monster = new CImage;
	if (SUCCEEDED(_stage1Monster->Load("./Image/Monsters/stage1Monster.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Stage1Monster , _stage1Monster });
	}
	CImage* _stage2Monster0 = new CImage;
	if (SUCCEEDED(_stage2Monster0->Load("./Image/Monsters/stage2Monster0.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Stage2Monster0 , _stage2Monster0 });
	}
	CImage* _stage2Monster1 = new CImage;
	if (SUCCEEDED(_stage2Monster1->Load("./Image/Monsters/stage2Monster1.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Stage2Monster1 , _stage2Monster1 });
	}
	CImage* _stage3Boss = new CImage;
	if (SUCCEEDED(_stage3Boss->Load("./Image/Monsters/stage3Boss.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Stage3Boss , _stage3Boss });
	}
#pragma endregion

#pragma region FGObject
	CImage* _leaf0 = new CImage;
	if (SUCCEEDED(_leaf0->Load("./Image/Objects/leaf1.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf0 , _leaf0 });
	}
	CImage* _leaf1 = new CImage;
	if (SUCCEEDED(_leaf1->Load("./Image/Objects/leaf2.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf1 , _leaf1 });
	}
	CImage* _leaf2 = new CImage;
	if (SUCCEEDED(_leaf2->Load("./Image/Objects/leaf3.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf2 , _leaf2 });
	}
	CImage* _leaf3 = new CImage;
	if (SUCCEEDED(_leaf3->Load("./Image/Objects/leaf4.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf3 , _leaf3 });
	}
	CImage* _leaf4 = new CImage;
	if (SUCCEEDED(_leaf4->Load("./Image/Objects/leaf5.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf4 , _leaf4 });
	}
	CImage* _rock0 = new CImage;
	if (SUCCEEDED(_rock0->Load("./Image/Objects/rock.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectRock0 , _rock0 });
	}
	CImage* _rock1 = new CImage;
	if (SUCCEEDED(_rock1->Load("./Image/Objects/rock2.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectRock1 , _rock1 });
	}
	CImage* _sign0 = new CImage;
	if (SUCCEEDED(_sign0->Load("./Image/Objects/sign.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectSign0 , _sign0 });
	}
	CImage* _sign1 = new CImage;
	if (SUCCEEDED(_sign1->Load("./Image/Objects/sign2.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectSign1 , _sign1 });
	}
	CImage* _sign2 = new CImage;
	if (SUCCEEDED(_sign2->Load("./Image/Objects/sign3.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectSign2 , _sign2 });
	}
	CImage* _sign3 = new CImage;
	if (SUCCEEDED(_sign3->Load("./Image/Objects/sign4.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectSign3 , _sign3 });
	}
	CImage* _tree0 = new CImage;
	if (SUCCEEDED(_tree0->Load("./Image/Objects/tree.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree0 , _tree0 });
	}
	CImage* _tree1 = new CImage;
	if (SUCCEEDED(_tree1->Load("./Image/Objects/tree2.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree1 , _tree1 });
	}
	CImage* _tree2 = new CImage;
	if (SUCCEEDED(_tree2->Load("./Image/Objects/tree3.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree2 , _tree2 });
	}

//Flip 이미지
	CImage* _leaf0Flip = new CImage;
	if (SUCCEEDED(_leaf0Flip->Load("./Image/Objects/leaf1flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf0Flip , _leaf0Flip });
	}
	CImage* _leaf1Flip = new CImage;
	if (SUCCEEDED(_leaf1Flip->Load("./Image/Objects/leaf2flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf1Flip , _leaf1Flip });
	}
	CImage* _leaf2Flip = new CImage;
	if (SUCCEEDED(_leaf2Flip->Load("./Image/Objects/leaf3flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf2Flip , _leaf2Flip });
	}
	CImage* _leaf3Flip = new CImage;
	if (SUCCEEDED(_leaf3Flip->Load("./Image/Objects/leaf4flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf3Flip , _leaf3Flip });
	}
	CImage* _leaf4Flip = new CImage;
	if (SUCCEEDED(_leaf4Flip->Load("./Image/Objects/leaf5flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectLeaf4Flip , _leaf4Flip });
	}
	CImage* _rock0Flip = new CImage;
	if (SUCCEEDED(_rock0Flip->Load("./Image/Objects/rockflip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectRock0Flip , _rock0Flip });
	}
	CImage* _rock1Flip = new CImage;
	if (SUCCEEDED(_rock1Flip->Load("./Image/Objects/rock2flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectRock1Flip , _rock1Flip });
	}
	CImage* _tree0Flip = new CImage;
	if (SUCCEEDED(_tree0Flip->Load("./Image/Objects/treeflip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree0Flip, _tree0Flip });
	}
	CImage* _tree1Flip = new CImage;
	if (SUCCEEDED(_tree1Flip->Load("./Image/Objects/tree2flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree1Flip , _tree1Flip });
	}
	CImage* _tree2Flip = new CImage;
	if (SUCCEEDED(_tree2Flip->Load("./Image/Objects/tree3flip.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ObjectTree2Flip , _tree2Flip });
	}
#pragma endregion

#pragma region Etc
	CImage* _heart = new CImage;
	if (SUCCEEDED(_heart->Load("./Image/Objects/heartItem.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Heart , _heart });
	}
	CImage* _portal = new CImage;
	if (SUCCEEDED(_portal->Load("./Image/Objects/portal.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Portal , _portal });
	}
	CImage* _clearEng = new CImage;
	if(SUCCEEDED(_clearEng->Load("./Image/Objects/Clear_Eng.png")))
	{
		m_ObjectImages.insert({ObjectAttribute::ClearEng, _clearEng});
	}
	CImage* _clearKor = new CImage;
	if (SUCCEEDED(_clearKor->Load("./Image/Objects/Clear_Kor.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ClearKor, _clearKor });
	}
	CImage* _clearName = new CImage;
	if (SUCCEEDED(_clearName->Load("./Image/Objects/Clear_Name.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::ClearName, _clearName });
	}
	CImage* _player = new CImage;
	if (SUCCEEDED(_player->Load("./Image/Objects/Idle.png")))
	{
		m_ObjectImages.insert({ ObjectAttribute::Player , _player });
	}
#pragma endregion
}

CImage* CResourceManager::GetTileImage(TileAttribute _attribute)
{
	auto iter = m_TileImages.find(_attribute);
	// 못찾으면 end 반환
	if(iter == m_TileImages.end())
	{
		return nullptr;
	}
	else
	{
		return m_TileImages.at(_attribute);
	}
}

CImage* CResourceManager::GetBGImage(int i)
{
	if (i >= m_BGImages.size())
	{
		return nullptr;
	}
	else
	{
		return m_BGImages[i];
	}
}

CImage* CResourceManager::GetObjectImage(ObjectAttribute _attribute)
{
	auto iter = m_ObjectImages.find(_attribute);
	// 못찾으면 end 반환
	if (iter == m_ObjectImages.end())
	{
		return nullptr;
	}
	else
	{
		return m_ObjectImages.at(_attribute);
	}
}


void CResourceManager::Update()
{

}

// 벡터들을 돌면서 지워준다.
void CResourceManager::Finalize()
{
	// 처음부터 끝까지 돌면서
		// 두 번째 값인 CImage*를 제거하고
		// 그 위치의 맵 값을 삭제
	for(auto iter = m_TileImages.begin(); iter != m_TileImages.end(); iter++)
	{
		delete *(iter)->second;
		m_TileImages.erase(iter);
	}
	for (int i = 0; i < m_BGImages.size(); i++)
	{
		delete m_BGImages[i];
	}
	for (auto iter = m_ObjectImages.begin(); iter != m_ObjectImages.end(); iter++)
	{
		delete* (iter)->second;
		m_ObjectImages.erase(iter);
	}
}