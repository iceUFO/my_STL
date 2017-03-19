#ifndef __UNINITIALIZED_H
#define __UNINITIALIZED_H

#include "__Algorithm.h"
#include "__Iterator.h"
#include "__Type_traits.h"
#include "__Construct.h"

namespace my_STL
{
	//uninitialized_copy
	//****************************************
	template <typename InputIterator, typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}

	template <typename InputIterator, typename ForwardIterator, typename V>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, V*)
	{
		using is_POD = __type_traits<V>::is_POD_type;
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	//是POD型别
	template <typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
	{
		return copy(first, last, result);
	}

	//不是POD型别
	template <typename InputIterator, typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
	{
		ForwarIterator cur = result;
		for (; first != last; ++first, ++cur)
		{
			construct(&*cur, *first);
		}
		return cur;
	}

	//特化版 const char*
	inline char *uninitialized_copy(const char* first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//特化版 const wchar_t*
	inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}
	//****************************************

	//uninitialized_fill
	//****************************************
	template <typename ForwardIterator, typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x)
	{
		__uninitialized_fill(first, last, x, value_type(first));
	}

	template <typename ForwardIterator, typename T, typename V>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, V*)
	{
		using is_POD = __type_traits<V>::is_POD_type;
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	//是POD型别
	template <typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __true_type)
	{
		fill(first, last, x);
	}

	//不是POD型别
	template <typename ForwardIterator, typename T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __false_type)
	{
		ForwardIterator cur = first;
		for (;cur != last; ++cur)
		{
			construct(&*cur, x);
		}
	}
	//****************************************

	//uninitialized_fill_n
	//****************************************
	template <typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
	{
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}

	template <typename ForwardIterator, typename Size, typename T, typename V>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, V*)
	{
		using is_POD = __type_traits<V>::is_POD_type;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	//是POD型别
	template <typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type)
	{
		return fill_n(first, n, x);
	}

	//不是POD型别
	template <typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type)
	{
		ForwardIterator cur = first;
		for (; n > 0; --n, ++cur)
		{
			construct(&*cur, x);
		}
		return cur;
	}
	//****************************************
}
#endif // !__UNINITIALIZED_H

