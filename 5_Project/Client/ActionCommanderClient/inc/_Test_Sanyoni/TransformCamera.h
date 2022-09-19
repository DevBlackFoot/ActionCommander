#pragma once

#include "ComponentBase.h"

namespace SanyoniEngine
{
	class GameObject;
}

class TransformCamera : public SanyoniEngine::ComponentBase
{
public:
	TransformCamera(SanyoniEngine::GameObject* _gameObject, tstring _typeName = TEXT("TransformCamera"));

	void Tick() override;
};
