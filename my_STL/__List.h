#ifndef __LIST_H
#define __LIST_H

#include<utility>
#include "__Allocator.h"
#include "__Construct.h"
#include "__Iterator.h"
#include "__Algorithm.h"

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
	struct __list_iterator :public iterator<bidirectional_iterator_tag, T>
	{
		template<typename T, typename Alloc>
		friend class list;
		using list_node_ptr = __list_node<T>*;
		using const_list_node_ptr = const __list_node<T>*;
		list_node_ptr node;

		__list_iterator(list_node_ptr p = nullptr) :node(p) {}
		__list_iterator(const_list_node_ptr p) :node(p) {}
		__list_iterator(const __list_iterator &p) :node(p.node) {}

		bool operator==(const __list_iterator &p) const;
		bool operator!=(const __list_iterator &p) const;
		
		T& operator*() const;
		T* operator->() const;

		__list_iterator &operator++();
		__list_iterator operator++(int);
		__list_iterator &operator--();
		__list_iterator operator--(int);

	};

	template <typename T, typename Alloc = allocator<__list_node<T>>>
	class list
	{
		template<typename T>
		friend struct __list_iterator;
	protected:
		using list_node = __list_node<T>;
		using list_node_ptr = __list_node<T>*;
		using list_node_allocator = Alloc;

	public:
		using value_type = T;
		using iterator = __list_iterator<T>;
		using const_iterator = const __list_iterator<T>;
		using reference = T&;
		using size_type = size_t;

	protected:
		list_node *node;
		void empty_initialize();

	protected:
		//配置节点
		list_node* get_node()
		{
			return list_node_allocator::allocate();
		}

		//释放节点
		void put_node(list_node *p)
		{
			list_node_allocator::deallocate(p);
		}

		//产生、销毁节点
		list_node* create_node(const T &value);
		void destroy_node(list_node *p);

	public:
		list();
		list(size_type n, const T &val);
		template <typename InputIterator>
		list(InputIterator *first, InputIterator *last);
		list(const list &l);
		list(std::initializer_list<T> il);
		list &operator=(const list &rhs);
		~list();

		//Iterator
		iterator begin()
		{
			return node->next;
		}

		const_iterator begin() const
		{
			return node->next;
		}

		iterator end()
		{
			return node;
		}

		const_iterator end() const
		{
			return node;
		}

		//Capacity
		bool empty() const
		{
			return node->next == node;
		}

		size_type size() const
		{
			return my_STL::distance(begin(), end());
		}

		//Element access
		reference front()
		{
			return *begin();
		}

		reference back()
		{
			return *(--end());
		}

		//Modifiers
		void clear();
		iterator insert(iterator pos, const T& value);
		void insert(iterator pos, size_type count, const T &value);
		template<typename InputIterator>
		void insert(iterator pos, InputIterator *first, InputIterator *last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void push_front(const T &val);
		void push_back(const T &val);

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

		void swap(list &l)
		{
			my_STL::swap(node, l.node);
		}

		//Operations
	protected:
		void transfer(iterator pos, iterator first, iterator last);
	public:
		void splice(iterator pos, list &other);
		void splice(iterator pos, list &other, iterator i);
		void splice(iterator pos, list &other, iterator first, iterator last);
		void merge(list &other);
		template <typename Compare>
		void merge(list &other, Compare comp);
		void reverse();
		void remove(const T &value);
		template <typename Predicate>
		void remove_if(Predicate p);
		void unique();
		void sort();

		template <typename T>
		friend bool operator==(const list<T> &lhs, const list<T> &rhs);
		template <typename T>
		friend bool operator!=(const list<T> &lhs, const list<T> &rhs);

	};

	/***********************************************************************/
	//__list_iterator
	/***********************************************************************/
	template<typename T>
	bool __list_iterator<T>::operator==(const __list_iterator &p) const
	{
		return node == p.node;
	}
	template<typename T>
	bool __list_iterator<T>::operator!=(const __list_iterator & p) const
	{
		return node != p.node;
	}
	template<typename T>
	T& __list_iterator<T>::operator*() const
	{
		return node->data;
	}
	template<typename T>
	T* __list_iterator<T>::operator->() const
	{
		return &(operator*());
	}
	template<typename T>
	__list_iterator<T>& __list_iterator<T>::operator++()
	{
		node = node->next;
		return *this;
	}
	template<typename T>
	__list_iterator<T> __list_iterator<T>::operator++(int)
	{
		auto temp = *this;
		++*this;
		return temp;
	}
	template<typename T>
	__list_iterator<T>& __list_iterator<T>::operator--()
	{
		node = node->prev;
		return *this;
	}
	template<typename T>
	__list_iterator<T> __list_iterator<T>::operator--(int)
	{
		auto temp = *this;
		--*this;
		return temp;
	}

	/***********************************************************************/
	//list
	/***********************************************************************/
	template<typename T, typename Alloc>
	inline void list<T, Alloc>::empty_initialize()
	{
		node = get_node();
		node->next = node;
		node->prev = node;
	}

	template<typename T, typename Alloc>
	inline typename list<T, Alloc>::list_node* list<T, Alloc>::create_node(const T &value)
	{
		list_node *p = get_node();
		construct(&p->data, value);
		return p;
	}

	template<typename T, typename Alloc>
	inline void list<T, Alloc>::destroy_node(list_node *p)
	{
		destroy(&p->data);
		put_node(p);
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::list()
	{
		empty_initialize();
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::list(size_type n, const T & val)
	{
		empty_initialize();
		while (n--)
		{
			push_back(val);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	list<T, Alloc>::list(InputIterator *first, InputIterator *last)
	{
		empty_initialize();
		while (first != last)
		{
			push_back(*first);
			++first;
		}
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::list(const list &l)
	{

		empty_initialize();
		iterator first = l.begin();
		iterator last = l.end();
		while (first != last)
		{
			push_back(*first);
			++first;
		}
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::list(std::initializer_list<T> il)
	{
		empty_initialize();
		for (auto i : il)
		{
			push_back(i);
		}
	}

	template<typename T, typename Alloc>
	list<T, Alloc> &list<T, Alloc>::operator=(const list &rhs)
	{
		if (this != &rhs)
		{
			clear();
			iterator first = rhs.begin();
			iterator last = rhs.end();
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}
		return *this;
	}

	template<typename T, typename Alloc>
	list<T, Alloc>::~list()
	{
		clear();
		put_node(node);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::clear()
	{
		list_node *cur = node->next;
		while (cur != node)
		{
			list_node *tmp = cur;
			cur = cur->next;
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos, const T &value)
	{
		list_node *temp = create_node(value);
		temp->next = pos.node;
		temp->prev = pos.node->prev;
		pos.node->prev->next = temp;
		pos.node->prev = temp;
		return temp;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::insert(iterator pos, size_type count, const T &value)
	{
		while (count--)
		{
			insert(pos, value);
		}
	}

	template<typename T, typename Alloc>
	template<typename InputIterator>
	void list<T, Alloc>::insert(iterator pos, InputIterator *first, InputIterator *last)
	{
		while (first != last)
		{
			insert(pos, *first);
			++first;
		}
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator pos)
	{
		iterator temp = pos.node->next;
		pos.node->prev->next = pos.node->next;
		pos.node->next->prev = pos.node->prev;
		destroy_node(pos.node);
		return temp;
	}

	template<typename T, typename Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last)
	{
		iterator result;
		while (first != last)
		{
			result = erase(first++);
		}
		return result;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::push_front(const T & val)
	{
		insert(begin(), val);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::push_back(const T  &val)
	{
		insert(end(), val);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last)
	{
		list_node_ptr position = pos.node;
		list_node_ptr fst = first.node;
		list_node_ptr lst = last.node;
		list_node_ptr temp = position->prev;
		lst->prev->next = position;
		fst->prev->next = lst;
		temp->next = fst;
		position->prev = lst->prev;
		lst->prev = fst->prev;
		fst->prev = temp;
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::splice(iterator pos, list &other)
	{
		if (!other.empty())
		{
			transfer(pos, other.begin(), other.end());
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::splice(iterator pos, list &other, iterator i)
	{
		iterator j = i;
		++j;
		if (pos == i || pos == j) return;
		transfer(pos, i, j);
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::splice(iterator pos, list & other, iterator first, iterator last)
	{
		if (first != last)
		{
			transfer(pos, first, last);
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::merge(list & other)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = other.begin();
		iterator last2 = other.end();
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
			{
				++first1;
			}
		}
		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
	}

	template <typename T, typename Alloc>
	template <typename Compare>
	void list<T, Alloc>::merge(list &other, Compare comp)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = other.begin();
		iterator last2 = other.end();
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
			{
				++first1;
			}
		}
		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
	}

	template <typename T, typename Alloc>
	void list<T, Alloc>::reverse()
	{
		if (node->next == node || node->next->next == node) return;
		iterator first = begin();
		++first;
		while (first != end())
		{
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::remove(const T &value)
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (*it == value)
			{
				it = erase(it);
				--it;
			}
		}
	}

	template<typename T, typename Alloc>
	template<typename Predicate>
	void list<T, Alloc>::remove_if(Predicate p)
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (p(*it))
			{
				it = erase(it);
				--it;
			}
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		iterator next = first;
		while (++next != last)
		{
			if (*first == *next)
			{
				erase(next);
			}
			else
			{
				first = next;
			}
			next = first;
		}
	}

	template<typename T, typename Alloc>
	void list<T, Alloc>::sort()
	{
		if (node->next == node || node->next->next == node) return;
		list carry;
		list counter[64];
		int fill = 0;
		while (!empty())
		{
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
			{
				++fill;
			}
		}
		for (int i = 1; i < fill; ++i)
		{
			counter[i].merge(counter[i - 1]);
		}
		swap(counter[fill - 1]);
	}

	template<typename T>
	bool operator==(const list<T>& lhs, const list<T>& rhs)
	{
		auto first1 = lhs.begin();
		auto last1 = lhs.end();
		auto first2 = rhs.begin();
		auto last2 = rhs.end();
		if (first1 != last1 && first2 != last2 && first1 == first2)
		{
			++first1;
			++first2;
		}
		return first1 == last1 && first2 == last2;
	}

	template<typename T>
	bool operator!=(const list<T>& lhs, const list<T>& rhs)
	{
		return !(lhs == rhs);
	}

}

#endif // !__LIST_H

