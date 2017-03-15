#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#include <string.h>        //memmove
#include "__Iterator.h"
#include "__Type_traits.h"

namespace my_STL
{
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
	OutputIterator fill_n(OutputIterator fitst, Size n, const T &value)
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
	//copy������
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	//copy�����1
	inline char *copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//copy�����2
	inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//__copy_dispatch������
	template <typename InputIteraot, typename OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIteraot first, InputIteraot last, OutputIterator result)
		{
			return __copy(first, last, result, iterator_category(first));
		}
	};

	//ƫ�ػ���1, T* T*
	template <typename T>
	struct __copy_dispatch<T*, T*>
	{
		T *operator()(T *first, T *last, T *result)
		{
			using t = __type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, result, t());
		}
	};

	//ƫ�ػ���2, const T* T*
	template <typename T>
	struct __copy_dispatch<const T*, T*>
	{
		T *operator()(const T *first, const T *last, T *result)
		{
			using t = __type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, result, t());
		}
	};

	//InputIterator��__copy
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag)
	{
		for (; first != last; ++result, ++first)       //����������ѭ���� �ٶ���
		{
			*result = *first;
		}
		return result;
	}

	//RandomAccessIterator��__copy
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
		for (Distance n = last - first; n > 0; --n, ++result, ++first)
		{
			*result = *first;
		}
		return result;
	}

	//trivial assignment operator�� __copy_t
	template <typename T>
	inline T *__copy_t(const T *first, const T *last, T *result, __true_type)
	{
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	//non-trivial assignment operator�� __copy_t
	template <typename T>
	inline T *__copy_t(const T *first, const T *last, T *result, __false_type)
	{
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}
	//****************************************
}
#endif // !__ALGORITHM_H

