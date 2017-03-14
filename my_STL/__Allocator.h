#ifndef __ALLOCATOR_H
#define __ALLOCATOR_H

#include "__Alloc.h"

namespace my_STL
{
	template <typename T>
	class allocator
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	public:
		static T *allocate();
		static T *allocate(size_t n);
		static void deallocate(T *p);
		static void deallocate(T *p, size_t n);
	};

	template <typename T>
	T *allocator<T>::allocate()
	{
		return static_cast<T *>(alloc::allocate(sizeof(T)));
	}

	template <typename T>
	T *allocator<T>::allocate(size_t n)
	{
		return static_cast<T *>(alloc::allocate(sizeof(T) * n));
	}

	template <typename T>
	void allocator<T>::deallocate(T *p)
	{
		alloc::deallocate(static_cast<void *>(p), sizeof(T));
	}

	template <typename T>
	void allocator<T>::deallocate(T *p, size_t n)
	{
		alloc::deallocate(static_cast<void *>(p), sizeof(T) * n);
	}
}
#endif // !__ALLOCATOR_H

