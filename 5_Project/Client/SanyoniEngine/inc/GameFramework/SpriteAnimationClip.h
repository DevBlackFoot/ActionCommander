#pragma once

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class Sprite;

	struct SpriteAnimationClip
	{
		tstring name;
		unsigned int fps;
		std::vector<Sprite*> sprites;
	};
}
