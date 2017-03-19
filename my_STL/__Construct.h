#ifndef __CONSTRUCT_H
#define __CONSTRUCT_H

#include <new>            //placement new
#include "__Type_traits.h"

namespace my_STL
{
	//construct
	template <typename T1, typename T2>
	inline void construct(T1 *p, const T2 &value)
	{
		new(p) T1(value);
	}

	//destroy
	template <typename T>
	inline void destroy(T *pointer)
	{
		pointer->~T();
	}

	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		__destroy(first, last, value_type(first));
	}

	//ÅÐ¶ÏÐÍ±ð
	template <typename ForwardIterator, typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		using trivial_destructor = typename __type_traits<T>::has_trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	//has trivial destructor (false)
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for ( ; first < last; ++first)
		{
			destroy(&*first);
		}
	}

	//has trivial destructor (true)
	template <typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) { }

}

#endif // !__CONSTRUCT_H
