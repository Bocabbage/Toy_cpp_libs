/*
    Project:        Toy_List
    Update date:    2020/1/14
    Author:         Zhuofan Zhang
*/
#pragma once
#include"toy_std.hpp"
#include"toymemory.hpp"
#include"toyiterator.hpp"
#include"toyalgo_base.hpp"

using std::initializer_list;

namespace toy_std
{
    template<typename T, typename Allocator> class tlist;

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
            toy_std::swap(__node, tmp.__node);
            return *this;
        }


        /* Operators */
        __Self& operator++() { __node = __node->_next; return *this; }
        __Self  operator++(int) { __Self tmp = *this; ++this; return tmp; }
        __Self& operator--() { __node = __node->_prev; return *this; }
        __Self operator--(int) { __Self tmp = *this; --this; return tmp; }
        const_reference operator*() const { return __node->_data; }
        reference operator*() { return const_cast<reference>(static_cast<const __Self*>(this)->operator*()); }
        bool operator==(const __Self& b) const { return  this->__node == b.__node; }
        bool operator!=(const __Self& b) const { return !(*this == b); }

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
        using const_pointer = const Pointer;
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
            toy_std::swap(__node, tmp.__node);
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
        using pointer = typename Allocator::pointer;            // Since C++11 the STL use allocator_traits<>
        using iterator = __List_Iterator<T>;
        using reverse_iterator = __List_Reverse_Iterator<T>;
        using const_iterator = const __List_Iterator<T>;
        using const_reverse_iterator = const __List_Reverse_Iterator<T>;
        using __tNode_Pointer = __tList_Node<T>*;

        /* Non-member functions */
        template<typename T, typename Alloc>
        friend bool operator==(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

        template<typename T, typename Alloc>
        friend inline bool operator!=(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

        template<typename T, typename Alloc>
        friend bool operator<(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

        template<typename T, typename Alloc>
        friend bool operator<=(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

        template<typename T, typename Alloc>
        friend inline bool operator>(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

        template<typename T, typename Alloc>
        friend inline bool operator>=(const tlist<T, Alloc>&, const tlist<T, Alloc>&);

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
        ~tlist() noexcept 
        { 
            clear();
            if(__Node != nullptr)
                // r-value constructor/= will bring a empty list
                // with __Node point to NULL.
                __alloc.deallocate(__Node, 1); 
        }

        /* Capacity */
        bool empty() { return __size == 0; }
        size_type size() const noexcept { return __size; }
        size_type max_size() const noexcept { return __alloc.maxsize() / sizeof(__tList_Node<T>); }

        /* Element Access */
        const_reference front() const { return __Node->_next->_data; }
        const_reference back() const { return __Node->_prev->_data; }
        reference front()
        {
            return const_cast<reference>(static_cast<const tlist<T, Allocator>&>(*this).front());
        }
        reference back()
        {
            return const_cast<reference>(static_cast<const tlist<T, Allocator>&>(*this).back());
        }


        /* Iterators */
        iterator begin() noexcept { return iterator(__Node->_next); }
        iterator end() noexcept { return iterator(__Node); }
        const_iterator cbegin() const noexcept { return iterator(__Node->_next); }
        const_iterator cend() const noexcept { return iterator(__Node); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(__Node->_prev); }
        reverse_iterator rend() noexcept { return reverse_iterator(__Node); }
        const_reverse_iterator crbegin() noexcept { return reverse_iterator(__Node->_prev); }
        const_reverse_iterator crend() noexcept { return reverse_iterator(__Node); }

        /* Modifiers */
        void swap(tlist<T, Allocator>&);

        iterator erase(iterator);
        inline iterator erase(iterator, iterator);

        iterator insert(iterator, const value_type&);
        iterator insert(iterator, value_type&&);
        inline iterator insert(iterator, size_type, const value_type&);
        inline iterator insert(iterator, initializer_list<T>);
        template<typename InputIt>
        iterator insert(iterator, InputIt, InputIt);

        void push_back(value_type&&);
        void push_front(value_type&&);
        void pop_front();
        void pop_back();

        inline void clear() noexcept;

        /* Operations */
        void merge(tlist<T, Allocator>&&);
        void reverse() noexcept;
        void remove(const value_type&);
        void unique();
        void sort();
        

    protected:
        __tNode_Pointer __Node;
        Allocator __alloc;
        size_type __size;   // Since the C++11, size() is in complexity constant.

        /* Dispatch Constructor */
        template<typename __Input>
        void __tlist_construct_dispatch(__Input first, __Input last, __false_type)
        {
            __Node = __alloc.allocate(1);
            auto tmp = __Node;
            while (first != last)
            {
                tmp->_next = __alloc.allocate(1);
                tmp->_next->_prev = tmp;
                tmp = tmp->_next;
                tmp->_data = *first;
                ++first;
                __size++;
            }

            __Node->_prev = tmp;
            tmp->_next = __Node;
        }

        template<typename __Input>
        void __tlist_construct_dispatch(__Input n, __Input value, __true_type)
        {
            __size = (size_type)n;
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
                tmp->_data = (value_type)value;
            }

            __Node->_prev = tmp;
            tmp->_next = __Node;
        }

    };

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::swap(tlist<T, Allocator>& t)
    {
        toy_std::swap(this->__Node, t.__Node);
        toy_std::swap(this->__alloc, t.__alloc);
        toy_std::swap(this->__size, t.__size);
    }

    template<typename T, typename Allocator>
    typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::erase(iterator pos)
    {
        if (pos.__node == __Node)
            return end();
        auto res = iterator(pos.__node->_next);
        pos.__node->_prev->_next = pos.__node->_next;
        pos.__node->_next->_prev = pos.__node->_prev;
        __size--;
        __alloc.deallocate(pos.__node, 1);
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
    typename tlist<T, Allocator>::iterator 
    tlist<T, Allocator>::insert(iterator pos, const value_type& value)
    {
        __tNode_Pointer tmp = __alloc.allocate(1);
        tmp->_data = value;
        tmp->_next = pos.__node;
        tmp->_prev = pos.__node->_prev;
        pos.__node->_prev->_next = tmp;
        pos.__node->_prev = tmp;
        __size++;

        return --pos;
    }

    template<typename T, typename Allocator>
    typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::insert(iterator pos, value_type&& value)
    {
        __tNode_Pointer tmp = __alloc.allocate(1);
        tmp->_data = value;
        tmp->_next = pos.__node;
        tmp->_prev = pos.__node->_prev;
        pos.__node->_prev->_next = tmp;
        pos.__node->_prev = tmp;
        __size++;

        return --pos;
    }

    template<typename T, typename Allocator>
    inline typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::insert(iterator pos, size_type count, const value_type& value)
    {
        for (size_type i = 0; i < count; ++i)
            pos = insert(pos, value);
        return pos;
    }

    template<typename T, typename Allocator>
    inline typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::insert(iterator pos, initializer_list<T> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
        /*
        auto t_tmp = ilist.begin();
        auto t_tmp_end = ilist.end();
        if (t_tmp == t_tmp_end)
            return pos;

        iterator res = pos;
        res--;
        while (t_tmp != t_tmp_end)
        {
            insert(pos, *t_tmp);
            t_tmp++;
        }

        return res;
        */
    }

    template<typename T, typename Allocator>
    template<typename InputIt>
    typename tlist<T, Allocator>::iterator
    tlist<T, Allocator>::insert(iterator pos, InputIt first, InputIt last)
    {   
        if (first == last)
            return pos;
        
        iterator res = pos;
        res--;
        while (first != last)
        {
            insert(pos, *first);
            first++;
        }

        return res;
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::push_back(value_type&& value)
    {
        __tNode_Pointer tmp = __alloc.allocate(1);
        tmp->_data = value;
        tmp->_next = __Node;
        tmp->_prev = __Node->_prev;
        __Node->_prev->_next = tmp;
        __Node->_prev = tmp;
        __size++;
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::push_front(value_type&& value)
    {
        __tNode_Pointer tmp = __alloc.allocate(1);
        tmp->_data = value;
        tmp->_prev = __Node;
        tmp->_next = __Node->_next;
        __Node->_next->_prev = tmp;
        __Node->_next = tmp;
        __size++;
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::pop_front()
    {
        auto tmp = __Node->_next;
        __Node->_next->_next->_prev = __Node;
        __Node->_next = __Node->_next->_next;
        __alloc.deallocate(tmp, 1);

    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::pop_back()
    {
        auto tmp = __Node->_prev;
        __Node->_prev->_prev->_next = __Node;
        __Node->_prev = __Node->_prev->_prev;
        __alloc.deallocate(tmp, 1);
    }

    template<typename T, typename Allocator>
    inline void
    tlist<T, Allocator>::clear() noexcept
    {
        erase(begin(), end());
    }

    template<typename T, typename Allocator>
    void tlist<T, Allocator>::merge(tlist<T, Allocator>&& t)
    {
        auto p_size = __size;
        auto p = __Node->_next;
        auto q = t.__Node;
        while (p_size > 0 && t.__size > 0)
        {
            q = t.__Node->_next;
            if (p->_data < q->_data)
            {
                q->_prev->_next = q->_next;
                q->_next->_prev = q->_prev;

                p->_prev->_next = q;
                p->_prev = q;

                t.__size--;
                __size++;
            }
            else
            {
                p_size--;
                p = p->_next;
            }
                
        }

        if (t.__size > 0)
        {
            __Node->_prev->_next = t.__Node->_next;
            t.__Node->_next->_prev = __Node->_prev;
            __Node->_prev = t.__Node->_prev;
            t.__Node->_prev->_next = __Node;

            t.__Node->_prev = t.__Node;
            t.__Node->_next = t.__Node;
            t.__size = 0;
        }

    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::reverse() noexcept
    {
        auto tmp = __Node->_next;
        while (tmp != __Node)
        {
            tmp = tmp->_next;
            toy_std::swap(tmp->_prev, tmp->_next);
        }
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::remove(const value_type& value)
    {
        auto tmp = __Node->_next;
        while (tmp != __Node)
        {
            if (tmp->_data == value)
                tmp = erase(iterator(tmp));
            else
                tmp = tmp->_next;
        }
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::unique()
    {
        auto tmp = __Node->_next;
        value_type _now_value = tmp->_data;
        while (tmp != __Node)
        {
            if (tmp->_value != _now_value)
            {
                tmp = tmp->_next;
                _now_value = tmp->_value;
            }
            else
                tmp = erase(iterator(tmp));
        }
    }

    template<typename T, typename Allocator>
    void
    tlist<T, Allocator>::sort()
    {
        // Selection Sort
        auto tmp = __Node->_next;
        auto max_suffix = tmp;
        while (tmp != __Node)
        {
            max_suffix = __Node->_next;
            for (auto suffix = __Node->_next; suffix != tmp; ++suffix)
                if (suffix->_data > max_suffix->_data)
                    max_suffix = suffix;
            toy_std::swap(tmp->_data, max_suffix->_data);
            tmp = tmp->_next;
        }
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
        rt.__Node = nullptr;
        rt.__alloc = Allocator();
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

            rt.__Node = nullptr;
            rt.__alloc = Allocator();
            rt.__size = 0;
        }

        return *this;
    }

    template<typename T, typename Allocator>
    template<typename InputIt>
    tlist<T, Allocator>::tlist(InputIt first, InputIt last):
    __alloc(), __Node(nullptr), __size(0)
    {
        using is_int_type = typename __Is_Integral_type_traits<InputIt>::is_int;
        __tlist_construct_dispatch(first, last, is_int_type());
        //__Node = __alloc.allocate(1);
        //auto tmp = __Node;
        //while (first != last)
        //{
        //  tmp->_next = __alloc.allocate(1);
        //  tmp->_next->_prev = tmp;
        //  tmp = tmp->_next;
        //  //tmp->_data = *first;
        //  ++first;
        //  __size++;
        //}

        //__Node->_prev = tmp;
        //tmp->_next = __Node;
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


    template<typename T, typename Alloc>
    bool operator==(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        if (lhs.__size != rhs.__size)
            return false;

        auto ls = lhs.__Node->_next, rs = rhs.__Node->_next;
        while (ls != lhs.__Node)
        {
            if (ls->_data != rs->_data)
                return false;
            ls = ls->_next;
            rs = rs->_next;
        }

        return true;
    }

    template<typename T, typename Alloc>
    inline bool
    operator!=(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Alloc>
    bool operator<(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        auto ls = lhs.__Node->_next, rs = rhs.__Node->_next;
        while (ls != lhs.__Node && rs != rhs.__Node)
        {
            if (ls->data < rs->data)
                return true;
            else if (ls->data > rs->data)
                return false;
            ls = ls->_next;
            rs = rs->_next;
        }

        if (rs == rhs.__Node)
            return false;
        return true;
    }

    template<typename T, typename Alloc>
    inline bool
    operator>=(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        return !(lhs < rhs);
    }

    template<typename T, typename Alloc>
    bool operator<=(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        auto ls = lhs.__Node->_next, rs = rhs.__Node->_next;
        while (ls != lhs.__Node && rs != rhs.__Node)
        {
            if (ls->data < rs->data)
                return true;
            else if (ls->data > rs->data)
                return false;
            ls = ls->_next;
            rs = rs->_next;
        }

        if (ls == lhs.__Node)
            return true;
        return false;
    }

    template<typename T, typename Alloc>
    inline bool
    operator>(const tlist<T, Alloc>& lhs, const tlist<T, Alloc>& rhs)
    {
        return !(lhs <= rhs);
    }
}

