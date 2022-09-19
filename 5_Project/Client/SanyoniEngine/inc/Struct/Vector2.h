#pragma once

namespace SanyoniEngine
{
	struct IPoint2;

	struct IVector2
	{
	};

	template <typename TElement>
	struct Vector2_Base
	{
		TElement x;
		TElement y;
	};

	template <typename TBase, typename TElement>
	struct Vector2_DerivedBase : public TBase, public IVector2
	{
	private:
		using TVector2 = Vector2_DerivedBase<TBase, TElement>;

		static constexpr float EPSILON = 0.001f;

		template <typename T>
		static T Clamp(T _value, T _min, T _max)
		{
			if (_value < _min)
				return _min;

			if (_value > _max)
				return _max;

			return _value;
		}

	public:
		Vector2_DerivedBase() = default;

		Vector2_DerivedBase(TElement _x, TElement _y)
			: TBase{_x, _y}
		{
		}

		TVector2 operator+(TVector2 _other) const
		{
			return {this->x + _other.x, this->y + _other.y};
		}

		template <typename TPoint2>
		TVector2 operator+(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			return {this->x + _other.x, this->y + _other.y};
		}

		TVector2 operator-(TVector2 _other) const
		{
			return {this->x - _other.x, this->y - _other.y};
		}

		template <typename TPoint2>
		TVector2 operator-(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			return {this->x - _other.x, this->y - _other.y};
		}

		TVector2 operator*(float _scalar) const
		{
			return {this->x * _scalar, this->y * _scalar};
		}

		TVector2 operator*(double _scalar) const
		{
			return {this->x * _scalar, this->y * _scalar};
		}

		TVector2 operator*(int _scalar) const
		{
			return {this->x * _scalar, this->y * _scalar};
		}

		TVector2 operator/(float _divider) const
		{
			return {this->x / _divider, this->y / _divider};
		}

		TVector2 operator/(double _divider) const
		{
			return {this->x / _divider, this->y / _divider};
		}

		TVector2 operator/(int _divider) const
		{
			return {this->x / _divider, this->y / _divider};
		}

		TVector2& operator+=(const TVector2& _other)
		{
			this->x += _other.x;
			this->y += _other.y;

			return *this;
		}

		TVector2& operator-=(const TVector2& _other)
		{
			this->x -= _other.x;
			this->y -= _other.y;

			return *this;
		}

		bool operator==(const TVector2& _other) const
		{
			const bool _isXEquals = abs(this->x - _other.x) <= EPSILON;
			const bool _isYEquals = abs(this->y - _other.y) <= EPSILON;

			return _isXEquals && _isYEquals;
		}

		template <typename TPoint2>
		bool operator==(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			const bool _isXEquals = abs(this->x - _other.x) <= EPSILON;
			const bool _isYEquals = abs(this->y - _other.y) <= EPSILON;

			return _isXEquals && _isYEquals;
		}

		bool operator!=(const TVector2& _other) const
		{
			return !(*this == _other);
		}

		template <typename TPoint2>
		bool operator!=(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			return !(*this == _other);
		}

		bool operator<(const TVector2& _other) const
		{
			// x, y값 모두가 상대적으로 작다면 true를 반환합니다.
			return this->x < _other.x && this->y < _other.y;
		}

		template <typename TPoint2>
		bool operator<(const TPoint2& _other) const
		{
			// x, y값 모두가 상대적으로 작다면 true를 반환합니다.
			return this->x < _other.x && this->y < _other.y;
		}

		bool operator>(const TVector2& _other) const
		{
			// x, y값 모두가 상대적으로 크다면 true를 반환합니다.
			return this->x > _other.x && this->y > _other.y;
		}

		template <typename TPoint2>
		bool operator>(const TPoint2& _other) const
		{
			// x, y값 모두가 상대적으로 크다면 true를 반환합니다.
			return this->x > _other.x && this->y > _other.y;
		}

		bool operator<=(const TVector2& _other) const
		{
			// x, y값 모두가 상대적으로 작거나 같으면 true를 반환합니다.
			return this->x <= _other.x && this->y <= _other.y;
		}

		template <typename TPoint2>
		bool operator<=(const TPoint2& _other) const
		{
			// x, y값 모두가 상대적으로 작거나 같으면 true를 반환합니다.
			return this->x <= _other.x && this->y <= _other.y;
		}

		bool operator>=(const TVector2& _other) const
		{
			// x, y값 모두가 상대적으로 크거나 같으면 true를 반환합니다.
			return this->x >= _other.x && this->y >= _other.y;
		}

		template <typename TPoint2>
		bool operator>=(const TPoint2& _other) const
		{
			// x, y값 모두가 상대적으로 크거나 같으면 true를 반환합니다.
			return this->x >= _other.x && this->y >= _other.y;
		}

		TVector2 Normalize() const
		{
			if (*this == TVector2::Zero)
				return TVector2::Zero;

			return *this / Magnitude();
		}

		template <typename TOutValue>
		TOutValue Dot(const TVector2& _other) const
		{
			return static_cast<TOutValue>(this->x * _other.x + this->y * _other.y);
		}

		/**
		 * \brief 2차원 벡터간 외적 연산을 수행합니다. <br>
		 * note: 피연산자 2차원 벡터들을 z축이 0인 벡터로 간주하고 외적하면
		 * 반드시 {0, 0, z}인 벡터, 즉 z축의 값만 있는 벡터가 나옵니다. <br>
		 * 이 벡터의 z축 값을 반환합니다. <br>
		 * 이 함수는 주로, this 벡터를 기준으로 other 벡터가 가리키는 방향이
		 * 반시계 방향에 있는지, 또는 시계 방향에 있는지를 판별할 때 사용합니다. <br>
		 * 리턴값이 양수면 other는 this의 반시계 방향에, 음수라면 other는 this의 시계 방향에, 0이라면 평행하다는 것을 의미합니다.
		 */
		float Cross(const TVector2 _other) const
		{
			return static_cast<float>(this->x * _other.y - this->y * _other.x);
		}

		float Magnitude() const
		{
			if (*this == TVector2::Zero)
				return 0.;

			const double _powX = static_cast<double>(this->x) * static_cast<double>(this->x);
			const double _powY = static_cast<double>(this->y) * static_cast<double>(this->y);
			const double _outMagnitude = sqrt(_powX + _powY);
			return static_cast<float>(_outMagnitude);
		}

		float DistanceTo(const TVector2& _other) const
		{
			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return std::sqrt(_xDifference * _xDifference + _yDifference * _yDifference);
		}

		template <typename TPoint2>
		float DistanceTo(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return std::sqrt(_xDifference * _xDifference + _yDifference * _yDifference);
		}

		float DistanceTo_Pow(const TVector2& _other) const
		{
			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return _xDifference * _xDifference + _yDifference * _yDifference;
		}

		template <typename TPoint2>
		float DistanceTo_Pow(const TPoint2& _other) const
		{
			static_assert(std::is_base_of<IPoint2, TPoint2>::value,
				"The template parameter is not of the correct type. type must be inherited from IPoint2. Make sure you pass the correct type.");

			const float _xDifference = abs(this->x - _other.x);
			const float _yDifference = abs(this->y - _other.y);
			return _xDifference * _xDifference + _yDifference * _yDifference;
		}

	public:
		static const TVector2 Left;
		static const TVector2 Right;
		static const TVector2 Up;
		static const TVector2 Down;
		static const TVector2 RightUp;
		static const TVector2 RightDown;
		static const TVector2 LeftUp;
		static const TVector2 LeftDown;
		static const TVector2 Zero;
		static const TVector2 Center;
		static const TVector2 One;

		static TVector2 Lerp(TVector2 _origin, TVector2 _destination, float _delta)
		{
			const TVector2 _direction = _destination - _origin;
			const TVector2 _deltaVector = _direction * _delta;
			const TVector2 _outVector2D = _origin + _deltaVector;

			return _outVector2D;
		}

		static TVector2 GetPivotAppliedPosition(TVector2 _position, TVector2 _pivot, int _width, int _height)
		{
			const TElement _pivotX = Clamp(_pivot.x, 0.f, 1.f);
			const TElement _pivotY = Clamp(_pivot.y, 0.f, 1.f);

			const TElement _startXWorldPosition = _position.x - _width * _pivotX;
			const TElement _startYWorldPosition = _position.y - _height * _pivotY;

			const TVector2 _outPosition = {_startXWorldPosition, _startYWorldPosition};
			return _outPosition;
		}

		static void GetMinAndMax(TVector2& _outMin, TVector2& _outMax, TVector2 _vector1, TVector2 _vector2)
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

	using Vector2 = SanyoniEngine::Vector2_DerivedBase<D2D_VECTOR_2F, FLOAT>;
	using IntVector2 = SanyoniEngine::Vector2_DerivedBase<SanyoniEngine::Vector2_Base<int>, int>;
	using DoubleVector2 = SanyoniEngine::Vector2_DerivedBase<SanyoniEngine::Vector2_Base<double>, double>;

#define __COMMA ,

#define __Vector2			SanyoniEngine::Vector2_DerivedBase<D2D_VECTOR_2F __COMMA FLOAT>
#define __IntVector2		SanyoniEngine::Vector2_DerivedBase<SanyoniEngine::Vector2_Base<int> __COMMA int>
#define __DoubleVector2		SanyoniEngine::Vector2_DerivedBase<SanyoniEngine::Vector2_Base<double> __COMMA double>

#define __DEFINE_VECTOR2_STATIC_MEMBERS__(TVector2, TVector2Origin, TElem) \
											const TVector2 TVector2Origin::Left{static_cast<TElem>(-1), static_cast<TElem>(0)}; \
											const TVector2 TVector2Origin::Right{static_cast<TElem>(1), static_cast<TElem>(0)}; \
											const TVector2 TVector2Origin::Up{static_cast<TElem>(0), static_cast<TElem>(-1)}; \
											const TVector2 TVector2Origin::Down{static_cast<TElem>(0), static_cast<TElem>(1)}; \
											const TVector2 TVector2Origin::RightUp{static_cast<TElem>(1), static_cast<TElem>(-1)}; \
											const TVector2 TVector2Origin::RightDown{static_cast<TElem>(1), static_cast<TElem>(1)}; \
											const TVector2 TVector2Origin::LeftUp{static_cast<TElem>(-1), static_cast<TElem>(-1)}; \
											const TVector2 TVector2Origin::LeftDown{static_cast<TElem>(-1), static_cast<TElem>(1)}; \
											const TVector2 TVector2Origin::Zero{static_cast<TElem>(0), static_cast<TElem>(0)}; \
											const TVector2 TVector2Origin::Center{static_cast<TElem>(.5), static_cast<TElem>(.5)}; \
											const TVector2 TVector2Origin::One{static_cast<TElem>(1), static_cast<TElem>(1)};

	__DEFINE_VECTOR2_STATIC_MEMBERS__(Vector2, __Vector2, FLOAT)
	__DEFINE_VECTOR2_STATIC_MEMBERS__(IntVector2, __IntVector2, INT)
	__DEFINE_VECTOR2_STATIC_MEMBERS__(DoubleVector2, __DoubleVector2, DOUBLE)
}

namespace std
{
	using namespace SanyoniEngine;

	template <typename TBase, typename TElement>
	struct hash<Vector2_DerivedBase<TBase, TElement>>
	{
		using TVector2 = Vector2_DerivedBase<TBase, TElement>;

		std::size_t operator()(const TVector2& _value) const noexcept
		{
			return ((std::hash<float>()(_value.x)
				^ (std::hash<float>()(_value.y) << 1)) >> 1);
		}
	};

	template <typename TElement>
	struct hash<Vector2_Base<TElement>>
	{
		using TVector2 = Vector2_Base<TElement>;

		std::size_t operator()(const TVector2& _value) const noexcept
		{
			return ((std::hash<float>()(_value.x)
				^ (std::hash<float>()(_value.y) << 1)) >> 1);
		}
	};
}
