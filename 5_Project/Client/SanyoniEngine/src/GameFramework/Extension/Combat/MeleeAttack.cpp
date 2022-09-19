#include "SanyoniEngine_pch.h"
#include "MeleeAttack.h"

#include "GameEvent.h"
#include "GameObject.h"
#include "SpriteAnimator.h"

using namespace SanyoniEngine;

MeleeAttack::MeleeAttack(GameObject* _gameObject, tstring _typeName) :
	ComponentBase(_gameObject, _typeName)
{
}

void MeleeAttack::Attack(tstring _attackName)
{
	// TODO: 애니메이션 정보에서 float값들을 읽어와 rect를 만듭니다.
	// auto _animator = GetGameObject()->GetComponent<SpriteAnimator>();
	// auto _valueCollection = _animator->GetValueCollection();
	// auto _animTime = _animator.GetCurrentClipPlayingTime();
	// _valueCollection->GetFloat("AttackRangeLeftTop", _animTime);

	// TODO: Rect로 Overlap검사를 실시하고, 충돌한 collider들 중 Health가 있는 컴포넌트들을 불러옵니다.

	// TODO: 컴포넌트에게 데미지 메세지를 보냅니다.
}
