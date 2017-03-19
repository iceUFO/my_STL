#ifndef __ALLOC_H
#define __ALLOC_H

#include <cstdlib>

namespace my_STL
{
	class alloc
	{
	private:
		//区块上调边界
		enum
		{
			__ALIGN = 8
		};

		//区块上限
		enum
		{
			__MAX_BYTES = 128
		};

		//free_lists 个数
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
		static char *start_free;         //内存池起始位置
		static char *end_free;           //内存池结束位置
		static size_t heap_size;

	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
		}

		//根据区块大小决定使用第n号free_list，n从0起
		static size_t FREELISTS_INDEX(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		//当free_list没有可用区块时，为free_list重新填充空间，新空间取自内存池
		static void *refill(size_t n);

		//从内存池取空间
		static char *chunk_alloc(size_t size, int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void *reallocate(void *p, size_t old_size, size_t new_size);
	};
}

#undef __PRIVATE

#endif // !__ALLOC_H
