/*
    Project:        Toy_STL_Construct
    Update date:    2019/12/02
    Author:         Zhuofan Zhang
*/
#pragma once
#include "../toy_std.hpp"
#include <new>

namespace toy_std
{
    class __malloc_alloc_template
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
    void (*__malloc_alloc_template::__malloc_alloc_oom_handler)() = nullptr;

    void* __malloc_alloc_template::allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == nullptr)
            result = oom_malloc(n);
        return result;
    }

    void* __malloc_alloc_template::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void* result;

        while (true)
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == nullptr)
                throw std::bad_alloc();
            result = malloc(n);
            if (result)
                return result;
        }
    }

    const size_t __ALIGN = 8;   // round_up bound
    const size_t __MAX_BYTES = 128;
    const size_t __NFREELISTS = __MAX_BYTES / __ALIGN;

    class __default_alloc_template
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

        // 16 free-lists
        static obj* volatile free_list[__NFREELISTS];

        static size_t FREELIST_INDEX(size_t bytes)
        {
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
    char* __default_alloc_template::start_free = 0;
    char* __default_alloc_template::end_free = 0;
    size_t __default_alloc_template::heap_size = 0;
    __default_alloc_template::obj* volatile __default_alloc_template::free_list[__NFREELISTS] =
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

}
