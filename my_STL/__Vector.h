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
		iterator start;            //ʹ�ÿռ�ͷ��
		iterator finish;           //ʹ�ÿռ�β��
		iterator cap;              //���ÿռ�β��

		using data_allocator = Alloc;

	protected:
		//���ö������
		iterator allocate_and_fill(size_type n, const T &x)
		{
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, x);
			return result;
		}

	   //�ͷſռ�
		void deallocate()
		{
			if (start)
			{
				data_allocator::deallocate(start, cap - start);
			}
		}

		//��䲢��ʼ��
		void fill_initialize(size_type n, const T &value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			cap = finish;
		}

		//�����㹻�ڴ������������Χ��Ԫ�أ�������ЩԪ�ؿ������·�����ڴ���
		std::pair<T*, T*> alloc_n_copy(const T *b, const T *e)
		{
			auto data = data_allocator::allocate(e - b);
			return{ data, uninitialized_copy(b, e, data) };
		}

		//destroyԪ�ز��ͷſռ�
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
		//���캯��
		vector() :start(nullptr), finish(nullptr), cap(nullptr) {}
		vector(size_type n, const T &value);
		vector(int n, const T &value);
		vector(long n, const T &value);
		vector(std::initializer_list<T> il);
		explicit vector(size_type n);

		//��������
		vector(const vector &v);

		//�ƶ�����
		vector(vector &&v);

		//��������
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

		reference front()               //��һ��Ԫ��
		{
			return *begin();
		}

		reference back()               //���һ��Ԫ��
		{
			return *(end() - 1);
		}

		//push_back
		void push_back(const T &x)      //Ԫ�ز�����β��
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
		void pop_back()              //ȡ����β��Ԫ��
		{
			--finish;
			destroy(finish);
		}

		//erase
		iterator erase(iterator first, iterator last)        //���[first, last)�е�Ԫ��
		{
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		iterator erase(iterator position)        //���ĳλ����Ԫ��
		{
			if (position + 1 != end())
			{
				copy(position + 1, finish, position);      //����Ԫ����ǰ�ƶ�
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
		if (finish != cap)                               //���пռ�
		{
			//�ڱ��ÿռ俪ʼ������һ��Ԫ�أ�����vector���һ��Ԫ��Ϊ���ֵ
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;
			copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		}
		else                                             //�ޱ��ÿռ�
		{
			//ԭ��СΪ0��������1��ԭ��С��Ϊ0��������ԭ��С2��
			const size_type old_size = size();
			const size_type len = (old_size != 0) ? 2 * old_size : 1;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try
			{
				//��ԭ���ݿ�������vector
				new_finish = uninitialized_copy(start, position, new_start);
				//Ϊ��Ԫ���趨��ֵ
				construct(new_finish, x);
				++new_finish;
				//��ԭ��������ݿ�������
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch(...)
			{
				//�ع�
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}

			free();            //�������ͷ�ԭvector
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
			if (size_type(cap - finish) >= n)            //���ÿռ���ڵ�������Ԫ�ظ���
			{
				T x_copy = x;
				//��������֮�������Ԫ�ظ���
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n)                     //�����֮�������Ԫ�ش�������Ԫ�ظ���
				{
					uninitialized_copy(finish - n, finish, finish);
					finish += n;                           //β�˱�Ǻ���
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);        //�Ӳ���㿪ʼ������ֵ
				}
				else                                     //�����֮�������Ԫ��С�ڵ�������Ԫ�ظ���
				{
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else                                         //���ÿռ�С������Ԫ�ظ���
			{
				//�����¿ռ䳤��
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);

				//�����µĿռ�
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try
				{
					//����vector�����֮ǰ��Ԫ�ظ��Ƶ��¿ռ�
					new_finish = uninitialized_copy(start, position, new_start);
					//������Ԫ�������¿ռ�
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//����vector�����֮���Ԫ�ظ��Ƶ��¿ռ�
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}

				free();                             //������ͷž�vector
				start = new_start;
				finish = new_finish;
				cap = new_finish + len;
			}
		}
	}
}

#endif // !__VECTOR_H

