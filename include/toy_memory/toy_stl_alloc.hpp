/*
    Project:        Toy_STL_Construct
    Update date:    2019/12/01
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
        static void deallocate(void *p) { free(p); }
        static void (* set_malloc_handler(void (*f)()))()
        {
            void (*old)() = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return (old);
        }
    };

    void* __malloc_alloc_template::allocate(size_t n)
    {
        void *result = malloc(n);
        if(result == nullptr)
            result = oom_malloc(n);
        return result;
    }

    void* __malloc_alloc_template::oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void *result;

        while(true)
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr)
                throw std::bad_alloc();
            result = malloc(n);
            if(result)
                return result;
        }
    }

}
