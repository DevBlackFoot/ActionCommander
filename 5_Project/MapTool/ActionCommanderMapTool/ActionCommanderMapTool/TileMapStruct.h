#pragma once

// Tile�� Object�� �Ӽ��� ��Ÿ�������� AttributeŬ����
// �� - 0x01xx / ���� �� �� ���� �÷��� - 0x02xx, ���� �� �� �ִ� �÷��� - 0x03xx
enum class TileAttribute
{
	GroundLeftTop			=	0x0101,
	GroundTop				=	0x0102,
	GroundRightTop			=	0x0103,
	GroundLeft				=	0x0104,
	GroundCenter			=	0x0105,
	GroundRight				=	0x0106,
	GroundLeftBottom		=	0x0107,
	GroundBottom			=	0x0108,
	GroundRightBottom		=	0x0109,
	GroundDiaLeftTop		=	0x0110,
	GroundDiaRightTop		=	0x0111,
	GroundDiaLeftBottom		=	0x0112,
	GroundDiaRightBottom	=	0x0113,

	NotJumpPlatLeft		=	0x0201,
	NotJumpPlatCenter	=	0x0202,
	NotJumpPlatRight	=	0x0203,

	JumpPlatLeft		=	0x0301,
	JumpPlatCenter		=	0x0302,
	JumpPlatRight		=	0x0303,

	None				=	0xFFFF,
};

// ���� - 0x01xx / ��ȣ �ۿ�(��Ʈ, ��Ż) - 0x02xx, ���� - 0x03xx, �÷��̾� - 0x04xx
enum class ObjectAttribute
{
	ObjectLeaf0			=	0x0101,
	ObjectLeaf1			=	0x0102,
	ObjectLeaf2			=	0x0103,
	ObjectLeaf3			=	0x0104,
	ObjectLeaf4			=	0x0105,
	ObjectRock0			=	0x0106,
	ObjectSign0			=	0x0107,
	ObjectTree0			=	0x0108,
	ObjectTree1			=	0x0109,
	// Flip ������Ʈ�� �� ���� ���̱�.
	ObjectLeaf0Flip		=	0x0110,
	ObjectLeaf1Flip		=	0x0111,
	ObjectLeaf2Flip		=	0x0112,
	ObjectLeaf3Flip		=	0x0113,
	ObjectLeaf4Flip		=	0x0114,
	ObjectRock0Flip		=	0x0115,
	ObjectTree0Flip		=	0x0116,
	ObjectTree1Flip		=	0x0117,
	// �ε忡�� ������ �ʱ� ���ؼ�..
	ObjectTree2			=	0x0118,
	ObjectTree2Flip		=	0x0119,
	ObjectRock1			=	0x0120,
	ObjectRock1Flip		=	0x0121,
	ObjectSign1			=	0x0122,
	ObjectSign2			=	0x0123,
	ObjectSign3			=	0x0124,

	Heart				=	0x0201,
	Portal				=	0x0202,
	ClearEng			=	0x0203,
	ClearKor			=	0x0204,
	ClearName			=	0x0205,

	Stage1Monster		=	0x0301,
	Stage2Monster0		=	0x0302,
	Stage2Monster1		=	0x0303,
	Stage3Boss			=	0x0304,

	Player				=	0x0401,

	None				=	0xFFFF,

};

// Ÿ�ϰ� �ʿ� ���� ����ü ����.

// Ÿ�� �ϳ��ϳ��� ������ ������ �ִ� ����ü
// ���� ��� ���� (left, top)
	// �ش� ��ġ ���� �׸��� �׸���.
	// �⺻������ Ÿ�ϸ��� 64*64 �������̱� ������ �ش� ��ġ���� �׸��� �׸���.
// Ÿ���� �Ӽ����� ��Ÿ���� attribute
// �ش� ������ ���� Ŭ���̾�Ʈ�� ���� �� ������Ʈȭ �ȴ�.
	// �⺻������ ��������Ʈ �������� �پ �̹����� ó���ϰ�
	// ���� �̵��� ��� �ݶ��̴��� �����Ѵ�. => �ݶ��̴� ������Ʈ�� �Ӽ��� �ľ��Ѵ�.
struct Tile
{
	int left, top;
	TileAttribute tileAttribute;
};

// ���Ϳ� ���� Ÿ�ϰ� ���� ���� ������Ʈ�� ���õ� ����ü
	// ���� ��� ����(PosX, PosY) => ������ �Ľ��� ���� ����
		// �ش� ��ġ���� ���� ��Ʈ�� �����ŭ �׸��� �׸���.
		// Ÿ���� ���� ��Ÿ���� ��� ���� ������Ʈ.
		// ���Ͷ�� ���� �� �ٸ� ������� ó���� ���־���� ����.
	// �׸��� ���� Center�� ��Ʈ�� ���̵� ������ ����������.
	// ������ �Ӽ� ���� ��Ÿ���� attribute
struct Object
{
	int left, top;
	int centerX, centerY;
	float bmpWidth, bmpHeight;
	ObjectAttribute objectAttribute;
};



// Ÿ�� �ʰ� ���õ� ����ü
	// App�� ����Ǿ� ��µǴ� ����ü
	// Ÿ�� �ʰ� ������Ʈ vector�� �̷�����ִٰ� ����.
	// �ش� ���͸� ������ �� ���� Ÿ�ϰ� ������Ʈ�� ���� �� �ִ�.
// �� ��ü�� ũ��(mapX, mapY)
// Ÿ�� ���Ϳ� ������Ʈ ���� => �� ������ �ۿ� public���� ������ �ֵ��� �ص� ��� ���� �� �ϴ�.
