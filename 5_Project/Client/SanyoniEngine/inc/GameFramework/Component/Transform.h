#pragma once

#include "ComponentBase.h"
#include "SanyoniEngine_types.h"

namespace SanyoniEngine
{
	class Transform : public ComponentBase
	{
	public:
		Transform(class GameObject* _gameObject, tstring _typeName = TEXT("Transform"));

		Transform(class GameObject* _gameObject, const Vector2& _position, float rotation, const Vector2 _scale, tstring _typeName = TEXT("Transform"));

		void UpdateWorldMatrix(Matrix3x2 _parentTM);

	private:
		void UpdateLocalMatrix();

	private:
		Vector2 m_LocalPosition;
		float m_LocalRotationRad;
		Vector2 m_LocalScale;

		/**
		 * \brief 로컬 변환 행렬입니다.
		 */
		Matrix3x2 m_LocalTM;

		/**
		 * \brief 월드 변환 행렬입니다.
		 */
		Matrix3x2 m_WorldTM;

		/**
		 * \brief 가장 마지막으로 월드 변환 행렬이 계산된 이후
		 * 값이 한 번이라도 수정되어서 변경될 필요가 있는지에 대한 판별값입니다.
		 * 이 변수는 행렬 계산을 최소한으로 줄이기 위해 사용됩니다.
		 */
		bool m_bWorldTMDirt;

		/**
		 * \brief 가장 마지막으로 월드 변환 행렬이 계산된 이후
		 * 값이 한 번이라도 수정되어서 변경될 필요가 있는지에 대한 판별값입니다.
		 * 이 변수는 행렬 계산을 최소한으로 줄이기 위해 사용됩니다.
		 */
		bool m_bLocalTMDirt;

	public:
		void SetLocalPosition(const Vector2& _newPosition);

		void SetLocalRotationRad(const float _newRotationRad);

		void SetLocalRotationDeg(const float _newRotationDeg);

		void SetLocalScale(const Vector2& _newScale);

		void SetWorldPosition(const Vector2& _newPosition);

		void SetWorldRotationRad(const float _newRotationRad);

		void SetWorldRotationDeg(const float _newRotationDeg);

		void SetWorldScale(const Vector2& _newScale);

		inline Vector2 GetLocalPosition() const { return m_LocalPosition; }

		inline float GetLocalRotationRad() const { return m_LocalRotationRad; }

		inline float GetLocalRotationDeg() const { return m_LocalRotationRad * RAD_TO_DEG; }

		inline Vector2 GetLocalScale() const { return m_LocalScale; }

		Vector2 GetWorldPosition();

		float GetWorldRotationRad();

		float GetWorldRotationDeg();

		Vector2 GetWorldScale();

		Matrix3x2 GetLocalTM();

		Matrix3x2 GetWorldTM();

		void MarkWorldTMDirt();

	private:
		static constexpr float RAD_TO_DEG = 57.295779513f;

		static constexpr float DEG_TO_RAD = 0.01745329252f;
	};
}
