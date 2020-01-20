/*
    Project:        Toy_Deque
    Update date:    2020/1/20
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
        // A buffer here means 'the space' which pointed by an 'node'
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
        reference operator*() { return *__cur; }

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
            if (__cur == __first)
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

        void set_node(map_pointer new_node)
        {
            __node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

    private:

        value_type* __cur;      // Currrent element in this buffer
        value_type* __first;    // First element in this buffer
        value_type* __last;     // Last element in this buffer 
        map_pointer __node;     // (Line) pointer to the buffer
        
    };

    template<typename T,
             size_t BuffSize = 0>
    class tdeque
    {
    public:
        /* Member types */
        using value_type = T;
        using allocator_type = tallocator<T>;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = __Deque_Iterator<BuffSize, T>;
        using const_iterator = const __Deque_Iterator<BuffSize, T>;

        /* Constructors */
        tdeque(): __start(), __finish(), __map(), __map_size(), __size(0)
        { __create_map_and_nodes(0); }
        inline tdeque(size_type, const value_type&);
        tdeque(const tdeque<T, BuffSize>&);
        tdeque(tdeque<T, BuffSize>&&);

        /* Iterators */
        const_iterator cbegin() const noexcept { return const __start; }
        const_iterator cend() const noexcept { return const __finish; }
        iterator begin() noexcept { return __start; }
        iterator end() noexcept { return __finish; }

        /* Element Access */
        reference front() { return *__start; }
        reference back() { return *(__finish - 1); }
        const_reference operator[](size_type pos) const { return *(__start + pos); }
        reference operator[](size_type pos) { return const_cast<reference>(static_cast<const_reference>(*this)[pos]); }

        /* Capacity */
        inline size_type size() const { return __size; }
        inline bool empty() const { return __size == 0; }

        /* Modifiers */
        void push_back(value_type&&);
        void push_front(value_type&&);
        void pop_back();
        void pop_front();

    protected:
        using map_pointer = value_type**;

        iterator __start;   // The first element
        iterator __finish;  // The next one of the last element

        map_pointer __map;
        size_type __map_size;
        size_type __size;

        tallocator<value_type> __data_allocator;
        tallocator<pointer> __map_allocator;

        void __fill_initialize(size_type, const value_type&);
        void __create_map_and_nodes(size_type);
        void __push_back_aux(value_type&&);
        void __push_front_aux(value_type&&);
        // void __pop_back_aux();
        // void __pop_front_aux();
        void __reallocate_map(size_type, bool);
    };

    template<typename T, size_t BuffSize>
    void 
    tdeque<T, BuffSize>::__fill_initialize(size_type n, const value_type& value)
    {
        __create_map_and_nodes(n);
        __size = n;
        map_pointer cur;
        for (cur = __start.__node; cur < __finish.__node; ++cur)
            uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
        uninitialized_fill(__finish.__first, __finish.__cur, value);
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements / iterator::buffer_size() + 1;
        __map_size = max(8, num_nodes + 2);
        __map = __map_allocator.allocate(__map_size);

        map_pointer  node_start = __map + (__map_size - num_nodes) / 2;
        map_pointer node_finish = node_start + num_nodes - 1;

        map_pointer cur;
        for (cur = node_start; cur <= node_finish; ++cur)
            *cur = __data_allocator(iterator::buffer_size());

        __start.set_node(node_start);
        __finish.set_node(node_finish);
        __start.__cur = __start.__first;
        __finish.__cur = __finish.__first + num_elements % iterator::buffer_size();
    }

    template<typename T, size_t BuffSize>
    inline
    tdeque<T, BuffSize>::tdeque(size_type count, const value_type& value):
    __start(), __finish(), __map(), __map_size(), __size(0)
    {
        __fill_initialize(count, value);
    }

    template<typename T, size_t BuffSize>
    tdeque<T, BuffSize>::tdeque(const tdeque<T, BuffSize>& other):
    __start(), __finish(), __map(), __map_size(), __size(other.__size)
    {
        __create_map_and_nodes(other.__size);
        auto cur = __start, o_cur = other.__start;
        while (o_cur != other.__finish)
        {
            *cur = *o_cur;
            o_cur++;
            cur++;
        }
        __finish = cur;
    }

    template<typename T, size_t BuffSize>
    tdeque<T, BuffSize>::tdeque(tdeque<T, BuffSize>&& other) :
    __start(other.__start), __finish(other.__finish), 
    __map(other.__map), __map_size(other.__map_size),
    __size(other.__size),
    __map_allocator(other.__map_allocator), __data_allocator(other.__data_allocator)
    {
        other.__start = iterator();
        other.__finish = iterator();
        other.__map = nullptr;
        other.__map_size = 0;
        other.__size = 0;
        __map_allocator = tallocator<T*>();
        __data_allocator = tallocator<T>();
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::push_back(value_type&& value)
    {
        if (__finish.__cur != __finish.__last - 1)
        {
            construct(__finish.__cur, value);
            ++__finish.__cur;
            __size++;
        }
        else
            __push_back_aux(value);
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::push_front(value_type&& value)
    {
        if (__start.__cur != __start.__first)
        {
            construct(__start.__cur - 1, value);
            --__start.__cur;
            __size++;
        }
        else
            __push_front_aux(value);
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::pop_back()
    {
        if (__finish.__cur != __finish.__first)
        {
            --__finish.__cur;
            destroy(__finish.__cur);
            --__size;
        }
        else
            //__pop_back_aux();
        {
            __finish.set_node(__finish.__node - 1);
            __finish.__cur = __finish.__last - 1;
            destroy(__finish.__cur);
            --__size;
        }
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::pop_front()
    {
        if (__start.__cur != __start.__last - 1)
        {
            destroy(__start.__cur);
            __start.__cur++;
            --__size;
        }
        else
            //__pop_front_aux();
        {
            destroy(__start.__cur);
            __start.set_node(__start.__node + 1);
            __start.__cur = __start.__first;
            --__size;
        }
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__push_back_aux(value_type&& value)
    {
        if (__finish.__node + 1 == __map[__map_size - 1])
            __reallocate_map(1, false);
        construct(__finish.__cur, value);
        __finish.set_node(__finish.__node + 1);
        __finish.__cur = __finish.__first;
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__push_front_aux(value_type&& value)
    {
        if (__start.__node == __map[0])
            __reallocate_map(1, true);
        __start.set_node(__start.__node - 1);
        __start.__cur = __start.__last - 1;
        construct(__start.__cur, value);
    }

    /*
    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__pop_back_aux()
    {
        
    }

    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__pop_front_aux()
    {

    }
    */
    
    template<typename T, size_t BuffSize>
    void
    tdeque<T, BuffSize>::__reallocate_map(size_type nodes_to_add, bool add_at_front)
    {
        size_type old_num_nodes = __finish.__node - __start.__node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_node_start;
        if (__map_size > 2 * new_num_nodes)
        {
            // Still have enough space:
            // move the used space segments to the center.
            new_node_start = __map + (__map_size - new_num_nodes) / 2
                             + (add_at_front ? nodes_to_add : 0);
            if (new_node_start < __start.__node)
                copy(__start.__node, __finish.__node + 1, new_node_start);
            else
                copy_backward(__start.__node, __finish.__node + 1, new_node_start + old_num_nodes);

        }
        else
        {
            size_type new_map_size = __map_size + max(__map_size, nodes_to_add) + 2;
            map_pointer new_map = __map_allocator.allocate(new_map_size);
            new_node_start = new_map + (new_map_size - new_num_nodes) / 2
                             + (add_at_front ? nodes_to_add : 0);
            __map_allocator.deallocate(__map, __map_size);
            __map = new_map;
            __map_size = new_map_size;
        }

        __start.set_node(new_node_start);
        __finish.set_node(new_node_start + old_num_nodes - 1);
    }
}