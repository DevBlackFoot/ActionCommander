#pragma once

#include "_global.h"

// template <const tchar* TypeName>
// inline static std::type_index _GetTypeIndex()
// {
// 	static_assert(false, "unregistered type");
// }

// #define REGISTER_TYPE(Type) \
// 	/* TypeName */ \
// 	inline static const tchar _TypeName_##Type##_[] = TEXT(#Type); \
// 	/* TypeIndex */ \
// 	inline static const std::type_index _TypeIndex_##Type##_ = std::type_index(typeid(class Type)); \
// 	/* GetTypeIndex<Type> */ \
// 	template <> \
// 	inline static std::type_index _GetTypeIndex<TEXT(#Type)>() { return _TypeIndex_##Type##_; } \
// 	/* TypeRegister */ \
// 	struct _TypeRegister_##Type##__ { \
// 				_TypeRegister_##Type##__() { _global::GetInstance()->g_TypeMap.insert({#Type, 99}); } \
// 	}; \
// 	/* for execute TypeRegister */ \
// 	/* 위에서 정의한 struct의 static 생성자처럼 빠르게 호출하기 위해 정의한 임시 변수입니다. */\
// 	inline static _TypeRegister_##Type##__ __TypeRegister_##Type##_temp__;

// #define DEFINE_TYPE(Type) \
// 	static const tchar _TypeName_##Type##_[] = TEXT(#Type); \
// 	static const std::type_index _TypeIndex_##Type##_ = std::type_index(typeid(class Type)); \
// 	template <> \
// 	static std::type_index _GetTypeIndex<TEXT(#Type)>() { return _TypeIndex_##Type##_; } \
// 	template <> \
// 	static void* _Instantiate<TEXT(#Type)>(){ return new Type(); } \
// 	\
// 	struct _TypeRegister_##Type##__ { _TypeRegister_##Type##__() { \
// 			g_map.insert({#Type, 99}); } \
// 	}; \
// 	static _TypeRegister_##Type##__ __TypeRegister_##Type##_temp__;

/*
 * 2022. 01. 16. 03:16 AM
 * https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
 * typename string으로 그에 대응되는 type의 인스턴스를 만들 수 있는 함수를 만들고 있습니다.
 * 엔진의 사용자가 매크로로 클래스를 등록하면, 그 typename으로 인스턴스를 생성할 수 있도록 자동으로 준비를 해주는 것이 목표입니다.
 * scene을 저장하거나 툴에 사용될 수 있는 serialization과 deserialization 기능을 구현하기 위해 필요한 기능입니다.
 * 아직 시도해보지는 않았지만 boost 라이브러리를 사용하여 아예 scene 객체를 통째로 serialize/deserialize 할 생각도 하고 있습니다.
 */

// #define DEFINE_TYPE(Type) \
// 	static const tchar _TypeName_##Type##_[] = TEXT(#Type); \
// 	static const std::type_index _TypeIndex_##Type##_ = std::type_index(typeid(class Type)); \
// 	struct _TypeRegister_##Type##__ { _TypeRegister_##Type##__() { \
// 			_global::GetInstance()->g_TypeMap.insert({std::string(#Type), std::type_index(typeid(class Type)).hash_code()}); } \
// 	}; \
// 	static _TypeRegister_##Type##__ __TypeRegister_##Type##_temp__;
