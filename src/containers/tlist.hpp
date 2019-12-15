#pragma once
#include"toy_std.hpp"
#include"toymemory.hpp"
#include"toyiterator.hpp"

namespace toy_std
{
    /* tList node structure */
    template<typename T>
    struct __tList_Node
    {
        using __tNode_Pointer = __tList_Node<T>*;

        // Bidirection Node
        __tNode_Pointer _prev;
        __tNode_Pointer _next;
        T _data;
    };
    
    /* tList iterator: Bidirection Iterator */

}