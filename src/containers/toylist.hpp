/*
    Project:        Toy_List
    Update date:    2020/1/7
    Author:         Zhuofan Zhang
*/
#pragma once
#include"toy_std.hpp"
#include"toymemory.hpp"
#include"toyiterator.hpp"

using std::initializer_list;

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

    /* tList */
    // Noted that the Allocator parameter is not used here.
    template<typename T, typename Allocator = tallocator<T>>
    class tlist
    {
    public:
        /* Member types */
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = Allocator::pointer;         // Since C++11 the STL use allocator_traits<>
        using iterator = __List_Iterator;
        using __tNode_Pointer = __tList_Node<T>*;

        /* Constructors */
        tlist() :__alloc(), __Node(nullptr) {  }
        tlist(size_type, const value_type&);
        tlist(const tlist<T, Allocator>&);
        tlist(tlist<T, Allocator>&&) noexcept;
        tlist(initializer_list<value_type>);
        template<typename InputIt>
        tlist(InputIt first, InputIt last);
        tlist<T, Allocator>& operator=(const tlist<T, Allocator>&);
        tlist<T, Allocator>& operator=(tlist<T, Allocator>&&);

        /* Destructor */
        ~tlist() noexcept;

        /* Iterators */

        /* Modifiers */
        void swap(tlist<T, Allocator>&);
        void clear() noexcept;
        

    protected:
        __tNode_Pointer __Node;
        tallocator<__tList_Node<T>> __alloc;
    };

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::swap(tlist<T, Allocator>& t)
    {
        std::swap(this->__Node, t.__Node);
        std::swap(this->__alloc, t.__alloc);
    }

    /*
    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::clear() noexcept
    {

    }
    */

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(size_type n, const value_type& value):
    __alloc(), __Node(nullptr)
    {
        __Node = __alloc.allocate(1);
        __Node->_data = value;
        auto tmp = __Node;
        n--;
        for (size_t i = 0; i < n; i++)
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            tmp->_data = value;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(const tlist<T, Allocator>& t):
    __alloc(), __Node(nullptr)
    {
        if (t.__Node == nullptr)
        {
            __Node = nullptr;
            return;
        }

        __Node = __alloc.allocate(1);
        __Node->_data = t.__Node->_data;
        auto tmp = __Node;
        auto t_tmp = t.__Node;
        while (t_tmp->_next != t.__Node)
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            t_tmp = t_tmp->_next;
            tmp->_data = t_tmp->_data;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(tlist<T, Allocator>&& rt) noexcept:
    __alloc(rt.__alloc), __Node(rt.__Node)
    {
        rt.__alloc = tallocator<__tList_Node<T>>(); // new empty allocator
        rt.__Node = nullptr;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>&
    tlist<T, Allocator>::operator=(const tlist<T, Allocator>& t)
    {
        tlist<T, Allocator> tmp(t);
        swap(tmp);
        return *this;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>&
    tlist<T, Allocator>::operator=(tlist<T, Allocator>&& rt)
    {
        if (this != &rt)
        {
            clear();
            __alloc = rt.__alloc;
            __Node = rt.__Node;

            rt.__alloc = tallocator<__tList_Node<T>>();
            rt.__Node = nullptr;
        }

        return *this;
    }

    template<typename T, typename Allocator>
    template<typename InputIt>
    tlist<T, Allocator>::tlist(InputIt first, InputIt last):
    __alloc(), __Node(nullptr)
    {
        if (first == last)
        {
            __Node = nullptr;
            return;
        }

        __Node = __alloc.allocate(1);
        __Node->_data = *first;
        auto tmp = __Node;
        first++;
        while (first != last)
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            tmp->_data = *first;
            first++;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(initializer_list<value_type> ilist):
    __alloc(), __Node(nullptr)
    {
        if (ilist.size() == 0)
        {
            __Node = nullptr;
            return;
        }

        __Node = __alloc.allocate(1);
        auto t_tmp = ilist.begin()
        __Node->_data = *t_tmp;
        auto tmp = __Node;
        t_tmp++;
        while (t_tmp != ilist.end())
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            tmp->_data = *t_tmp;
            t_tmp++;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

}

