/*
    Project:        Toy_STL_Construct
    Description:    build stl-like construct/destroy
    Update date:    2019/2/22
    Author:         Zhuofan Zhang
*/
#pragma once
#include "toy_std.hpp"
#include "toytype_traits.hpp"
#include "toyiterator.hpp"
#include <new>

namespace toy_std
{
    template<typename ForwardIter, typename T>
    inline void __destroy(ForwardIter first, ForwardIter last, T*)
    {
        using trivial_destructor = typename __type_traits<T>::has_trivival_destructor;
        __destroy_aux(first, last, trivial_destructor());
    }

    template<typename ForwardIter>
    inline void __destroy_aux(ForwardIter first, ForwardIter last, __true_type){ }

    template<typename ForwardIter>
    inline void __destroy_aux(ForwardIter first, ForwardIter last, __false_type)
    {
        for (; first != last; ++first)
            destroy(&*first);
    }

    template <typename T1, typename T2> void construct(T1* p, const T2& value) { new(p) T1(value); }

    template <typename T> inline void destroy(T* p) { p->~T(); }

    template<typename ForwardIter>
    inline void destroy(ForwardIter first, ForwardIter last)
    {
        __destroy(first, last, __value_type(first));
    }

    template<>
    inline void destroy(char* first, char* last){ }

    template<>
    inline void destroy(wchar_t* first, wchar_t* last){ }


    

}