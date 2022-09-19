#pragma once

#define DECLARE_SINGLETON_CLASS(TClass) \
	public: \
		static TClass Instance; \
	private: \
		TClass() = default; \
		~TClass() = default; \
		TClass(const TClass& _instance) = default; \
		TClass& operator=(const TClass& _instance) {}

#define IMPLEMENT_SINGLETON_CLASS(TClass) \
		TClass TClass::Instance;

namespace SanyoniEngine
{
	template <class TClass>
	class SingletonInstanceBase
	{
	public:
		SingletonInstanceBase();

		virtual ~SingletonInstanceBase();

		static TClass* GetInstance();

	protected:
		static TClass* Instance;
	};

	template <class TClass>
	SingletonInstanceBase<TClass>::SingletonInstanceBase()
	{
		// Note: 싱글턴 타입인 TClass의 인스턴스는 프로그램 내에서 유일해야 합니다.
		if (Instance != nullptr)
		{
			assert(true);
		}

		Instance = static_cast<TClass*>(this);
	}

	template <class TClass>
	SingletonInstanceBase<TClass>::~SingletonInstanceBase()
	{
		Instance = nullptr;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::GetInstance()
	{
		return Instance;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::Instance = nullptr;
}
