#include "SanyoniEngine_pch.h"
#include "Screen.h"

#include "Camera.h"
#include "Core.h"
#include "GameTime.h"
#include "Helper.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextFormat.h"
#include "Transform.h"

using namespace SanyoniEngine;

IMPLEMENT_SINGLETON_CLASS(Screen)

void Screen::Initialize()
{
	Core::GetRenderTarget()->CreateSolidColorBrush(Color::White, &m_SolidColorBrush);
}

void Screen::Release()
{
	m_SolidColorBrush->Release();
}

void Screen::PreRender()
{
	Core::GetRenderTarget()->BeginDraw();
	Core::GetRenderTarget()->Clear(Color::Black);
}

void Screen::PostRender()
{
	HRESULT _hr;

	_hr = Core::GetRenderTarget()->EndDraw();

	if (_hr == D2DERR_RECREATE_TARGET)
	{
		_hr = S_OK;
		Core::Instance.ReleaseDeviceResources();
	}
	else if (_hr != S_OK)
	{
		assert(false);
	}
}

void Screen::SetTransform(D2D1::Matrix3x2F _transform)
{
	Core::GetRenderTarget()->SetTransform(_transform);
}

void Screen::DrawLineScreenSpace(const Color& _color, const Point2& _point1, const Point2& _point2,
                                 const Matrix3x2& _transform,
                                 float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	m_SolidColorBrush->SetColor(_color);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawLine(_point1, _point2, m_SolidColorBrush, _strokeWidth, _strokeStyle);
}

void Screen::DrawLineScreenSpace(Bitmap* _bitmap, const Point2& _point1, const Point2& _point2,
                                 const Matrix3x2& _transform,
                                 float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	BitmapBrush* _brush = nullptr;
	Core::GetRenderTarget()->CreateBitmapBrush(_bitmap, &_brush);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawLine(_point1, _point2, _brush, _strokeWidth, _strokeStyle);
}

void Screen::DrawLineWorldSpace(const Color& _color, const Point2& _point1, const Point2& _point2,
                                const Matrix3x2& _transform, float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	Screen::DrawLineScreenSpace(_color, _point1, _point2,
	                            _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->
	                                                                GetCameraWorldTM(),
	                            _strokeWidth, _strokeStyle);
}

void Screen::DrawLineWorldSpace(Bitmap* _bitmap, const Point2& _point1, const Point2& _point2,
                                const Matrix3x2& _transform, float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	Screen::DrawLineScreenSpace(_bitmap, _point1, _point2,
	                            _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->
	                                                                GetCameraWorldTM(),
	                            _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleScreenSpace(const Color& _color, const Rect& _rect, const Matrix3x2& _transform,
                                      float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	m_SolidColorBrush->SetColor(_color);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawRectangle(_rect, m_SolidColorBrush, _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleScreenSpace(Bitmap* _bitmap, const Rect& _rect, const Matrix3x2& _transform,
                                      float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	BitmapBrush* _brush = nullptr;
	Core::GetRenderTarget()->CreateBitmapBrush(_bitmap, &_brush);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawRectangle(_rect, _brush, _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleScreenSpace(const Color& _color, Transform* _transform, const float _width,
                                      const float _height,
                                      float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	const float _halfWidth = _width / 2;
	const float _halfHeight = _height / 2;

	DrawRectangleScreenSpace(_color, {-_halfWidth, -_halfHeight, _halfWidth, _halfHeight}, _transform->GetWorldTM(),
	                         _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleWorldSpace(const Color& _color, const Rect& _rect, const Matrix3x2& _transform,
                                     float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	DrawRectangleScreenSpace(_color, _rect,
	                         _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
	                         _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleWorldSpace(Bitmap* _bitmap, const Rect& _rect, const Matrix3x2& _transform,
                                     float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	DrawRectangleScreenSpace(_bitmap, _rect,
	                         _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
	                         _strokeWidth, _strokeStyle);
}

void Screen::DrawRectangleWorldSpace(const Color& _color, Transform* _transform, const float _width,
                                     const float _height, float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	const float _halfWidth = _width / 2;
	const float _halfHeight = _height / 2;

	DrawRectangleScreenSpace(_color, {-_halfWidth, -_halfHeight, _halfWidth, _halfHeight},
	                         _transform->GetWorldTM() * SceneManager::Instance.GetActiveScene()->GetMainCamera()->
	                                                                           GetCameraWorldTM(),
	                         _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleScreenSpace(const Color& _color, const Ellipse& _ellipse, const Matrix3x2& _transform,
                                   float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	m_SolidColorBrush->SetColor(_color);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawEllipse(_ellipse, m_SolidColorBrush, _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleScreenSpace(Bitmap* _bitmap, const Ellipse& _ellipse, const Matrix3x2& _transform,
                                   float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	BitmapBrush* _brush = nullptr;
	Core::GetRenderTarget()->CreateBitmapBrush(_bitmap, &_brush);

	SetTransform(_transform);
	Core::GetRenderTarget()->DrawEllipse(_ellipse, _brush, _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleScreenSpace(const Color& _color, Transform* _transform, const float _radius, float _strokeWidth,
                                   ID2D1StrokeStyle* _strokeStyle)
{
	DrawCircleScreenSpace(_color, {0, 0, _radius, _radius}, _transform->GetWorldTM(),
	                      _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleWorldSpace(const Color& _color, const Ellipse& _ellipse, const Matrix3x2& _transform,
                                  float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	DrawCircleScreenSpace(_color, _ellipse,
	                      _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
	                      _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleWorldSpace(Bitmap* _bitmap, const Ellipse& _ellipse, const Matrix3x2& _transform,
                                  float _strokeWidth, ID2D1StrokeStyle* _strokeStyle)
{
	DrawCircleScreenSpace(_bitmap, _ellipse,
	                      _transform * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
	                      _strokeWidth, _strokeStyle);
}

void Screen::DrawCircleWorldSpace(const Color& _color, Transform* _transform, const float _radius, float _strokeWidth,
                                  ID2D1StrokeStyle* _strokeStyle)
{
	DrawCircleScreenSpace(_color, {0, 0, _radius, _radius},
	                      _transform->GetWorldTM() * SceneManager::Instance.GetActiveScene()->GetMainCamera()->
	                                                                        GetCameraWorldTM(),
	                      _strokeWidth, _strokeStyle);
}

void Screen::DrawBitmapFillScreenSpace(float _fillAmount, const SpriteInfo& _spriteInfo, const Matrix3x2& _transform, bool _flip, float _opacity,
                                       D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode)
{
	// TODO: 상당히 많은 부분이 DrawBitmapScreenSpace와 중복됩니다.
	// TODO: fill의 옵션이 주어지지 않고, 오로지 왼쪽에서 오른쪽으로 채워지는 방식으로만 구현되었습니다.

	const Vector2 _pivotAppliedLeftTop{
		-_spriteInfo.rect.GetWidth() * _spriteInfo.pivot.x,
		-_spriteInfo.rect.GetHeight() + _spriteInfo.rect.GetHeight() * _spriteInfo.pivot.y
	};

	const Rect _pivotAppliedScreenRect{
		_pivotAppliedLeftTop.x,
		_pivotAppliedLeftTop.y,
		_pivotAppliedLeftTop.x + _spriteInfo.rect.GetWidth() * _fillAmount,
		_pivotAppliedLeftTop.y + _spriteInfo.rect.GetHeight()
	};

	const Rect _sourceRect{
		_spriteInfo.rect.left,
		_spriteInfo.rect.top,
		_spriteInfo.rect.left + _spriteInfo.rect.GetWidth() * _fillAmount,
		_spriteInfo.rect.top + _spriteInfo.rect.GetHeight(),
	};

	SetTransform(
		_flip == false
			? _transform
			: Matrix3x2::MakeScaleMatrix({-1, 1}) * _transform
	);

	if (Core::GetEnableDebugNoDrawBitmap() == false)
	{
		Core::GetRenderTarget()->DrawBitmap(_spriteInfo.bitmap,
		                                    _pivotAppliedScreenRect,
		                                    _opacity,
		                                    _interpolationMode,
		                                    _sourceRect
		);
	}


	if (Core::GetEnableDebug())
	{
		// debug draw
		Core::GetRenderTarget()->DrawRectangle(_pivotAppliedScreenRect, m_SolidColorBrush, 2);
	}
}

void Screen::DrawBitmapFillWorldSpace(float _fillAmount, const SpriteInfo& _spriteInfo, Transform* _transform, bool _flip, float _opacity,
                                      D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode)
{
	DrawBitmapFillScreenSpace(
		_fillAmount,
		_spriteInfo,
		_transform->GetWorldTM() * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
		_flip,
		_opacity, _interpolationMode);
}

void Screen::DrawBitmapScreenSpace(const SpriteInfo& _spriteInfo, const Matrix3x2& _transform, bool _flip,
                                   float _opacity, D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode)
{
	const Vector2 _pivotAppliedLeftTop{
		-_spriteInfo.rect.GetWidth() * _spriteInfo.pivot.x,
		-_spriteInfo.rect.GetHeight() + _spriteInfo.rect.GetHeight() * _spriteInfo.pivot.y
	};

	const Rect _pivotAppliedScreenRect{
		_pivotAppliedLeftTop.x,
		_pivotAppliedLeftTop.y,
		_pivotAppliedLeftTop.x + _spriteInfo.rect.GetWidth(),
		_pivotAppliedLeftTop.y + _spriteInfo.rect.GetHeight()
	};

	SetTransform(
		_flip == false
			? _transform
			: Matrix3x2::MakeScaleMatrix({-1, 1}) * _transform
	);


	if (Core::GetEnableDebugNoDrawBitmap() == false)
	{
		Core::GetRenderTarget()->DrawBitmap(_spriteInfo.bitmap,
		                                    _pivotAppliedScreenRect,
		                                    _opacity,
		                                    _interpolationMode,
		                                    _spriteInfo.rect
		);
	}


	if (Core::GetEnableDebug())
	{
		// debug draw
		Core::GetRenderTarget()->DrawRectangle(_pivotAppliedScreenRect, m_SolidColorBrush, 2);
	}
}

void Screen::DrawBitmapWorldSpace(const SpriteInfo& _spriteInfo, Transform* _transform, bool _flip,
                                  float _opacity, D2D1_BITMAP_INTERPOLATION_MODE _interpolationMode)
{
	DrawBitmapScreenSpace(_spriteInfo,
	                      _transform->GetWorldTM() * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM(),
	                      _flip,
	                      _opacity, _interpolationMode);
}

void Screen::DrawTextScreenSpace(const tstring& _text, TextFormat* _textFormat,
                                 const Vector2& _position, const Color& _color,
                                 const Matrix3x2& _transform)
{
	m_SolidColorBrush->SetColor(_color);

	SetTransform(_transform);

	// TODO: 텍스트 포맷의 출력 옵션 설정을 제공해주어야 합니다.

	// TODO: 텍스트 박스의 크기가 하드코딩으로 작성되어 있습니다.

	const Rect _rect = Rect(_position.x, _position.y, _position.x + _textFormat->GetFontSize() * _text.length(), _position.y + _textFormat->GetFontSize());

	Core::GetRenderTarget()->DrawTextW(
		_text.c_str(), _text.length(),
		_textFormat->GetTextFormat(),
		_rect,
		m_SolidColorBrush);
}

void Screen::DrawTextWorldSpace(const tstring& _text, TextFormat* _textFormat, const Vector2& _position, const Color& _color)
{
	DrawTextScreenSpace(_text, _textFormat,
	                    Vector2::Zero, _color,
	                    Matrix3x2::MakeTranslateMatrix(_position) * SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetCameraWorldTM());
}

void Screen::OnResize(UINT _width, UINT _height)
{
	if (Core::GetRenderTarget() == nullptr)
		return;

	// Note: 이 함수의 동작에 실패할 수 있지만,
	// 다음 번에 EndDraw를 호출할 때 다시 에러가 반환될 것이므로
	// 여기에서는 이 에러를 무시해도 됩니다.
	HRESULT _hr;
	if ((_hr = Core::GetRenderTarget()->Resize(D2D1::SizeU(_width, _height))) != S_OK)
	{
		assert(false);
		return;
	}
}

Vector2 Screen::GetSize() const
{
	auto _size = Core::GetRenderTarget()->GetSize();
	return {_size.width, _size.height};
}

Vector2 Screen::ScreenToWorldPosition(const Vector2& _screenPosition)
{
	const auto _worldPosition =
		SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetLeftTopWorldPosition() + _screenPosition;

	return _worldPosition;
}

Vector2 Screen::WorldToScreenPosition(const Vector2& _worldPosition)
{
	const auto _screenPosition =
		_worldPosition - SceneManager::Instance.GetActiveScene()->GetMainCamera()->GetLeftTopWorldPosition();

	return _screenPosition;
}
