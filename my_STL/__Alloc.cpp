#include "__Alloc.h"

namespace my_STL
{
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	void *alloc::allocate(size_t n)
	{
		if (n > __MAX_BYTES)  //超过128
		{
			return malloc(n);
		}
		int index = FREELISTS_INDEX(n);
		obj *result = free_list[index];
		if (result)      //list 有空间
		{
			free_list[index] = result->next;
			return result;
		}
		else         //没有空间，从内存池取
		{
			return refill(ROUND_UP(n));
		}
	}

	void alloc::deallocate(void *p, size_t n)
	{
		if (n > __MAX_BYTES)   //大于128，还给堆
		{
			free(p);
		}
		else                   //还给free_list
		{
			int index = FREELISTS_INDEX(n);
			obj *node = static_cast<obj *>(p);
			node->next = free_list[index];
			free_list[index] = node;
		}
	}

	void *alloc::reallocate(void *p, size_t old_size, size_t new_size)
	{
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}

	void *alloc::refill(size_t n)
	{
		int nobjs = 20;
		char *chunk = chunk_alloc(n, nobjs);
		if (nobjs == 1)
		{
			return chunk;
		}

		int index = FREELISTS_INDEX(n);
		obj *result = (obj *)chunk;
		obj *next_obj = (obj *)(chunk + n);
		obj *current_obj = next_obj;
		free_list[index] = current_obj;
		for (int i = 1; ;i++)
		{
			current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + n);
			if (i == (nobjs - 1))
			{
				current_obj->next = 0;
				break;
			}
			else
			{
				current_obj->next = next_obj;
			}
		}
		return result;
	}

	char *alloc::chunk_alloc(size_t size, int &nobjs)
	{
		char *result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes)              //内存池剩余满足需求
		{
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else if (bytes_left >= size)             //内存池不完全满足需求，但足够一个以上区块
		{
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}
		else                                   //剩余连一个块都无法提供
		{
			if (bytes_left > 0)                //将剩余分配给适当的free_list
			{
				int index = FREELISTS_INDEX(bytes_left);
				((obj *)start_free)->next = free_list[index];
				free_list[index] = (obj *)start_free;
			}

			//补充内存池
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = static_cast<char *>(malloc(bytes_to_get));
			if (start_free == 0)                      //heap空间不足，malloc失败
			{
				for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					int index = FREELISTS_INDEX(i);
					if (free_list[index] != 0)
					{
						start_free = (char *)free_list[index];
						free_list[index] = free_list[index]->next;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;
				return nullptr;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
}