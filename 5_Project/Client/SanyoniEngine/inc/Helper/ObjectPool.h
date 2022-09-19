/*
 * 원본 소스코드는 다음 게시물에서 확인할 수 있습니다.
 * https://www.codeproject.com/Articles/746630/O-Object-Pool-in-Cplusplus?fid=1855889&df=90&mpp=25&sort=Position&spc=Relaxed&prof=True&view=Normal&fr=1#xx0xx
 *
 * ITEM_SIZE를 구하는 수식, ITEM_SIZE의 사용 의미 (= GetItemSize() )
 *
 * This is to make sizeof(T) aligned to sizeof(void *).
 *
 *	For example, in a 32-bit computer, where sizeof(void *) == 4, this means:
 *	if sizeof(T) == 1, the result will be 4.
 *	if sizeof(T) == 2, the result will be 4.
 *	if sizeof(T) == 3, the result will be 4.
 *	if sizeof(T) == 4, the result will be 4.
 *	if sizeof(T) == 5, the result will be 8.
 *
 *	This works because when I divide, I lose precision, then when I multiply, I got the value "rounded".
 *	But if I did only (sizeof(T) / sizeof(void *)) * sizeof(void *), values like 1, 2 and 3 will become 0, when I need it to be 4.
 *	If I used sizeof(T) + sizeof(void *) (without the -1) values like 1, 2 and 3 will be correctly, but the value 4 (for the sizeof(T)) will become 8, when it may stay at 4. This is why I use the -1.
 *
 *	Actually I saw this kind of time many, many times about 20 years ago... now it is something extremely rare.
 *
 */
#pragma once

#include <new>

namespace SanyoniEngine
{
	template <typename TObject>
	struct IObjectArrayAllocator
	{
		virtual TObject* Allocate(size_t _itemCount) abstract;

		virtual void Deallocate(TObject* pointer) abstract;

		virtual size_t GetItemSize() abstract;
	};

	template <typename TObject>
	struct DefaultObjectArrayAllocator : IObjectArrayAllocator<TObject>
	{
	public:
		TObject* Allocate(size_t _itemCount) override
		{
			return static_cast<TObject*>(
				::operator new(GetItemSize() * _itemCount, ::std::nothrow));
		}

		void Deallocate(TObject* _instanceArray) override
		{
			::operator delete(_instanceArray);
		}

		size_t GetItemSize() override
		{
			return ((sizeof(TObject) + sizeof(void*) - 1) / sizeof(void*)) * sizeof(void*);
		}
	};

	template <typename TObject, class TObjectArrayAllocator = DefaultObjectArrayAllocator<TObject>>
	class ObjectPool
	{
		static_assert(std::is_base_of<IObjectArrayAllocator<TObject>, TObjectArrayAllocator>::value,
			"The template parameter type 'TObjectArrayAllocator' must be inherited from IObjectArrayAllocator<TObject>. Make sure you pass the correct type.");

	private:
		struct Node
		{
			TObjectArrayAllocator allocator;

			/**
			 * \brief 노드 내 할당된 TObject 인스턴스 배열의 크기입니다.
			 */
			size_t capacity;

			/**
			 * \brief TObject형 인스턴스들이 할당된 배열입니다. <br>
			 * 사용자가 ObjectPool에서 오브젝트를 빌릴 때, 이 배열에서 남는
			 * note: 사용자가 ObjectPool에서 빌린 오브젝트를 반환할 때,
			 * 반환한(=원래 빌렸던) 인스턴스가 있던 지역은
			 * 이 오브젝트를 반환하기 전 가장 마지막으로 반환했던 인스턴스 메모리 주소를 저장하는 데 사용됩니다. <br>
			 */
			TObject* instanceArray;

			/**
			 * \brief 다음 노드의 포인터입니다.
			 */
			Node* nextNode;

			/**
			 * \brief Node의 생성자입니다.
			 * 만약 capacity가 1 이상이 아니거나, TObject 인스턴스 배열 할당에 실패했다면 예외를 발생시킵니다.
			 */
			Node(size_t _capacity, TObjectArrayAllocator _allocator) :
				allocator(_allocator),
				capacity(_capacity),
				nextNode(nullptr)
			{
				if (_capacity < 1)
					throw std::invalid_argument("_capacity must be at least 1.");

				instanceArray = _allocator.Allocate(_capacity);

				if (instanceArray == nullptr)
					throw std::bad_alloc();
			}

			~Node()
			{
				allocator.Deallocate(instanceArray);
			}
		};

	public:
		explicit ObjectPool(TObjectArrayAllocator _allocator = TObjectArrayAllocator(), size_t _initialCapacity = 32, size_t _maxBlockLength = 1000000) :
			m_Allocator(_allocator),
			m_LastRestoredInstance(nullptr),
			m_LastUsedInstanceIndexInNode(0),
			m_InstanceCapacity(_initialCapacity),
			m_FirstNode(_initialCapacity, _allocator),
			m_MaxNodeSize(_maxBlockLength)
		{
			if (_maxBlockLength < 1)
				throw std::invalid_argument("_maxBlockLength must be at least 1.");

			m_LastNode = &m_FirstNode;
		}

		~ObjectPool()
		{
			Node* _node = m_FirstNode.nextNode;
			while (_node != nullptr)
			{
				Node* _nextNode = _node->nextNode;
				delete _node;
				_node = _nextNode;
			}
		}

		template <typename ... Args>
		TObject* Get(Args ... _args)
		{
			TObject* _outInstanceAddress;

			// 중간에 반환되어 쉬고 있는 인스턴스가 있다면,
			// 인덱스 위치에 있는 인스턴스를 반환하지 않고 쉬고 있는 중간 인스턴스를 반환합니다.
			if (m_LastRestoredInstance)
			{
				_outInstanceAddress = m_LastRestoredInstance;

				// 가장 마지막으로 반환된 인스턴스 포인터를
				// 그 이전으로 반환된 인스턴스의 주소값으로 덮어씁니다.
				m_LastRestoredInstance = reinterpret_cast<TObject*>(*m_LastRestoredInstance);
			}
			else
			{
				// 모든 인스턴스가 사용중인 경우,
				// 먼저 인스턴스를 새로 할당하여 pool의 크기를 늘립니다.
				if (m_LastUsedInstanceIndexInNode >= m_LastNode->capacity)
					AllocateNewNode();


				// 반환할 인스턴스의 주소를 계산합니다.
				_outInstanceAddress = reinterpret_cast<TObject*>(
					reinterpret_cast<size_t>(m_LastNode->instanceArray) +
					m_LastUsedInstanceIndexInNode * m_Allocator.GetItemSize()
				);

				// 인덱스를 한 칸 뒤로 이동시킵니다.
				++m_LastUsedInstanceIndexInNode;
			}


			// 생성자 호출 옵션이 활성화되었다면, 생성자를 호출한 뒤 반환합니다.
			new(_outInstanceAddress) TObject(_args ...);
			return _outInstanceAddress;
		}
		
		TObject* GetWithoutConstructor()
		{
			TObject* _outInstanceAddress;

			// 중간에 반환되어 쉬고 있는 인스턴스가 있다면,
			// 인덱스 위치에 있는 인스턴스를 반환하지 않고 쉬고 있는 중간 인스턴스를 반환합니다.
			if (m_LastRestoredInstance)
			{
				_outInstanceAddress = m_LastRestoredInstance;

				// 가장 마지막으로 반환된 인스턴스 포인터를
				// 그 이전으로 반환된 인스턴스의 주소값으로 덮어씁니다.
				m_LastRestoredInstance = reinterpret_cast<TObject*>(*m_LastRestoredInstance);
			}
			else
			{
				// 모든 인스턴스가 사용중인 경우,
				// 먼저 인스턴스를 새로 할당하여 pool의 크기를 늘립니다.
				if (m_LastUsedInstanceIndexInNode >= m_LastNode->capacity)
					AllocateNewNode();


				// 반환할 인스턴스의 주소를 계산합니다.
				_outInstanceAddress = reinterpret_cast<TObject*>(
					reinterpret_cast<size_t>(m_LastNode->instanceArray) +
					m_LastUsedInstanceIndexInNode * m_Allocator.GetItemSize()
					);

				// 인덱스를 한 칸 뒤로 이동시킵니다.
				++m_LastUsedInstanceIndexInNode;
			}

			return _outInstanceAddress;
		}

		void Restore(TObject* content, bool _callDestructor = true)
		{
			if (_callDestructor == true)
				content->~T();

			// 원래 인스턴스의 영역에 가장 마지막으로 반환했던 인스턴스의 주소를 저장합니다.
			*content = reinterpret_cast<TObject>(m_LastRestoredInstance);

			// 그 뒤, 가장 마지막으로 반환한 인스턴스의 주소는 지금 반환한 인스턴스의 주소로 갱신합니다.
			m_LastRestoredInstance = content;
		}

		void RestoreWithoutDestructor(TObject* content)
		{
			// 원래 인스턴스의 영역에 가장 마지막으로 반환했던 인스턴스의 주소를 저장합니다.
			*content = reinterpret_cast<TObject>(m_LastRestoredInstance);

			// 그 뒤, 가장 마지막으로 반환한 인스턴스의 주소는 지금 반환한 인스턴스의 주소로 갱신합니다.
			m_LastRestoredInstance = content;
		}

	private:
		// ObjectPool 객체를 복사할 수 없도록 막습니다.
		ObjectPool(const ObjectPool<TObject, TObjectArrayAllocator>& source);
		void operator =(const ObjectPool<TObject, TObjectArrayAllocator>& source);

		/**
		 * \brief 이 함수는 지금까지 할당된 모든 Node들의 인스턴스가 모두 사용중일 때, Pool의 사이즈를 늘리기 위해 사용됩니다. <br>
		 * 새로운 Node 인스턴스를 생성하고, 바로 이후에 있을 인스턴스 반환에, 이 Node의 인스턴스 배열의 첫 번째 인스턴스를 반환할 수 있도록 준비합니다.
		 */
		void AllocateNewNode()
		{
			size_t _newNodeSize = m_LastUsedInstanceIndexInNode;

			if (_newNodeSize >= m_MaxNodeSize)
				_newNodeSize = m_MaxNodeSize;
			else
			{
				_newNodeSize *= 2;

				if (_newNodeSize < m_LastUsedInstanceIndexInNode)
					throw std::overflow_error("size became too big.");

				if (_newNodeSize >= m_MaxNodeSize)
					_newNodeSize = m_MaxNodeSize;
			}

			Node* newNode = new Node(_newNodeSize, m_Allocator);
			m_LastNode->nextNode = newNode;
			m_LastNode = newNode;
			m_LastUsedInstanceIndexInNode = 0;
			m_InstanceCapacity = _newNodeSize;
		}

	private:
		TObjectArrayAllocator m_Allocator;

		/**
		 * \brief 가장 마지막으로 반환된 인스턴스의 포인터입니다. <br>
		 * note: 이 포인터가 가리키는 인스턴스 영역에는 이 인스턴스가 반환되기 전에 가장 마지막으로 반환되었던 인스턴스의 주소를 가지고 있습니다. <br>
		 * 또 그 인스턴스는 자기가 반환되기 전 가장 마지막으로 반환되었던 인스턴스의 주소를 저장하고 있습니다. <br>
		 * 연쇄적으로 가지고 있으며 가장 처음에 반환된 인스턴스는 nullptr 값을 가지고 있습니다. <br>
		 * 만약, Pool 내에서 사용중인 인스턴스들이 중간에 빈틈 없이 모두 사용중이라면, 이 값은 nullptr을 가집니다.
		 */
		TObject* m_LastRestoredInstance;

		/**
		 * \brief 지금까지 반환한 인스턴스 중 가장 마지막에 위치한 인스턴스의 노드 내에서의 인덱스를 반환합니다. <br>
		 * e.g Pool이 생성된 뒤 사용자가 다음 순서대로 인스턴스를 요청하고 반환하기를 했다고 가정해봅시다. <br>
		 * 1. 인스턴스를 3번 요청했습니다. <br>
		 * 2. 반환받은 인스턴스를 1번 반환했습니다. <br>
		 * 3. 인스턴스를 다시 3번 요청했습니다. <br>
		 * 그렇게 된다면, 이 값은 5가 될 것입니다.
		 */
		size_t m_LastUsedInstanceIndexInNode;

		/**
		 * \brief Pool에 할당된 모든 인스턴스 배열들의 총 용량입니다. <br>
		 * i.e 모든 Node들의 인스턴스 배열의 capacity의 총 합입니다.
		 */
		size_t m_InstanceCapacity;

		/**
		 * \brief Pool이 생성될 때 처음으로 생성되는 시작 Node 인스턴스 입니다. <br>
		 */
		Node m_FirstNode;

		/**
		 * \brief Pool에서 생성된 Node 중 가장 마지막 Node의 포인터입니다. <br>
		 */
		Node* m_LastNode;

		/**
		 * \brief Pool에서 Node가 생성될 때 Node의 최대 크기 제한 값입니다. (byte)
		 */
		size_t m_MaxNodeSize;
	};
}
