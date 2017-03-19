#ifndef __VECTOR_H
#define __VECTOR_H

#include "__Allocator.h"
#include "__Construct.h"
#include "__Uninitialized.h"
#include <utility>

namespace my_STL
{
	template <typename T, typename Alloc = allocator<T>>
	class vector
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using iterator = value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	protected:
		iterator start;            //使用空间头部
		iterator finish;           //使用空间尾部
		iterator cap;              //可用空间尾部

		using data_allocator = Alloc;

	protected:
		//配置而后填充
		iterator allocate_and_fill(size_type n, const T &x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}

	   //释放空间
		void deallocate()
		{
			if (start)
			{
				data_allocator::deallocate(start, cap - start);
			}
		}

		//填充并初始化
		void fill_initialize(size_type n, const T &value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			cap = finish;
		}

		//分配足够内存来保存给定范围的元素，并将这些元素拷贝到新分配的内存中
		std::pair<T*, T*> alloc_n_copy(const T *b, const T *e)
		{
			auto data = data_allocator::allocate(e - b);
			return{ data, uninitialized_copy(b, e, data) };
		}

		//destroy元素并释放空间
		void free()
		{
			if (start)
			{
				destroy(start, finish);
				deallocate();
			}
		}

		void insert_aux(iterator position, const T &x);

	public:
		//构造函数
		vector() :start(nullptr), finish(nullptr), cap(nullptr) {}
		vector(size_type n, const T &value);
		vector(int n, const T &value);
		vector(long n, const T &value);
		vector(std::initializer_list<T> il);
		explicit vector(size_type n);

		//拷贝构造
		vector(const vector &v);

		//移动构造
		vector(vector &&v);

		//析构函数
		~vector();

		reference operator=(const vector &rhs);
		reference operator=(vector &&rhs);

	public:
		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		bool operator==(const vector &rhs) const;
		bool operator!=(const vector &rhs) const;

	public:
		iterator begin() const
		{
			return start;
		}

		iterator end() const
		{
			return finish;
		}

		size_type size() const
		{
			return size_type(end() - begin());
		}

		size_type capacity() const
		{
			return size_type(cap - begin());
		}

		bool empty() const
		{
			return begin() == end();
		}

		reference front()               //第一个元素
		{
			return *begin();
		}

		reference back()               //最后一个元素
		{
			return *(end() - 1);
		}

		//push_back
		void push_back(const T &x)      //元素插入至尾端
		{
			if (finish != cap)
			{
				construct(finish, x);
				++finish;
			}
			else
			{
				insert_aux(end(), x);
			}
		}

		//pop_back
		void pop_back()              //取出最尾端元素
		{
			--finish;
			destroy(finish);
		}

		//erase
		iterator erase(iterator first, iterator last)        //清除[first, last)中的元素
		{
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		iterator erase(iterator position)        //清除某位置上元素
		{
			if (position + 1 != end())
			{
				copy(position + 1, finish, position);      //后面元素往前移动
			}
			--finish;
			destroy(finish);
			return position;
		}

		//resize
		void resize(size_type new_size, const T &x)
		{
			if (new_size < size())
			{
				erase(begin() + new_size, end());
			}
			else
			{
				insert(end(), new_size - size(), x);
			}
		}

		void resize(size_type new_size)
		{
			resize(new_size, T());
		}

		//clear
		void clear()
		{
			erase(begin(), end());
		}

		//insert
		void insert(iterator position, size_type n, const T &x);
	};

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(size_type n, const T &value)
	{
		fill_initialize(n, value);
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(int n, const T &value)
	{
		fill_initialize(n, value);
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(long n, const T &value)
	{
		fill_initialize(n, value);
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(std::initializer_list<T> il)
	{
		T* const newdata = data_allocator::allocate(il.size());
		T *p = newdata;
		for (auto &t : il)
		{
			construct(p++, t);
		}
		start = newdata;
		finish = cap = start + il.size();
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(size_type n)
	{
		fill_initialize(n, T());
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(const vector &v)
	{
		auto newdata = alloc_n_copy(v.begin(), v.end());
		start = newdata.first;
		finish = cap = newdata.second;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::vector(vector &&v) :start(v.start), finish(v.finish), cap(v.cap)
	{
		v.start = v.finish = v.cap = nullptr;
	}

	template <typename T, typename Alloc>
	vector<T, Alloc>::~vector()
	{
		free();
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::operator=(const vector &rhs)
	{
		if (this != &rhs)
		{
			auto data = alloc_n_copy(rhs.begin(), rhs.end());
			free();
			start = data.first;
			finish = cap = data.second;
		}
		return *this;
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::operator=(vector &&rhs)
	{
		if (this != &rhs)
		{
			free();
			start = rhs.start;
			finish = rhs.finish;
			cap = rhs.cap;

			rhs.start = rhs.finish = rhs.cap = nullptr;
		}
		return *this;
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n)
	{
		return *(begin() + n);
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](size_type n) const
	{
		return *(begin() + n);
	}

	template <typename T, typename Alloc>
	bool vector<T, Alloc>::operator==(const vector &rhs) const
	{
		if (size() != rhs.size())
		{
			return false;
		}
		else
		{
			auto ptr1 = start;
			auto ptr2 = rhs.start;
			for (; ptr1 != finish, ptr2 != rhs.finish; ++ptr1, ++ptr2)
			{
				if (*ptr1 != *ptr2)
				{
					return false;
				}
			}
			return true;
		}
	}

	template <typename T, typename Alloc>
	bool vector<T, Alloc>::operator!=(const vector &rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T &x)
	{
		if (finish != cap)                               //还有空间
		{
			//在备用空间开始处构造一个元素，并用vector最后一个元素为其初值
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else                                             //无备用空间
		{
			//原大小为0，则配置1；原大小不为0，则配置原大小2倍
			const size_type old_size = size();
			const size_type len = (old_size != 0) ? 2 * old_size : 1;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try
			{
				//将原内容拷贝到新vector
				new_finish = uninitialized_copy(start, position, new_start);
				//为新元素设定初值
				construct(new_finish, x);
				++new_finish;
				//将原安插点内容拷贝过来
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch(...)
			{
				//回滚
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}

			free();            //析构并释放原vector
			start = new_start;
			finish = new_finish;
			cap = new_finish + len;
		}
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T &x)
	{
		if (n != 0)
		{
			if (size_type(cap - finish) >= n)            //备用空间大于等于新增元素个数
			{
				T x_copy = x;
				//计算插入点之后的现有元素个数
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n)                     //插入点之后的现有元素大于新增元素个数
				{
					uninitialized_copy(finish - n, finish, finish);
					finish += n;                           //尾端标记后移
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);        //从插入点开始填入新值
				}
				else                                     //插入点之后的现有元素小于等于新增元素个数
				{
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else                                         //备用空间小于新增元素个数
			{
				//决定新空间长度
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);

				//配置新的空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//将旧vector插入点之前的元素复制到新空间
					new_finish = uninitialized_copy(start, position, new_start);
					//将新增元素填入新空间
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//将旧vector插入点之后的元素复制到新空间
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}

				free();                             //清除并释放旧vector
				start = new_start;
				finish = new_finish;
				cap = new_finish + len;
			}
		}
	}
}

#endif // !__VECTOR_H

