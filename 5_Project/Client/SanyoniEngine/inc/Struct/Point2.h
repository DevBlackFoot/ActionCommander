#pragma once

namespace SanyoniEngine
{
	struct IVector2;

	struct IPoint2
	{
	};

	template <typename TElement>
	struct Point2_Base
	{
		TElement x;
		TElement y;
	};

	template <typename TBase, typename TElement>
	struct Point2_DerivedBase : public TBase, public IPoint2
	{
	private:
		using TPoint2 = Point2_DerivedBase<TBase, TElement>;

		static constexpr float EPSILON = 0.001f;

	public:
		Point2_DerivedBase() = default;

		Point2_DerivedBase(TElement _x, TElement _y)
			: TBase{_x, _y}
		{
		}

		TPoint2 operator+(const TPoint2& _other) const
		{
			return {this->x + _other.x, this->y + _other.y};
		}

		template <typename TVector2>
		TPoint2 operator+(const TVector2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			return {this->x + _other.x, this->y + _other.y};
		}

		TPoint2 operator-(const TPoint2& _other) const
		{
			return {this->x - _other.x, this->y - _other.y};
		}

		template <typename TVector2>
		TPoint2 operator-(const TVector2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			return {this->x - _other.x, this->y - _other.y};
		}

		bool operator==(const TPoint2& _other) const
		{
			const bool _isXEquals = abs(this->x - _other.x) <= EPSILON;
			const bool _isYEquals = abs(this->y - _other.y) <= EPSILON;

			return _isXEquals && _isYEquals;
		}

		template <typename TVector2>
		bool operator==(const TVector2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			const bool _isXEquals = abs(this->x - _other.x) <= EPSILON;
			const bool _isYEquals = abs(this->y - _other.y) <= EPSILON;

			return _isXEquals && _isYEquals;
		}

		bool operator!=(const TPoint2& _other) const
		{
			return !(*this == _other);
		}

		template <typename TVector2>
		bool operator!=(const TVector2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			return !(*this == _other);
		}

		float DistanceTo(const TPoint2& _other) const
		{
			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return std::sqrt(_xDifference * _xDifference + _yDifference * _yDifference);
		}

		template <typename TVector2>
		float DistanceTo(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return std::sqrt(_xDifference * _xDifference + _yDifference * _yDifference);
		}

		float DistanceTo_Pow(const TPoint2& _other) const
		{
			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return _xDifference * _xDifference + _yDifference * _yDifference;
		}

		template <typename TVector2>
		float DistanceTo_Pow(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IVector2, TVector2>::value,
				"The template parameter is not of the correct type. type must be inherited from IVector2. Make sure you pass the correct type.");

			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return _xDifference * _xDifference + _yDifference * _yDifference;
		}

	public:
		static const TPoint2 Zero;

		static void GetMinAndMax(TPoint2& _outMin, TPoint2& _outMax, const TPoint2& _vector1, const TPoint2& _vector2)
		{
			TElement _minX, _maxX;
			TElement _minY, _maxY;

			if (_vector1.x <= _vector2.x)
			{
				_maxX = _vector2.x;
				_minX = _vector1.x;
			}
			else
			{
				_maxX = _vector1.x;
				_minX = _vector2.x;
			}

			if (_vector1.y <= _vector2.y)
			{
				_maxY = _vector2.y;
				_minY = _vector1.y;
			}
			else
			{
				_maxY = _vector1.y;
				_minY = _vector2.y;
			}

			_outMin = {_minX, _minY};
			_outMax = {_maxX, _maxY};
		}
	};

	using Point2 = SanyoniEngine::Point2_DerivedBase<D2D_POINT_2F, FLOAT>;
	using LongPoint2 = SanyoniEngine::Point2_DerivedBase<D2D_POINT_2L, LONG>;
	using UIntPoint2 = SanyoniEngine::Point2_DerivedBase<D2D_POINT_2U, UINT>;
	using IntPoint2 = SanyoniEngine::Point2_DerivedBase<SanyoniEngine::Point2_Base<int>, int>;

#define __COMMA ,

#define __Point2		SanyoniEngine::Point2_DerivedBase<D2D_POINT_2F __COMMA FLOAT>
#define __LongPoint2	SanyoniEngine::Point2_DerivedBase<D2D_POINT_2L __COMMA LONG>
#define __UIntPoint2	SanyoniEngine::Point2_DerivedBase<D2D_POINT_2U __COMMA UINT>
#define __IntPoint2		SanyoniEngine::Point2_DerivedBase<SanyoniEngine::Point2_Base<int> __COMMA int>

#define __DEFINE_POINT2_STATIC_MEMBERS__(TPoint2, TPoint2Origin, TElem) \
											const TPoint2 TPoint2Origin::Zero{static_cast<TElem>(0), static_cast<TElem>(0) };

	__DEFINE_POINT2_STATIC_MEMBERS__(Point2, __Point2, FLOAT)
	__DEFINE_POINT2_STATIC_MEMBERS__(LongPoint2, __LongPoint2, LONG)
	__DEFINE_POINT2_STATIC_MEMBERS__(UIntPoint2, __UIntPoint2, UINT)
	__DEFINE_POINT2_STATIC_MEMBERS__(IntPoint2, __IntPoint2, INT)
}

namespace std
{
	using namespace SanyoniEngine;

	template <typename TBase, typename TElement>
	struct hash<Point2_DerivedBase<TBase, TElement>>
	{
		using TPoint2 = Point2_DerivedBase<TBase, TElement>;

		std::size_t operator()(const TPoint2& _value) const noexcept
		{
			return ((std::hash<float>()(_value.x)
				^ (std::hash<float>()(_value.y) << 1)) >> 1);
		}
	};

	template <typename TElement>
	struct hash<Point2_Base<TElement>>
	{
		using TPoint2 = Point2_Base<TElement>;

		std::size_t operator()(const TPoint2& _value) const noexcept
		{
			return ((std::hash<float>()(_value.x)
				^ (std::hash<float>()(_value.y) << 1)) >> 1);
		}
	};
}
