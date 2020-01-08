/*
    Project:        Toy_List
    Update date:    2020/1/8
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
        __List_Iterator() = default;
        __List_Iterator(__tNode_Pointer N): __node(N){ }
        __List_Iterator(const __Self& X): __node(X.__node){ }
        __Self& operator=(const __Self& X)
        {
            __Self tmp(X);
            std::swap(__node, tmp.__node);
            return *this;
        }


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

    template<typename T,
             typename Pointer = T*,
             typename Reference = T&,
             typename Distance = ptrdiff_t>
    class __List_Reverse_Iterator
    {
        // Will be replaced after I have learnt the 'iterator adaptor'.
    public:

        using __Self = __List_Reverse_Iterator<T, T*, T&>;
        using __tNode_Pointer = __tList_Node<T>*;

        using iterator_category = bidirection_iterator_tag;
        using value_type = T;
        using pointer = Pointer;
        using reference = Reference;
        using const_reference = const Reference;
        using difference_type = Distance;
        using size_type = size_t;

        /* Constructors */
        __List_Reverse_Iterator() = default;
        __List_Reverse_Iterator(__tNode_Pointer N) : __node(N) { }
        __List_Reverse_Iterator(const __Self & X) : __node(X.__node) { }
        __Self& operator=(const __Self & X)
        {
            __Self tmp(X);
            std::swap(__node, tmp.__node);
            return *this;
        }

        /* Operators */
        __Self& operator++() { __node = __node->_prev; return *this; }
        __Self  operator++(int) { __Self tmp = *this; ++this; return tmp; }
        __Self& operator--() { __node = __node->_next; return *this; }
        __Self operator--(int) { __Self tmp = *this; --this; return tmp; }
        const_reference operator*() const { return __node->_data; }
        reference operator*() { return const_cast<reference>(*(static_cast<const __Self*>(this))); }
        bool operator==(const __Self& b) const { return  this->__node == b.__node; }
        bool operator!=(const __Self& b) const { return !(*this == b); }

    private:
        __tNode_Pointer __node;
    };

    /* tList */
    template<typename T, typename Allocator = tallocator<__tList_Node<T>>>
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
        using iterator = __List_Iterator<T>;
        using reverse_iterator = __List_Reverse_Iterator<T>;
        using const_iterator = const __List_Iterator<T>;
        using const_reverse_iterator = const __List_Reverse_Iterator<T>;
        using __tNode_Pointer = __tList_Node<T>*;

        /* Constructors */
        tlist();
        tlist(size_type, const value_type&);
        tlist(const tlist<T, Allocator>&);
        tlist(tlist<T, Allocator>&&) noexcept;
        tlist(initializer_list<value_type>);
        template<typename InputIt>
        tlist(InputIt first, InputIt last);
        tlist<T, Allocator>& operator=(const tlist<T, Allocator>&);
        tlist<T, Allocator>& operator=(tlist<T, Allocator>&&);

        /* Destructor */
        ~tlist() noexcept { clear(); }

        /* Capacity */
        bool empty() { return __size == 0; }
        size_type size() const noexcept { return __size; }
        size_type max_size() const noexcept { return __alloc.maxsize() / sizeof(__tList_Node<T>); }

        /* Element Access */
        const_reference front() const { return __Node->_next->_data; }
        const_reference back() const { return __Node->_prev->_data; }
        reference front()
        {
            return const_cast(static_cast<const tlist<T, Allocator>&>(*this).front());
        }
        reference back()
        {
            return const_cast(static_cast<const tlist<T, Allocator>&>(*this).back());
        }


        /* Iterators */
        iterator begin() noexcept { return iterator(__Node->_next); }
        iterator end() noexcept { return iterator(__Node); }
        const_iterator cbegin() const noexcept { return const iterator(__Node->_next); }
        const_iterator cend() const noexcept { return const iterator(__Node); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(__Node->_prev); }
        reverse_iterator rend() noexcept { return reverse_iterator(__Node); }
        const_reverse_iterator crbegin() noexcept { return const reverse_iterator(__Node->_prev); }
        const_reverse_iterator crend() noexcept { return const reverse_iterator(__Node); }

        /* Modifiers */
        void swap(tlist<T, Allocator>&);
        iterator erase(iterator);
        inline iterator erase(iterator, iterator);
        inline void clear() noexcept;
        

    protected:
        __tNode_Pointer __Node;
        Allocator __alloc;
        size_type __size;   // Since the C++11, size() is in complexity constant.
    };

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::swap(tlist<T, Allocator>& t)
    {
        std::swap(this->__Node, t.__Node);
        std::swap(this->__alloc, t.__alloc);
        std::swap(this->__size, t.__size);
    }

    template<typename T, typename Allocator>
    typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::erase(iterator pos)
    {
        if (pos->__node == __Node)
            return end();
        auto res = iterator(pos->__node->_next);
        pos->__node->_prev->_next = pos->__node->_next;
        pos->__node->_next->_prev = pos->__node->_prev;
        __size--;
        __alloc.deallocate(pos->__node);
        return res;
    }

    template<typename T, typename Allocator>
    inline typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::erase(iterator first, iterator last)
    {
        while (first != last)
            first = erase(first);
        return first;
    }

    template<typename T, typename Allocator>
    inline void
    tlist<T, Allocator>::clear() noexcept
    {
        erase(begin(), end());
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist():
    __alloc(), __Node(nullptr), __size(0)
    {
        // put an empty node in the tlist.
        __Node = __alloc.allocate(1);
        __Node->_prev = __Node;
        __Node->_next = __Node;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(size_type n, const value_type& value):
    __alloc(), __Node(nullptr), __size(n)
    {
        __Node = __alloc.allocate(1);
        // Can be replaced by the last 2 lines.
        // __Node->_prev = __Node;
        // __Node->_next = __Node;
        auto tmp = __Node;
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
    __alloc(), __Node(nullptr), __size(t.__size)
    {
        __Node = __alloc.allocate(1);
        // Can be replaced by the last 2 lines.
        // __Node->_prev = __Node;
        // __Node->_next = __Node;

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
    __alloc(rt.__alloc), __Node(rt.__Node), __size(rt.__size)
    {
        rt.__alloc = Allocator();   // new empty allocator
        rt.__Node = nullptr;
        rt.__size = 0;
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
            __size = rt.__size;

            rt.__alloc = Allocator();
            rt.__Node = nullptr;
            rt.__size = 0;
        }

        return *this;
    }

    template<typename T, typename Allocator>
    template<typename InputIt>
    tlist<T, Allocator>::tlist(InputIt first, InputIt last):
    __alloc(), __Node(nullptr), __size(0)
    {
        __Node = __alloc.allocate(1);
        auto tmp = __Node;
        while (first != last)
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            tmp->_data = *first;
            first++;
            __size++;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

    template<typename T, typename Allocator>
    tlist<T, Allocator>::tlist(initializer_list<value_type> ilist):
    __alloc(), __Node(nullptr), __size(0)
    {
        __Node = __alloc.allocate(1);
        auto t_tmp = ilist.begin();
        auto t_tmp_end = ilist.end();
        auto tmp = __Node;
        while (t_tmp != t_tmp_end)
        {
            tmp->_next = __alloc.allocate(1);
            tmp->_next->_prev = tmp;
            tmp = tmp->_next;
            tmp->_data = *t_tmp;
            t_tmp++;
            __size++;
        }

        __Node->_prev = tmp;
        tmp->_next = __Node;
    }

}

