/*
    Project:        Toy_STL_Construct
    Update date:    2019/12/03
    Author:         Zhuofan Zhang

    Update Log:     2019/12/03 -- Add 'allocate/deallocate/refill' of the sub-allocator
*/
#pragma once
#include "../toy_std.hpp"
#include <new>

namespace toy_std
{
    class __malloc_alloc
    {
    private:
        /* oom: Out of memory */
        static void* oom_malloc(size_t);
        // Function pointer:
        static void (*__malloc_alloc_oom_handler)();

    public:
        static void* allocate(size_t);
        static void deallocate(void* p) { free(p); }
        static void (*set_malloc_handler(void (*f)()))()
        {
            void (*old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return (old);
        }
    };

    // Initialize the new_handler with 'nullptr'
    void (*__malloc_alloc::__malloc_alloc_oom_handler)() = 0;

    void* __malloc_alloc::allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == 0)
            result = oom_malloc(n);
        return result;
    }

    void* __malloc_alloc::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void* result;

        while (true)
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == 0)
                throw std::bad_alloc();
            result = malloc(n);
            if (result)
                return result;
        }
    }

    const size_t __ALIGN = 8;   // round_up bound
    const size_t __MAX_BYTES = 128;
    const size_t __NFREELISTS = __MAX_BYTES / __ALIGN;

    class __default_alloc
    {
    private:
        static size_t ROUND_UP(size_t bytes)
        {
            return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
        }

        union obj
        {
            union obj* free_list_link;
            char client_data[1];        // The client sees this.
        };

        /* 
            16 free-lists 
            manage blocks size of:
            8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
        */
        static obj* volatile free_list[__NFREELISTS];

        static size_t FREELIST_INDEX(size_t bytes)
        {
            // Decide which free_list to be used according to
            // the block size.
            return ((bytes + __ALIGN - 1) / __ALIGN - 1);
        }

        static void* refill(size_t);
        static char* chunk_alloc(size_t, int&);
        
        // Chunk allocation state
        static char* start_free;    // memory pool start, only be modified by chunk_alloc()
        static char* end_free;      // memory pool end, only be modified by chunk_alloc()
        static size_t heap_size;

    public:
        static void* allocate(size_t n);
        static void deallocate(void*, size_t);
    };

    // Initialization
    char* __default_alloc::start_free = 0;
    char* __default_alloc::end_free = 0;
    size_t __default_alloc::heap_size = 0;
    __default_alloc::obj* volatile __default_alloc::free_list[__NFREELISTS] =
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    char* __default_alloc::chunk_alloc(size_t n, int& nobjs)
    {
        // Memory Pool
    }

    void* __default_alloc::allocate(size_t n)
    {
        if (n > __MAX_BYTES)
            return __malloc_alloc::allocate(n);

        obj* volatile* my_free_list; // pointer of pointer
        obj* result;

        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if (result == 0)
        {
            // no free-mem could be used; refill the free-list.
            void* r = refill(ROUND_UP(n));
            return r;
        }
    
        // Modify the free list
        *my_free_list = result->free_list_link;
        return result;
    }

    void __default_alloc::deallocate(void* p, size_t n)
    {
        if (n > __MAX_BYTES)
        {
            __malloc_alloc::deallocate(p);
            return;
        }
        
        obj* q = (obj*) p;
        obj* volatile* my_free_list = free_list + FREELIST_INDEX(n);
        
        q->free_list_link = *my_free_list;
        *my_free_list = q;

    }

    void* __default_alloc::refill(size_t n)
    {
        // default: get 20 new blocks
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        obj* volatile* my_free_list;
        obj* result;
        obj* current_obj, * next_obj;
        int i;

        if (nobjs == 1)return chunk;
        my_free_list = free_list + FREELIST_INDEX(n);
        
        result = (obj*)chunk;
        *my_free_list = next_obj = (obj*)(chunk + n);

        for (i = 1;; i++)
        {
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj + n);
            if (nobjs - 1 == i)
            {
                current_obj->free_list_link = 0;
                break;
            }
            else
                current_obj->free_list_link = next_obj;
        }
        return result;
    }
}
