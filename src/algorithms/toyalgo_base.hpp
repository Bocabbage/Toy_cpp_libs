/*
    Project:        Toy_Mem_Tools
    Update date:    2020/1/14
    Author:         Zhuofan Zhang

    Update Log:     2019/12/16 -- Implement 'copy','fill','fill_n'.

*/
#pragma once
#include"toy_std.hpp"
#include"toytype_traits.hpp"
#include"toyiterator.hpp"

namespace toy_std
{

/* max */
    template<typename T>
    inline const T& max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template<typename T, typename Compare>
    inline const T& max(const T& a, const T& b, Compare cmp)
    {
        return cmp(a, b) ? a : b;
    }

/* swap */
    template<typename T>
    void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = a;
    }

/* fill */
    template<typename ForwardIterator,typename T>
    void fill(ForwardIterator first, ForwardIterator last, T x)
    {
        for (; first != last; first++)
            *first = x;
    }


/* fill_n */
    template<typename ForwardIterator, typename T>
    void fill_n(ForwardIterator first, size_t n, T x)
    {
        for (; n > 0; --n, ++first)
            *first = x;
    }


/* copy */
    /* Dispatch template */
    // Generalized Version
    template<typename InputIterator, typename OutputIterator>
    struct __copy_dispatch
    {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
        {
            return __copy(first, last, result, __iterator_category(first));
        }
    };

    // Partial Specialization version for T*
    // Remain a problem that whether this specialization can be removed;(Waiting for unit-test to give the answer)a
    template<typename T>
    struct __copy_dispatch<T*, T*>
    {
        T* operator()(T* first, T* last, T* result)
        {
            using has_trivial = typename __type_traits<T>::has_trivival_assignment_operator;
            // the _t suffix might mean 'trivival'
            return __copy_t(first, last, result, has_trivial());
        }
    };

    // Partial Specialization version for const T*
    template<typename T>
    struct __copy_dispatch<const T*, T*>
    {
        T* operator()(const T* first, const T* last, T* result)
        {
            using has_trivial = typename __type_traits<T>::has_trivival_assignment_operator;
            // the _t suffix might mean 'trivival'
            return __copy_t(first, last, result, has_trivial());
        }
    };


    /* Internal Methods */
    // Case1: Has trivival assignment operator
    template<typename T>
    inline T* __copy_t(const T* first, const T* last, T* result, __true_type)
    {
        memmove(result, first, (last - first) * sizeof(T));
        return result + (last - first);
    }

    // Case2: Hasn't trivival assignment operator
    template<typename T>
    inline T* __copy_t(const T* first, const T* last, T* result, __false_type)
    {
        return __copy_d(first, last, result, (ptrdiff_t)0);
    }

    // InputIterator Version
    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
    {
        for (; first != last; result++, first++)
            *result = *first;       // assignment operator
        return result;
    }

    // RandomAccessIterator Version
    template<typename RandomAccessIterator, typename OutputIterator>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
                                 OutputIterator result, random_access_iterator_tag)
    {
        return __copy_d(first, last, result, __difference_type(first));
    }

    template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
                                   OutputIterator result, Distance*)
    {
        for (Distance n = last - first; n > 0; --n, ++result, ++first)
            *result = *first;
        return result;
    }


    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __copy_dispatch<InputIterator, OutputIterator>::operator()(first, last, result);
    }

    /* Complete Specialization versions of copy: Use memmove() */
    template<>
    inline char* copy(char* first, char* last, char* result)
    {
        memmove(result, first, (last - first));
        return result + (last - first);
    }

    /* Complete Specialization versions of copy: Use wmemmove() */
    template<>
    inline wchar_t* copy(wchar_t* first, wchar_t* last, wchar_t* result)
    {
        wmemmove(result, first, (last - first));
        return result + (last - first);
    }
    
}