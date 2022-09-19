#include "SanyoniEngine_pch.h"
#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Screen.h"

using namespace SanyoniEngine;

SpriteRenderer::SpriteRenderer(GameObject* _gameObject, tstring _typeName):
	ComponentBase(_gameObject, _typeName),
	m_Sprite(nullptr),
	m_Flip(false),
	m_Opacity(1.f)
{
}

void SpriteRenderer::OnRender()
{
	if (m_Sprite == nullptr)
		return;


	if (m_bEnableFillMode == false)
	{
		Screen::Instance.DrawBitmapWorldSpace(
			m_Sprite->GetSpriteInfo(),
			m_GameObject->GetComponent<Transform>(),
			m_Flip,
			m_Opacity
		);
	}
	else
	{
		Screen::Instance.DrawBitmapFillWorldSpace(
			m_FillAmount,
			m_Sprite->GetSpriteInfo(),
			m_GameObject->GetComponent<Transform>(),
			m_Flip,
			m_Opacity
		);
	}
}
