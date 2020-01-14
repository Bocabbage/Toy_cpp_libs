/*
    Project:        Toy_Deque
    Update date:    2020/1/14
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

}