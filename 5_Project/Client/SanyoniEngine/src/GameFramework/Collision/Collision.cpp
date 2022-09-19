#include "SanyoniEngine_pch.h"
#include "Collision.h"

#include "AABBCollider.h"
#include "CircleCollider.h"
#include "Helper.h"

using namespace SanyoniEngine::Collision;

bool AABB::OverlapAABB(const AABBBounds& _lhs, const AABBBounds& _rhs)
{
	const bool _notOverlapX =
		_lhs.GetRight() < _rhs.GetLeft()
		|| _lhs.GetLeft() > _rhs.GetRight();

	if (_notOverlapX == true)
		return false;

	const bool _notOverlapY =
		_lhs.GetBottom() < _rhs.GetTop()
		|| _lhs.GetTop() > _rhs.GetBottom();

	if (_notOverlapY == true)
		return false;

	return true;
}

bool AABB::OverlapCircle(const AABBBounds& _box, const CircleBounds& _circle)
{
	const RectZoneCase _zoneCase = GetRectZone(_box, _circle);

	float _distance = 0;

	switch (_zoneCase)
	{
		// 원의 원점이 사각형 내부에 있는 경우입니다.
	case RectZoneCase::CENTER:
		return true;

		// 원의 원점이 사각형의 위 또는 아래에 있는 경우입니다.
	case RectZoneCase::BOTTOM:
		_distance = abs(_circle.GetCenter().y - _box.GetBottom());
		break;

	case RectZoneCase::TOP:
		_distance = abs(_circle.GetCenter().y - _box.GetTop());
		break;

		// 원의 원점이 사각형의 왼쪽 또는 오른쪽에 있는 경우입니다.
	case RectZoneCase::LEFT:
		_distance = abs(_circle.GetCenter().x - _box.GetLeft());
		break;

	case RectZoneCase::RIGHT:
		_distance = abs(_circle.GetCenter().x - _box.GetRight());
		break;

		// 원의 원점이 사각형의 대각선 방향에 있는 경우입니다.
	default:
		{
			const int _zoneCaseValue = static_cast<int>(_zoneCase);
			const float _nearestPointX = (_zoneCaseValue & RectZoneCase_LeftBit) ? _box.GetLeft() : _box.GetRight();
			const float _nearestPointY = (_zoneCaseValue & RectZoneCase_TopBit) ? _box.GetTop() : _box.GetBottom();
			const float _distanceX = abs(_nearestPointX - _circle.GetCenter().x);
			const float _distanceY = abs(_nearestPointY - _circle.GetCenter().y);
			const float _distancePow = _distanceX * _distanceX + _distanceY * _distanceY;

			const bool _isCollided = _distancePow <= _circle.GetRadiusPow();
			return _isCollided;
		}
	}

	if (_distance <= _circle.GetRadius())
		return true;

	return false;
}

bool AABB::Overlap(const AABBBounds& _box, ColliderBase* _collider)
{
	bool _isOverlap = false;

	if (const auto _aabbCollider = dynamic_cast<AABBCollider*>(_collider);
		_aabbCollider != nullptr)
	{
		_isOverlap = OverlapAABB(_box, _aabbCollider->GetAABBBounds());
	}
	else if (const auto _circleCollider = dynamic_cast<CircleCollider*>(_collider);
		_circleCollider != nullptr)
	{
		_isOverlap = OverlapCircle(_box, _circleCollider->GetBounds());
	}

	return _isOverlap;
}

AABB::RectZoneCase AABB::GetRectZone(const AABBBounds& _box, const CircleBounds& _circle)
{
	const int _zoneX = (_circle.GetCenter().x < _box.GetLeft())
		                   ? RectZoneCase_LeftBit
		                   : (_circle.GetCenter().x > _box.GetRight())
		                   ? RectZoneCase_RightBit
		                   : 0;

	// direct2d에서는 원점이 윈도우의 최상단 왼쪽이기 때문에,
	// y값이 더 높을 수록 더 아래에 위치합니다.
	const int _zoneY = (_circle.GetCenter().y > _box.GetBottom())
		                   ? RectZoneCase_BottomBit
		                   : (_circle.GetCenter().y < _box.GetTop())
		                   ? RectZoneCase_TopBit
		                   : 0;

	const RectZoneCase _zone = static_cast<RectZoneCase>(_zoneX | _zoneY);
	return _zone;
}

bool OBB::SimpleBoxAndBox(const OBBBounds& _lhs, const OBBBounds& _rhs)
{
	static const std::function _isNonCollidedAxis = [](const OBBBounds& _lhs, const OBBBounds& _rhs,
	                                                   const Vector2& _lhsAxisStart, const Vector2& _lhsAxisEnd)-> bool
	{
		const Vector2 _lhsAxis = _lhsAxisEnd - _lhsAxisStart;

		const float _lhsDotAxisStart = _lhsAxis.Dot<float>(_lhsAxisStart);
		float _lhsMin = _lhsDotAxisStart;
		float _lhsMax = _lhsDotAxisStart;

		const float _lhsDotAxisEnd = _lhsAxis.Dot<float>(_lhsAxisEnd);
		if (_lhsMin > _lhsDotAxisEnd) _lhsMin = _lhsDotAxisEnd;
		else _lhsMax = _lhsDotAxisEnd;

		// rhs의 각 꼭짓점과 axis를 내적한 결과에서 최소값과 최대값을 구합니다.
		const float _rhsDotLeftTop = _lhsAxis.Dot<float>(_rhs.GetLeftTop());
		float _rhsMin = _rhsDotLeftTop;
		float _rhsMax = _rhsDotLeftTop;

		const float _rhsDotRightTop = _lhsAxis.Dot<float>(_rhs.GetRightTop());
		if (_rhsMin > _rhsDotRightTop) _rhsMin = _rhsDotRightTop;
		else if (_rhsMax < _rhsDotRightTop) _rhsMax = _rhsDotRightTop;

		const float _rhsDotRightBottom = _lhsAxis.Dot<float>(_rhs.GetRightBottom());
		if (_rhsMin > _rhsDotRightBottom) _rhsMin = _rhsDotRightBottom;
		else if (_rhsMax < _rhsDotRightBottom) _rhsMax = _rhsDotRightBottom;

		const float _rhsDotLeftBottom = _lhsAxis.Dot<float>(_rhs.GetLeftBottom());
		if (_rhsMin > _rhsDotLeftBottom) _rhsMin = _rhsDotLeftBottom;
		else if (_rhsMax < _rhsDotLeftBottom) _rhsMax = _rhsDotLeftBottom;

		const bool _isCollidedAxis =
			// _rhsMin이 _lhs 영역에 들어왔는지 확인합니다.
			(_lhsMin <= _rhsMin && _rhsMin <= _lhsMax)
			// 또는 _rhsMax이 _lhs 영역에 들어왔는지 확인합니다.
			|| (_lhsMin <= _rhsMax && _rhsMax <= _lhsMax);

		return !_isCollidedAxis;
	};

	// 각 사각형의 빗변 방향을 축으로 두 사각형의 꼭짓점을 내적한 값들을 각각 구하고,
	// 그 값들의 min / max를 각각의 사각형으로부터 구합니다.
	// _lhsMin과 _lhsMax, 그리고 _rhsMin과 _rhsMax의 값의 범위가 곂치는지 확인합니다.
	// 곂치지 않는다면, 충돌하지 않는 축을 찾아낸 것입니다.
	if (_isNonCollidedAxis(_lhs, _rhs, _lhs.GetLeftTop(), _lhs.GetRightTop()) == true)
		return false;

	if (_isNonCollidedAxis(_lhs, _rhs, _lhs.GetRightTop(), _lhs.GetRightBottom()) == true)
		return false;

	if (_isNonCollidedAxis(_rhs, _lhs, _rhs.GetLeftTop(), _rhs.GetRightTop()) == true)
		return false;

	if (_isNonCollidedAxis(_rhs, _lhs, _rhs.GetRightTop(), _rhs.GetRightBottom()) == true)
		return false;

	return true;
}

bool Circle::OverlapCircle(const CircleBounds& _lhs, const CircleBounds& _rhs)
{
	const float _distance = _lhs.GetCenter().DistanceTo(_rhs.GetCenter());
	const bool _overlap = _lhs.GetRadius() + _rhs.GetRadius() >= _distance;

	return _overlap;
}

bool Circle::OverlapAABB(const CircleBounds& _circle, const AABBBounds& _box)
{
	return AABB::OverlapCircle(_box, _circle);
}

bool Circle::Overlap(const CircleBounds& _circle, ColliderBase* _collider)
{
	bool _isOverlap = false;

	if (const auto _aabbCollider = dynamic_cast<AABBCollider*>(_collider);
		_aabbCollider != nullptr)
	{
		_isOverlap = OverlapAABB(_circle, _aabbCollider->GetAABBBounds());
	}
	else if (const auto _circleCollider = dynamic_cast<CircleCollider*>(_collider);
		_circleCollider != nullptr)
	{
		_isOverlap = OverlapCircle(_circle, _circleCollider->GetBounds());
	}

	return _isOverlap;
}

bool Ray::RayIntersection(const RayBounds& _lhs, const RayBounds& _rhs, Vector2& _outPosition, float& _outDistance)
{
	// a_x + p b_x = c_x + q d_x
	// a_y + p b_y = c_y + q d_y
	// 직선 위의 두 점을 연립하여, p를 구할 수 있는지 판별하고, 있다면 p를 원래 식에 대입하여 교점을 얻습니다.
	// p를 구할 수 없다면 두 직선은 교점이 없다는 것을 의미합니다.

	// 본 코드에서는 이를 외적으로 깔끔하게 정리한 형태로 계산합니다.
	const auto _cMinusA = _rhs.GetStart() - _lhs.GetStart();
	const float _det = _rhs.GetDirection().Cross(_lhs.GetDirection());

	// 만약 두 Ray의 기울기가 같다면, 교점이 없다고 간주합니다.
	if (MathHelper::AlmostEquals(_det, 0) == true)
		return false;

	// _lhs.Start로부터
	// 두 Ray의 교점까지의 거리가 어느정도인지 얻습니다.
	float _p = _rhs.GetDirection().Cross(_cMinusA) / _det;
	float _q = _lhs.GetDirection().Cross(_cMinusA) / _det;

	// 부동소수점 오차에 의해, 충돌했지만 충돌 거리가 미세한 음수값이 나왔을 수 있습니다.
	// 따라서 거의 근사하다면, ray와 부딪힌 지점이라 간주합니다.
	// 그렇기 때문에, 교점으로 판단하기 전에 0에 근사한지 아닌지를 판별합니다.

	if (MathHelper::AlmostEquals(_p, 0) == true)
		_p = 0;

	if (MathHelper::AlmostEquals(_q, 0) == true)
		_q = 0;

	// 만약 Ray의 길이로부터 벗어난 지점이라면, 교점이 아닙니다.

	if (_p < 0 || _lhs.GetLength() < _p)
		return false;

	if (_q < 0 || _rhs.GetLength() < _q)
		return false;

	// 교점을 반환합니다.
	_outPosition = _lhs.GetStart() + _lhs.GetDirection() * _p;
	_outDistance = _p;
	return true;
}

bool Ray::RaycastAABB(const RayBounds& _ray, const AABBBounds& _box, RaycastHitInfo& _outHit)
{
	Vector2 _hitPoint;

	const RayBounds _boxSideSegments[4] = {
		{_box.GetLeftTop(), _box.GetRightTop()},
		{_box.GetRightTop(), _box.GetRightBottom()},
		{_box.GetRightBottom(), _box.GetLeftBottom()},
		{_box.GetLeftBottom(), _box.GetLeftTop()}
	};

	float _lowerHitDistance = std::numeric_limits<float>::infinity();
	bool _collision = false;

	for (const auto& _segment : _boxSideSegments)
	{
		float _hitDistance;

		if (RayIntersection(_ray, _segment, _hitPoint, _hitDistance) == true)
		{
			// 지금까지 검사했던 사각형의 변들 중, 더 가까운 변과 이미 교차했었다면 건너뜁니다.
			if (_hitDistance > _lowerHitDistance)
				continue;

			_lowerHitDistance = _hitDistance;
			_collision = true;

			_outHit.start = _ray.GetStart();
			_outHit.direction = _ray.GetDirection();
			_outHit.distance = (_hitPoint - _ray.GetStart()).Magnitude();
			_outHit.point = _hitPoint;
		}
	}

	return _collision;
}

bool Ray::RaycastCircle(const RayBounds& _ray, const CircleBounds& _circle, RaycastHitInfo& _outHit)
{
	// 원과 직선의 연립방정식을 전개하고 정리하여
	// ax^2 + bx + c = 0 꼴로 만들었을 때, 각각의 a, b, c값을 구합니다.
	// 이것들은 아래 판별식을 구할 때 사용되는 값입니다.
	const float _a = _ray.GetDirection().x * _ray.GetDirection().x + _ray.GetDirection().y * _ray.GetDirection().y;
	const float _b =
		2 * (_ray.GetStart().x * _ray.GetDirection().x + _ray.GetStart().y * _ray.GetDirection().y
			- _circle.GetCenter().x * _ray.GetDirection().x - _circle.GetCenter().y * _ray.GetDirection().y);
	const float _c =
		(_ray.GetStart().x * _ray.GetStart().x) + (_ray.GetStart().y * _ray.GetStart().y)
		- (2 * _circle.GetCenter().x * _ray.GetStart().x) - (2 * _circle.GetCenter().y * _ray.GetStart().y)
		+ (_circle.GetCenter().x * _circle.GetCenter().x) + (_circle.GetCenter().y * _circle.GetCenter().y)
		- _circle.GetRadiusPow();


	// 판별식을 계산하여 원과 직선의 연립방정식이 실근을 가지는지 확인합니다.
	// 실근을 가진다는 것은 곧, 원과 직선이 만나는 교점이 있다는 것을 의미합니다.
	//   판별식의 값이 0이라는 것은 하나의 실근을 가진다는 것을 의미합니다.
	//   판별식의 값이 0보다 크다는 것은 서로 다른 실근을 가진다는 것을 의미합니다.
	//   판별식의 값이 0보다 작다면 서로 다른 허근을 가진다는 것을 의미합니다.
	const float _discriminant = (_b * _b) - (4 * _a * _c);

	// 교점이 있을 때, 그 교점이 실제로 선분의 시작점에서 거리 내에 있는지 확인합니다.
	// 구하는 값 t는 선의 중심점에서 선의 방향으로 얼만큼 이동한 값인지에 대한 오프셋입니다.
	bool _isHit = false;
	float _hitDistance = 1;

	if (MathHelper::AlmostEquals(_discriminant, 0) == true)
	{
		const float _sqrtDiscriminant = sqrt(_discriminant);
		const float _t = (-_b + _sqrtDiscriminant) / (2 * _a);
		_isHit = MathHelper::InRange(_t, 0, _ray.GetLength());

		_hitDistance = _t;
	}
	else if (_discriminant > 0)
	{
		const float _sqrtDiscriminant = sqrt(_discriminant);
		const float _t1 = (-_b + _sqrtDiscriminant) / (2 * _a);
		const float _t2 = (-_b - _sqrtDiscriminant) / (2 * _a);

		// _t2는 항상 _t1보다 작은 값입니다.
		// 이럴 때, _t2가 0보다 작다는 것은, ray의 접점 중 한 점은 ray 방향의 뒤쪽에 있다는 것을 의미합니다.
		// 다른 말로, ray의 시작점이 원형 안에 있다는 것을 의미합니다.
		// 그렇기 때문에 ray의 앞쪽에 있는 교점과의 거리인 _t1을 반환합니다.
		// 만약 _t2가 0보다 큰 경우, ray와 원의 교점 두 개 모두 ray의 앞쪽에 있다는 것이므로
		// 더 가까운 교점과의 거리인 _t2를 반환합니다.
		_hitDistance = _t2 < 0 ? _t1 : _t2;

		_isHit = MathHelper::InRange(_hitDistance, 0, _ray.GetLength());
	}

	if (_isHit == true)
	{
		_outHit.start = _ray.GetStart();
		_outHit.direction = _ray.GetDirection();
		_outHit.distance = _hitDistance;
		_outHit.point = _ray.GetStart() + _ray.GetDirection() * _hitDistance;

		return true;
	}

	return false;
}

bool Ray::Raycast(const RayBounds& _ray, ColliderBase* _collider, RaycastHitInfo& _outHit)
{
	// 예외적으로 ray의 길이가 정확히 0과 같을 경우
	// 더 이상 검사를 진행하지 않습니다.
	if (_ray.GetLength() == 0)
		return false;

	bool _isHit;

	if (const auto _aabbCollider = dynamic_cast<AABBCollider*>(_collider);
		_aabbCollider != nullptr)
	{
		_isHit = RaycastAABB(_ray, _aabbCollider->GetAABBBounds(), _outHit);
	}
	else if (const auto _circleCollider = dynamic_cast<CircleCollider*>(_collider);
		_circleCollider != nullptr)
	{
		_isHit = RaycastCircle(_ray, _circleCollider->GetBounds(), _outHit);
	}

	_outHit.collider = _isHit == true ? _collider : nullptr;

	return _isHit;
}
