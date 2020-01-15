/*
    Project:        Toy_Deque
    Update date:    2020/1/15
    Author:         Zhuofan Zhang
*/
#pragma once
#include"toy_std.hpp"
#include"toymemory.hpp"
#include"toyiterator.hpp"
#include"toyalgo_base.hpp"

namespace toy_std
{
    inline size_t 
    __deque_buf_size(size_t n, size_t sz)
    {
        // Decide the size of the buffer
        return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }

    template<size_t BuffSize,
             typename T,
             typename Pointer = T*,
             typename Reference = T&,
             typename Distance = ptrdiff_t>
    class __Deque_Iterator
    {
    public:
        using __Self = __Deque_Iterator<BuffSize, T, T*, T&>;
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = ptrdiff_t;
        using map_pointer = T**;

        static size_t buffer_size()
        { return __deque_buf_size(BuffSize, sizeof(T)); }

        /* Constructors */
        __Deque_Iterator() = default;
        
        __Deque_Iterator(__Self& x) :
        __cur(x.__cur), __first(x.__first), __last(x.__last), __node(x.__node)
        { }

        __Deque_Iterator(value_type* cur, value_type* first, value_type* last, map_pointer node):
        __cur(cur), __first(first), __last(last), __node(node)
        { }

        ~__Deque_Iterator() = default;

        /* Operators */
        reference operator*() { return *cur; }

        pointer operator->() { return &(operator*()); }

        __Self& operator++()
        {
            ++__cur;
            if (__cur == last)
            {
                set_node(__node + 1);
                __cur = __first;
            }
            return *this;
        }

        __Self operator++(int)
        {
            __Self res = *this;
            ++* this;
            return res;
        }

        __Self& operator--()
        {
            if (__cur == start)
            {
                set_node(__node - 1);
                __cur = __last;
            }
            __cur--;
            
            return *this;
        }

        __Self operator--(int)
        {
            __Self res = *this;
            --* this;
            return res;
        }

        __Self& operator+=(difference_type n)
        {
            difference_type offset = n + __cur - __first;
            if (offset > 0 && offset < difference_type(buffer_size()))
                __cur += n;
            else
            {
                difference_type node_offset =
                                offset > 0 ?
                                offset / difference_type(buffer_size()) :
                                -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(__node + node_offset);
                __cur = first + (offset - node_offset * difference_type(buffer_size()));
            }

            return *this;
        }

        __Self& operator-=(difference_type n) { return *this += -n; }

        __Self operator+(difference_type n) const
        {
            __Self res = *this;
            res += n;
            return res;
        }

        __Self operator-(difference_type n) const
        {
            __Self res = *this;
            res -= n;
            return res;
        }

        difference_type operator-(const __Self& x) const
        {
            return difference_type(buffer_size()) * (__node - x.__node - 1)
                   + (__cur - __first) + (x.__last - x.__cur);
        }

        reference operator[](difference_type n) { return *(*this + n); }

        bool operator==(const __Self& x) const { return __cur == x.__cur; }
        bool operator!=(const __Self& x) const { return !(*this == x); }
        bool operator<(const __Self& x) const
        {
            return (__node == x.__node) ? (__cur < x.__cur) : (__node < x.__node);
        }

        

    private:

        value_type* __cur;
        value_type* __first;
        value_type* __last;
        map_pointer __node;
        
        void set_node(map_pointer new_node)
        {
            __node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }
    };

    template<typename T,
             std::size_t BuffSize = 0>
    class tdeque
    {
    public:
        /* Member types */
        using value_type = T;
        using allocator_type = tallocator<T>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = __Deque_Iterator<BuffSize, T>;
        using const_iterator = const __Deque_Iterator<BuffSize, T>;




    protected:
        using map_pointer = value_type**;

        iterator __start;   // The first node
        iterator __finish;  // The last node

        map_pointer __map;
        size_type __map_size;

        tallocator<value_type> __data_allocator;
        tallocator<pointer> __map_allocator;

        void __fill_initialize(size_type, const value_type&);
        void __create_map_and_nodes(size_type);
    };

    template<typename T, std::size_t BuffSize>
    void 
    tdeque<T, BuffSize>::__fill_initialize(size_type n, const value_type& value)
    {
        __create_map_and_nodes(n);
        map_pointer cur;
        for (cur = __start.__node; cur < __finish.__node; ++cur)
            uninitialized_fill(__finish.__first, __finish.__cur, value);
    }

    template<typename T, std::size_t BuffSize>
    void
    tdeque<T, BuffSize>::__create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements / iterator::buffer_size() + 1;
        __map_size = max(8, num_nodes + 2);
        __map = __map_allocator.allocate(__map_size);

        map_pointer  nstart = __map + (__map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;

        map_pointer cur;
        for (cur = nstart; cur <= nfinish; ++cur)
            *cur = __data_allocator(iterator::buffer_size());

        __start.set_node(nstart);
        __finish.set_node(nfinish);
        __start.__cur = __start.__first;
        __finish.__cur = __finish.__first + num_elements % iterator::buffer_size();
    }
}