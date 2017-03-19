#ifndef __ALLOC_H
#define __ALLOC_H

#include <cstdlib>

namespace my_STL
{
	class alloc
	{
	private:
		//�����ϵ��߽�
		enum
		{
			__ALIGN = 8
		};

		//��������
		enum
		{
			__MAX_BYTES = 128
		};

		//free_lists ����
		enum
		{
			__NFREELISTS = __MAX_BYTES / __ALIGN
		};

    private:
		union obj
		{
			union obj *next;
			char client_data[1];
		};

		static obj *free_list[__NFREELISTS];
	private:
		static char *start_free;         //�ڴ����ʼλ��
		static char *end_free;           //�ڴ�ؽ���λ��
		static size_t heap_size;

	private:
		//��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
		}

		//���������С����ʹ�õ�n��free_list��n��0��
		static size_t FREELISTS_INDEX(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		//��free_listû�п�������ʱ��Ϊfree_list�������ռ䣬�¿ռ�ȡ���ڴ��
		static void *refill(size_t n);

		//���ڴ��ȡ�ռ�
		static char *chunk_alloc(size_t size, int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void *reallocate(void *p, size_t old_size, size_t new_size);
	};
}

#undef __PRIVATE

#endif // !__ALLOC_H
