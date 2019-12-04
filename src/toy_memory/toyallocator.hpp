/*
    Project:        Toy_Allocator
    Update date:    2019/12/04
    Author:         Zhuofan Zhang
*/
#pragma once
#include"../toy_std.hpp"
#include "../toymemory.hpp"

namespace toy_std
{
    template<typename T>
    class tallocator
    {
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        // template<typename T1, typename T2>
        // friend bool operator==(const tallocator<T1>&, const tallocator<T2>&) noexcept;

        /* Constants */
        static const size_type max_size = std::numeric_limits<size_type>::max();

    private:
        __default_alloc __alloc;

    public:
        /* Constructors */
        tallocator() { } noexcept;

        // tallocator(tallocator<T>&) noexcept;

        // template<typename U>
        // tallocator(tallocator<U>&) noexcept;

        /* Destructor */
        ~tallocator() { } noexcept;



        pointer allocate(size_type);
        void deallocate(pointer, size_type);
        void construct(pointer, const_reference);
        void construct(pointer, size_type, const_pointer);
        void destroy(pointer);
        size_type max_size() { return max_size; }

    };

    template<typename T>
    typename tallocator<T>::pointer
    tallocator<T>::allocate(size_type n)
    {
        return (n == 0 ? nullptr : __alloc.allocate(n * sizeof(T)));
    }

    template<typename T>
    void
    tallocator<T>::deallocate(pointer p, size_type n)
    {
        __alloc.deallocate(p, n * sizeof(T));
    }

    template<typename T>
    void
    tallocator<T>::construct(pointer p, const_reference x)
    {
        construct(p, x);
    }

    template<typename T>
    void
    tallocator<T>::construct(pointer p, size_type n, const_pointer first)
    {
        for (size_t i = 0; i < n; ++i)
            construct(p + i, *(first + i));
    }

    template<typename T>
    void
    tallocator<T>::destroy(pointer p)
    {
        destroy(p);
    }
}