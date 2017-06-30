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
		
		T& operator*() const;
		T* operator->() const;

		__list_iterator &operator++();
		__list_iterator operator++(int);
		__list_iterator &operator--();
		__list_iterator operator--(int);

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

	protected:
		//配置节点
		list_node* get_node()
		{
			return list_node_allocator::allocate();
		}

		//释放节点
		void put_node(list_node* p)
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
		list(InputIterator first, InputIterator last);
		list(const list &l);
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

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

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

}

#endif // !__LIST_H

