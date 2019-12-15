/*
    Project:        Toy_STL_Construct
    Description:    build stl-like construct/destroy
    Update date:    2019/11/29
    Author:         Zhuofan Zhang
*/
#pragma once
#include "toy_std.hpp"
#include <new>

namespace toy_std
{
    template <typename T1, typename T2> void construct(T1 *p, const T2& value){ new(p) T1(value); }

    template <typename T> inline void destroy(T *p) { p->~T(); }


    /*
        Unfinished:
        1. implement destroy version with paras: [ForwardIter, ForwardIter) 
        2. Use type_traits to optimize trivial type
    */

}