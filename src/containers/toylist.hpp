/*
    Project:        Toy_List
    Update date:    2019/12/17
    Author:         Zhuofan Zhang
*/
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
    template<typename T,
             typename Pointer = T*,
             typename Reference = T&,
             typename Distance = ptrdiff_t>
    class __List_Iterator
    {
    public:

        using __Self = __List_Iterator<T, T*, T&>;
        using __tNode_Pointer = __tList_Node<T>*;

        using iterator_category = bidirection_iterator_tag;
        using value_type = T;
        using pointer = Pointer;
        using reference = Reference;
        using const_reference = const Reference;
        using difference_type = Distance;
        using size_type = size_t;


        /* Constructors */
        __List_Iterator():__node(nullptr){ }
        __List_Iterator(__tNode_Pointer N): __node(N){ }
        __List_Iterator(const __Self& X): __node(X.__node){ }


        /* Operators */
        __Self& operator++() { __node = __node->_next; return *this; }
        __Self  operator++(int) { __Self tmp = *this; ++this; return tmp; }
        __Self& operator--() { __node = __node->_prev; return *this; }
        __Self operator--(int) { __Self tmp = *this; --this; return tmp; }
        const_reference operator*() const { return __node->_data; }
        reference operator*() { return const_cast<reference>(*(static_cast<const __Self*>(this))); }
        bool operator==(const __Self& b) const { return  this->__node == b.__node; }
        bool operator!=(const __Self& b) const { return !(*this == b); }

    private:
        __tNode_Pointer __node;
    };

}