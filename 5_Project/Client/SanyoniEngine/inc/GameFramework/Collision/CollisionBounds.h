#pragma once

#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class ColliderBase;
	class AABBCollider;
	class CircleCollider;
}

namespace SanyoniEngine::Collision
{
	class OBBBounds;
	class CircleBounds;
	class RayBounds;

	class BoundsBase
	{
	};

	class AABBBounds : public BoundsBase
	{
	public:
		AABBBounds();

		AABBBounds(float _left, float _top, float _right, float _bottom);

		AABBBounds(AABBCollider* _collider);

		AABBBounds(CircleCollider* _collider);

		AABBBounds(ColliderBase* _collider);

		AABBBounds(const CircleBounds& _bounds);

		AABBBounds(const OBBBounds& _bounds);

		AABBBounds(const RayBounds& _bounds);

		bool operator==(const AABBBounds&& _other) const;

		void Set(float _left, float _top, float _right, float _bottom);

		void Set(AABBCollider* _collider);

		void Set(CircleCollider* _collider);

	private:
		float left;
		float top;
		float right;
		float bottom;

	public:
		inline float GetLeft() const { return left; }
		inline float GetTop() const { return top; }
		inline float GetRight() const { return right; }
		inline float GetBottom() const { return bottom; }

		inline void SetLeft(float _left) { left = _left; }
		inline void SetTop(float _top) { top = _top; }
		inline void SetRight(float _right) { right = _right; }
		inline void SetBottom(float _bottom) { bottom = _bottom; }

		inline Vector2 GetLeftTop() const { return {left, top}; }
		inline Vector2 GetRightTop() const { return {right, top}; }
		inline Vector2 GetRightBottom() const { return {right, bottom}; }
		inline Vector2 GetLeftBottom() const { return {left, bottom}; }

		inline Vector2 GetCenter() const { return {(right + left) / 2, (bottom + top) / 2}; }
		inline float GetCenterX() const { return (right + left) / 2; }
		inline float GetCenterY() const { return (bottom + top) / 2; }

		inline Vector2 GetSize() const { return {right - left, bottom - top}; }
		inline float GetWidth() const { return (right - left); }
		inline float GetHeight() const { return (bottom - top); }
	};

	class OBBBounds : public BoundsBase
	{
	public:
		OBBBounds(const Vector2&& _leftTop, const Vector2&& _rightTop, const Vector2&& _rightBottom,
		          Vector2 _leftBottom);

		bool operator==(const OBBBounds&& _other) const;

		void Set(const Vector2&& _leftTop, const Vector2&& _rightTop, const Vector2&& _rightBottom,
		         const Vector2&& _leftBottom);

	private:
		Vector2 leftTop;
		Vector2 rightTop;
		Vector2 rightBottom;
		Vector2 leftBottom;

	public:
		inline Vector2 GetLeftTop() const { return leftTop; }
		inline Vector2 GetRightTop() const { return rightTop; }
		inline Vector2 GetRightBottom() const { return rightBottom; }
		inline Vector2 GetLeftBottom() const { return leftBottom; }

		inline void SetLeftTop(const Vector2&& _leftTop) { leftTop = _leftTop; }
		inline void SetRightTop(const Vector2&& _rightTop) { rightTop = _rightTop; }
		inline void SetRightBottom(const Vector2&& _rightBottom) { rightBottom = _rightBottom; }
		inline void SetLeftBottom(const Vector2&& _leftBottom) { leftBottom = _leftBottom; }
	};

	class CircleBounds : public BoundsBase
	{
	public:
		CircleBounds(Point2 _center, float _radius);

		CircleBounds(float _centerX, float _centerY, float _radius);

		CircleBounds(CircleCollider* _collider);

		bool operator==(const CircleBounds&& _other) const;

		void Set(const Point2&& _center, float _radius);

		void Set(float _centerX, float _centerY, float _radius);

		void Set(CircleCollider* _collider);

	private:
		Point2 center;
		float radius;
		float radiusPow;

	public:
		inline Point2 GetCenter() const { return center; }
		inline float GetRadius() const { return radius; }
		inline float GetRadiusPow() const { return radiusPow; }

		inline void SetCenter(const Point2&& _center) { center = _center; }

		inline void SetRadius(float _radius)
		{
			radius = _radius;
			radiusPow = _radius * _radius;
		}
	};

	class RayBounds : public BoundsBase
	{
	public:
		RayBounds(const Vector2& _start, const Vector2& _direction, const float _length);

		RayBounds(const Vector2& _start, const Vector2& _end);

		bool operator==(const RayBounds&& _other) const;

		void Set(const Vector2& _start, const Vector2& _direction, const float _length);

		void Set(const Vector2& _start, const Vector2& _end);

	private:
		inline void UpdateEnd() { end = start + direction * length; }

		Vector2 start;
		Vector2 direction;
		float length;

		Vector2 end;

	public:
		inline Vector2 GetStart() const { return start; }
		inline Vector2 GetEnd() const { return end; }
		inline Vector2 GetDirection() const { return direction; }
		inline float GetLength() const { return length; }

		inline void SetStart(const Vector2& _center)
		{
			start = _center;
			UpdateEnd();
		}

		void SetEnd(const Vector2& _end)
		{
			end = _end;
			const auto _startToEndDifference = end - start;
			const auto _startToEnd = Vector2{_startToEndDifference.x, _startToEndDifference.y};
			direction = _startToEnd.Normalize();
			length = _startToEnd.Magnitude();
		}

		inline void SetDirection(const Vector2& _direction)
		{
			direction = _direction.Normalize();
			UpdateEnd();
		}

		inline void SetLength(float _length)
		{
			length = _length;
			UpdateEnd();
		}
	};
}
