#pragma once

namespace SanyoniEngine
{
	struct Rect : public D2D_RECT_F
	{
	public:
		Rect() = default;

		Rect(FLOAT _left, FLOAT _top, FLOAT _right, FLOAT _bottom)
			: D2D_RECT_F{_left, _top, _right, _bottom}
		{
		}

		FLOAT GetWidth() const { return this->right - this->left; }

		FLOAT GetHeight() const { return this->bottom - this->top; }
	};
}
