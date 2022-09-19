#pragma once

#include "Application.h"

class GameApplication :
	public SanyoniEngine::Application
{
protected:
	void OnStart() override;

	void OnUpdate() override;
};
