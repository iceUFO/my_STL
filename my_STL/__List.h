#ifndef __LIST_H
#define __LIST_H

#include "__Allocator.h"
#include "__Iterator.h"

namespace my_STL
{
	template <typename T>
	struct __list_node
	{
		__list_node *prev;
		__list_node *next;
		T data;
		__list_node() :prev(nullptr), next(nullptr), data() {}
	};

	template <typename T>
	struct __list_iterator :public bidirectional_iterator<T, ptrdiff_t>
	{
		using list_node_pointer = __list_node<T>*;
		using const_list_node_pointer = const __list_node<T>*;
		list_node_pointer node;

		__list_iterator(list_node_pointer p) :node(p) {}
		__list_iterator(const_list_node_pointer &p) :node(p) {}
		__list_lterator(const __list_iterator &p) :node(p.node) {}

		bool operator==(const __list_iterator &p) const;
		bool operator!=(const __list_iterator &p) const;
		
		reference operator*() const;
		pointer operator->() const;

		__list_iterator &operator++();
		__list_iterator &operator++(int);
		__list_iterator &operator--();
		__list_iterator &operator--(int);

	};

	template <typename T, typename Alloc = allocator<T>>
	class list
	{
	protected:
		using list_node = __list_node<T>;
		using list_node_allocator = Alloc;

	public:
		using value_type = T;
		using iterator = __list_iterator<T>;
		using const_iterator = __list_iterator<const T>;
		using reference = T&;
		using size_type = size_t;

	protected:
		list_node *node;
		void empty_initialize();

	public:
		list();
		list(size_type n, const T &val);
		template <typename InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list &l);
		~list();

		//iterator
		__list_iterator begin()
	};
}

#endif // !__LIST_H

