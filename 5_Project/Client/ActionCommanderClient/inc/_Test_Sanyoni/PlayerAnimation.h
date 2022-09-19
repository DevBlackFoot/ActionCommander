#pragma once
#include "ComponentBase.h"

class PlayerAnimation :
	public ComponentBase
{
public:
	PlayerAnimation(GameObject* _gameObject, const tstring& _typeName = TEXT("PlayerAnimation"));

	void PostTick() override;

private:
	bool BasicAttack();

	bool Skill1();
	bool Skill2();
	bool Skill3();
	bool Skill4();

private:
	bool m_bDoBasicAttack;

	bool m_bDoSkill1;
	bool m_bDoSkill2;
	bool m_bDoSkill3;
	bool m_bDoSkill4;

public:
	void DoBasicAttack() { m_bDoBasicAttack = true; }

	void DoSkill1() { m_bDoSkill1 = true; }

	void DoSkill2() { m_bDoSkill2 = true; }

	void DoSkill3() { m_bDoSkill3 = true; }

	void DoSkill4() { m_bDoSkill4 = true; }
};
