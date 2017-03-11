#pragma once
#ifndef __ITERATOR_H
#define __ITERATOR_H

#include <cstddef>      //ptrdiff_t

namespace my_STL
{
	//迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};

	//iterator基类
	template <typename Category, typename T, typename Distance = ptrdiff_t,
			  typename Pointer = T*, typename Reference = T&>
		struct iterator
	{
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};

	//input_iterator
	template <typename T, typename Distance>
	struct input_iterator
	{
		using iterator_category = input_iterator_tag;
		using value_type = T;
		using difference_type = Distance;
		using pointer = T*;
		using reference = T&;
	};

	//output_iterator
	struct output_iterator
	{
		using iterator_category = output_iterator_tag;
		using value_type = void;
		using difference_type = void;
		using pointer = void;
		using reference = void;
	};

	//forward_iterator
	template <typename T, typename Distance>
	struct forward_iterator
	{
		using iterator_category = forward_iterator_tag;
		using value_type = T;
		using difference = Distance;
		using pointer = T*;
		using reference = T&;
	};

	//bidirectional_iterator
	template <typename T, typename Distance>
	struct bidirectional_iterator
	{
		using iterator_category = bidirectional_iterator_tag;
		using value_type = T;
		using difference = Distance;
		using pointer = T*;
		using reference = T&;
	};

	//random_access_iterator
	template <typename T, typename Distance>
	struct random_access_iterator
	{
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using difference_type = Distance;
		using pointer = T*;
		using reference = T&;
	};

	//traits
	template <typename Iterator>
	struct iterator_traits
	{
		using iterator_category = Iterator::iterator_category;
		using valu_type = Iterator::value_type;
		using difference_type = Iterator::difference_type;
		using pointer = Iterator::pointer;
		using reference = Iterator::reference;
	};

	//traits partial specialization
	template <typename T>
	struct iterator_traits<T*>
	{
		using iterator_category = random_access_iterator_tag;
		using valu_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};

	template <typename T>
	struct iterator_traits<const T*>
	{
		using iterator_category = random_access_iterator_tag;
		using valu_type = T;
		using difference_type = ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
	};

	//获取迭代器类型
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &)
	{
		using category = typename iterator_traits<Iterator>::iterator_category;
		return category();
	}

	//获取迭代器distance_type
	template <typename Iterator>
	inline typename iterator_traits<Iterator>::difference_type distance_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//获取迭代器value type
	template <typename Iterator>
	struct value_type
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	};

	//计算迭代器距离
	template <typename InputIterator>
	inline iterator_traits<InputIterator>::difference_type 
	__distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	template <typename RandomAccessIterator>
	inline iterator_traits<RandomAccessIterator>::difference_type 
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template <typename InputIterator>
	inline iterator_traits<InputIterator>::difference_type 
	distance(InputIterator first, InputIterator last)
	{
		using category = iterator_traits<InputIterator>::iterator_category;
		return __distance(first, last, category());
	}

	//迭代器移动指定距离
	template <typename InputIterator, typename Distance>
	inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
	{
		while (n--)
		{
			++i;
		}
	}

	template <typename BidirectionalIterator, typename Distance>
	inline void __advance(BidirectionalIterator &i, Distance n, bidirectional_iterator_tag)
	{
		if (n > 0)
		{
			while (n--)
				++i;
		}
		else
		{
			while (n++)
				--i;
		}
	}

	template <typename RandomAccessIterator, typename Distance>
	inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template <typename InputIterator, typename Distance>
	inline void advance(InputIterator &i, Distance n)
	{
		using category = iterator_traits<InputIterator>::iterator_category;
		advance(i, n, category(i));
	}
}

#endif // !__ITERATOR_H
