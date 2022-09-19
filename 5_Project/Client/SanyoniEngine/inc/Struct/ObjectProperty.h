#pragma once

#include "SanyoniEngine_types.h"
#include "SingletonInstanceBase.h"

namespace SanyoniEngine
{
	class ObjectUIDGenerator
	{
	public:
		inline static int Generate() { return Counter++; }

	private:
		inline static int Counter = 0;
	};

	class ObjectUIDInTypeGenerator
	{
		DECLARE_SINGLETON_CLASS(ObjectUIDInTypeGenerator)

	public:
		inline int Generate(tstring _typeName)
		{
			if (const auto _iter = ObjectTypeCounterMap.find(_typeName);
				_iter != ObjectTypeCounterMap.end())
			{
				return ++ObjectTypeCounterMap[_typeName];
			}

			ObjectTypeCounterMap.insert({_typeName, 0});
			return 0;
		}

	private:
		std::map<tstring, int> ObjectTypeCounterMap;
	};

	class ObjectProperty
	{
	public:
		ObjectProperty(tstring _typeName) :
			m_UID(ObjectUIDGenerator::Generate()),
			m_UIDInType(ObjectUIDInTypeGenerator::Instance.Generate(_typeName))
		{
			m_Name = StringHelper::Format(TEXT("%s_%d"), _typeName.c_str(), m_UIDInType);
		}

	private:
		int m_UID;

		int m_UIDInType;

		tstring m_Name;

	public:
		inline int GetUID() const { return m_UID; }

		inline tstring GetName() const { return m_Name; }

		inline void SetName(tstring _name) { m_Name = _name; }
	};
}
