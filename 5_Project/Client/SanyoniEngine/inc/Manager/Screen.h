#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class TextFormat;

	class Screen
	{
		DECLARE_SINGLETON_CLASS(Screen)

	public:
		void Initialize();

		void Release();

		void PreRender();

		void PostRender();

		void SetTransform(D2D1::Matrix3x2F _transform);

		/*
		 * DrawLine
		 */

		void DrawLineScreenSpace(const Color& _color, const Point2& _point1, const Point2& _point2,
		                         const Matrix3x2& _transform = Matrix3x2::Identity,
		                         float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawLineScreenSpace(Bitmap* _bitmap, const Point2& _point1, const Point2& _point2,
		                         const Matrix3x2& _transform = Matrix3x2::Identity,
		                         float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawLineWorldSpace(const Color& _color, const Point2& _point1, const Point2& _point2,
		                        const Matrix3x2& _transform = Matrix3x2::Identity,
		                        float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawLineWorldSpace(Bitmap* _bitmap, const Point2& _point1, const Point2& _point2,
		                        const Matrix3x2& _transform = Matrix3x2::Identity,
		                        float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		/*
		 * DrawRectangle
		 */

		void DrawRectangleScreenSpace(const Color& _color, const Rect& _rect,
		                              const Matrix3x2& _transform = Matrix3x2::Identity,
		                              float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawRectangleScreenSpace(Bitmap* _bitmap, const Rect& _rect,
		                              const Matrix3x2& _transform = Matrix3x2::Identity,
		                              float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawRectangleScreenSpace(const Color& _color, class Transform* _transform, const float _width,
		                              const float _height,
		                              float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawRectangleWorldSpace(const Color& _color, const Rect& _rect,
		                             const Matrix3x2& _transform = Matrix3x2::Identity,
		                             float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawRectangleWorldSpace(Bitmap* _bitmap, const Rect& _rect,
		                             const Matrix3x2& _transform = Matrix3x2::Identity,
		                             float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawRectangleWorldSpace(const Color& _color, class Transform* _transform, const float _width,
		                             const float _height,
		                             float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);


		/*
		 * DrawCircle
		 */

		void DrawCircleScreenSpace(const Color& _color, const Ellipse& _ellipse,
		                           const Matrix3x2& _transform = Matrix3x2::Identity,
		                           float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawCircleScreenSpace(Bitmap* _bitmap, const Ellipse& _ellipse,
		                           const Matrix3x2& _transform = Matrix3x2::Identity,
		                           float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawCircleScreenSpace(const Color& _color, class Transform* _transform, const float _radius,
		                           float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawCircleWorldSpace(const Color& _color, const Ellipse& _ellipse,
		                          const Matrix3x2& _transform = Matrix3x2::Identity,
		                          float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawCircleWorldSpace(Bitmap* _bitmap, const Ellipse& _ellipse,
		                          const Matrix3x2& _transform = Matrix3x2::Identity,
		                          float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		void DrawCircleWorldSpace(const Color& _color, class Transform* _transform, const float _radius,
		                          float _strokeWidth = 1.f, ID2D1StrokeStyle* _strokeStyle = nullptr);

		/*
		 * DrawBitmap
		 */

		void DrawBitmapFillScreenSpace(float _fillAmount,
		                               const SpriteInfo& _spriteInfo, const Matrix3x2& _transform, bool _flip = false,
		                               float _opacity = 1.f,
		                               D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode =
			                               D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

		void DrawBitmapFillWorldSpace(float _fillAmount,
		                               const SpriteInfo& _spriteInfo, class Transform* _transform, bool _flip = false,
		                               float _opacity = 1.f,
		                               D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode =
			                               D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

		void DrawBitmapScreenSpace(const SpriteInfo& _spriteInfo, const Matrix3x2& _transform, bool _flip = false,
		                           float _opacity = 1.f,
		                           D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode =
			                           D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

		void DrawBitmapWorldSpace(const SpriteInfo& _spriteInfo, class Transform* _transform, bool _flip = false,
		                          float _opacity = 1.f,
		                          D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode =
			                          D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

		/*
		 * DrawText
		 */

		void DrawTextScreenSpace(const tstring& _text, TextFormat* _textFormat,
		                         const Vector2& _position, const Color& _color = Color::Black,
		                         const Matrix3x2& _transform = Matrix3x2::Identity);

		void DrawTextWorldSpace(const tstring& _text, TextFormat* _textFormat,
		                        const Vector2& _position, const Color& _color = Color::Black);

		/*
		 * Callbacks
		 */

		void OnResize(UINT _width, UINT _height);

	private:
		SolidColorBrush* m_SolidColorBrush;

	public:
		Vector2 GetSize() const;

	public:
		static Vector2 ScreenToWorldPosition(const Vector2& _screenPosition);

		static Vector2 WorldToScreenPosition(const Vector2& _worldPosition);
	};
}
