/*
    Project:        Toy_Mem_Tools
    Update date:    2019/2/22
    Author:         Zhuofan Zhang

    Update Log:     2019/12/15 -- Try to finish the different types(unfinished, need to implement the 'copy'/'fill' functions)
                    2019/12/16 -- Implement 'copy/fill/fill_n' in <toyalgo_base.hpp>; temporarily removed 'uninitialized_copy_n'

*/
#pragma once
#include"toy_std.hpp"
#include"toyiterator.hpp"
#include"toytype_traits.hpp"
#include"toyalgo_base.hpp"

namespace toy_std
{
/* Internal Methods */
    // Uninitialized_copy
    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
    {
        // Case1: IS POD
        // Noted that the toy_std::copy has been implemented in "toyalgo_base.hpp".
        return copy(first, last, result);
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
    {
        // Case2: IS NOT POD
        ForwardIterator cur = result;
        for (; first != last; first++, cur++)
            construct(&(*cur), *first);
        return cur;
    }

    // Uninitialized_copy_n
    //template<typename InputIterator, typename ForwardIterator>
    //inline ForwardIterator __uninitialized_copy_n_aux(InputIterator first, size_t n, ForwardIterator result, __true_type)
    //{
    //  // Case1: IS POD
    //}

    //template<typename InputIterator, typename ForwardIterator>
    //ForwardIterator __uninitialized_copy_n_aux(InputIterator first, size_t n, ForwardIterator result, __false_type)
    //{
    //  // Case2: IS NOT POD
    //  ForwardIterator cur = result;
    //  for (size_t i = 0; i < n; i++, cur++)
    //      construct(&(*cur), *first);
    //  return cur;
    //}


    // Uninitialized_fill
    template<typename ForwardIterator, typename T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, T& x, __true_type)
    {
        // Case1: IS POD
        fill(first, last, x);
    }

    template<typename ForwardIterator, typename T>
    void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, T& x, __false_type)
    {
        // Case2: IS NOT POD
        for (; first != last; first++)
            construct(&(*first), x);
    }

    // Uninitialized_fill_n
    template<typename ForwardIterator, typename T>
    inline void __uninitialized_fill_n_aux(ForwardIterator first, size_t n, T& x, __true_type)
    {
        // Case1: IS POD
        fill_n(first, n, x);
    }

    template<typename ForwardIterator, typename T>
    void __uninitialized_fill_n_aux(ForwardIterator first, size_t n, T& x, __false_type)
    {
        // Case2: IS NOT POD
        for (size_t i = 0; i < n; ++i, ++first)
            construct(&(*first), x);
    }

    template<typename InputIterator, typename ForwardIterator, typename T>
    inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
    {
        using is_POD = typename __type_traits<T>::is_POD_type;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }

     /*
     template<typename InputIterator, typename ForwardIterator, typename T>
     inline ForwardIterator __uninitialized_copy_n(InputIterator first, size_t n, ForwardIterator result, T*)
     {
         using is_POD = typename __type_traits<T>::is_POD_type;
         return __uninitialized_copy_n_aux(first, n, result, is_POD());
     }
     */

    template<typename ForwardIterator, typename T, typename T1>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, T& x, T1*)
    {
        using is_POD = typename __type_traits<T1>::is_POD_type;
        __uninitialized_fill_aux(first, last, x, is_POD());
    }

    template<typename ForwardIterator, typename T, typename T1>
    inline void __uninitialized_fill_n(ForwardIterator first, size_t n, T& x, T1*)
    {
        using is_POD = typename __type_traits<T1>::is_POD_type;
        __uninitialized_fill_n_aux(first, n, x, is_POD());
    }

    

/* Memory manage tools */

    template<typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        return __uninitialized_copy(first, last, result, __value_type(result));
    }

     /*
     template<typename InputIterator, typename ForwardIterator>
     inline ForwardIterator uninitialized_copy_n(InputIterator first, size_t n, ForwardIterator result)
     {
         return __uninitialized_copy_n(first, n, result, __value_type(result));
     }
     */

    template<typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
    {
        __uninitialized_fill(first, last, x, __value_type(first));
    }

    template<typename ForwardIterator, typename T>
    inline void uninitialized_fill_n(ForwardIterator first, size_t n, const T& x)
    {
        __uninitialized_fill_n(first, n, x, __value_type(first));
    }

}
