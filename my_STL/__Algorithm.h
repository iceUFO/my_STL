#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#include <string.h>        //memmove
#include "__Iterator.h"
#include "__Type_traits.h"

namespace my_STL
{
	//max
	//****************************************
	template <typename T>
	inline const T &max(const T &a, const T &b)
	{
		return a < b ? b : a;
	}

	template <typename T, typename Compare>
	inline const T &max(const T &a, const T &b, Compare comp)
	{
		return comp(a, b) ? b : a;              //由comp决定比较标准
	}
	//****************************************

	//min
	//****************************************
	template <typename T>
	inline const T &min(const T &a, const T &b)
	{
		return b < a ? b : a;
	}

	template <typename T, typename Compare>
	inline const T &min(const T &a, const T &b, Compare comp)
	{
		return comp(b, a) ? b : a;              //由comp决定比较标准
	}
	//****************************************

	//swap
	//****************************************
	template <typename T>
	inline void swap(T &a, T &b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
	//****************************************

	//iter_swap
	//****************************************
	template <typename ForwardIterator1, typename ForwardIterator2>
	inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		typename iterator_traits<ForwardIterator1>::value_type temp = *a;
		*a = *b;
		*b = temp;
	}
	//****************************************

	//fill
	//****************************************
	template <typename ForwardIterator, typename T>
	void fill(ForwardIterator first, ForwardIterator last, const T &value)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}
	//****************************************

	//fill_n
	//****************************************
	template <typename OutputIterator, typename Size, typename T>
	OutputIterator fill_n(OutputIterator first, Size n, const T &value)
	{
		for (; n > 0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}
	//****************************************

	//copy
	//****************************************
	//copy泛化版
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	//copy特殊版1
	inline char *copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//copy特殊版2
	inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//__copy_dispatch泛化版
	template <typename InputIteraot, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIteraot first, InputIteraot last, OutputIterator result)
		{
			return __copy(first, last, result, iterator_category(first));
		}
	};

	//偏特化版1, T* T*
	template <typename T>
	struct __copy_dispatch<T*, T*>
	{
		T *operator()(T *first, T *last, T *result)
		{
			using t = typename __type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, result, t());
		}
	};

	//偏特化版2, const T* T*
	template <typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T *operator()(const T *first, const T *last, T *result)
		{
			using t = typename __type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, result, t());
		}
	};

	//InputIterator版__copy
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag)
	{
		for (; first != last; ++result, ++first)       //迭代器决定循环， 速度慢
		{
			*result = *first;
		}
		return result;
	}

	//RandomAccessIterator版__copy
	template <typename RandomAccessIterator, typename OutputIterator>
	inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, 
		OutputIterator result, random_access_iterator_tag)
	{
		return __copy_d(first, last, result, distance_type(first));
	}

	template <typename RandomAccessIterator, typename OutputIterator, typename Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, 
		OutputIterator result, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++result, ++first)  //用n决定循环，速度快
		{
			*result = *first;
		}
		return result;
	}

	//trivial assignment operator版 __copy_t
	template <typename T>
	inline T *__copy_t(const T *first, const T *last, T *result, __true_type)
	{
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	//non-trivial assignment operator版 __copy_t
	template <typename T>
	inline T *__copy_t(const T *first, const T *last, T *result, __false_type)
	{
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}
	//****************************************

	//copy_backward
	//****************************************
	//copy_backward泛化版
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, 
		BidirectionalIterator1 last, BidirectionalIterator2 result)
	{
		return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
	}

	//__copy_backward_dispatch泛化版
	template <typename BidirectionalIterator1, typename BidirectionalIterator2>
	struct __copy_backward_dispatch
	{
		BidirectionalIterator2 operator()(BidirectionalIterator1 first, 
			BidirectionalIterator1 last, BidirectionalIterator2 result)
		{
			return __copy_backward(first, last, result, iterator_category(first), distance_type(first));
		}
	};

	//BidirectionalIterator版__copy_backward
	template <typename BidirectionalIterator1, typename BidirectionalIterator2, typename Distance>
	inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first,
		BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag, Distance*)
	{
		while (first != last)
		{
			*(--result) = *(--last);
		}
		return result;
	}

	//RandomAccessIterator版__copy_backward
	template <typename RandomAccessIterator, typename BidirectionalIterator, typename Distance>
	inline BidirectionalIterator __copy_backward(RandomAccessIterator first, 
		RandomAccessIterator last, BidirectionalIterator result, Distance*)
	{
		Distance n = last - first;
		for (; n > 0; --n)
		{
			*(--result) = *(--last);
		}
		return result;
	}
	//****************************************
}
#endif // !__ALGORITHM_H

