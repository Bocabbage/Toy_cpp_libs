/*
    Project:        Toy_Mem_Tools
    Update date:    2019/12/04
    Author:         Zhuofan Zhang
*/
#pragma once
#include "../toy_std.hpp"

namespace toy_std
{
    // will be replaced after the iterators implement.
    using InputIterator = char*;
    using ForwardIterator = char*;

    /* Memory manage tools */
    // Now the functions are not completed.
    void uninitialized_copy(InputIterator,InputIterator,ForwardIterator);
    void uninitialized_copy_n(InputIterator, size_t, ForwardIterator);
    
    template<typename __ForwardIterator, typename T>
    void uninitialized_fill(__ForwardIterator, __ForwardIterator, const T&);

    template<typename __ForwardIterator, typename T>
    void uninitialized_fill_n(__ForwardIterator, size_t, const T&);


    /* Implement */
    void uninitialized_copy(InputIterator first,
                            InputIterator last,
                            ForwardIterator result)
    {
        auto __tmp = first;
        auto __tmp_res = result;
        while (__tmp != last)
        {
            // use trait<ForwardIterator>::operator=
            // because use 'construct' might lead to memory leak.

            // can be replaced by toy_std::copy(unfinished)
            *__tmp_res = *__tmp;
            __tmp++;
            __tmp_res++;
        }
    }

    void uninitialized_copy_n(InputIterator first, 
                              size_t n, 
                              ForwardIterator result)
    {
        for (size_t i = 0; i < n; i++)
            *(result + i) = *(first + i);
    }

    template<typename __ForwardIterator, typename T>
    void uninitialized_fill(__ForwardIterator first, 
                            __ForwardIterator last, 
                            const T& value)
    {
        auto __tmp = first;
        while (__tmp != last)
            *__tmp++ = value;
    }

    template<typename __ForwardIterator, typename T>
    void uninitialized_fill_n(__ForwardIterator first, 
                              size_t n,
                              const T& value)
    {
        for (size_t i = 0; i < n; i++)
            *(first + i) = value;
    }
}
