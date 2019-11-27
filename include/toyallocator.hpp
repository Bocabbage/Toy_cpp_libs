/*
    Project:        Toy_Allocator
    Update date:    2019/11/27
    Author:         Zhuofan Zhang
*/
#pragma once
#include"toy_std.hpp"
#include<new>

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

        template<typename T1, typename T2>
        friend bool operator==(const tallocator<T1>&, const tallocator<T2>&) noexcept;

        /* Constants */
        static const size_type max_size = std::numeric_limits<size_type>::max();
        
    public:
        /* Constructors */
        tallocator() noexcept;
        tallocator(tallocator<T>&) noexcept;

        template<typename U>
        tallocator(tallocator<U>&) noexcept;

        /* Destructor */
        ~tallocator() noexcept;

        

        pointer allocate(size_type);
        void deallocate(pointer, size_type);
        void construct(pointer, const_reference);
        void construct(pointer, size_type, const_pointer);
        void destroy(pointer);
        size_type max_size() { return max_size; }

    };
}