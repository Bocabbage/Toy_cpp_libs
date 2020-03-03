#pragma once
/*
    Project:        Toy_STL_Function
    Update date:    2020/3/3
    Author:         Zhuofan Zhang

*/
#include"toy_std.hpp"

namespace toy_std
{
    template<typename Args, typename Result>
    struct unary_function
    {
        using argument_type = Args;
        using result_type = Result;
    };

    template<typename Args1, typename Args2, typename Result>
    struct binary_function
    {
        using first_argment_type = Args1;
        using second_argument_type = Args2;
        using result_type = Result;
    };

    /* Arthmetic Functors */
    template<typename T>
    struct plus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) { return x + y; }
    };

    template<typename T>
    struct minus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) { return x - y; }
    };

    template<typename T>
    struct multiplies : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) { return x * y; }
    };

    template<typename T>
    struct divides : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) { return x / y; }
    };

    template<typename T>
    struct modulus: public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) { return x % y; }
    };

    template<typename T>
    struct negate : public unary_function<T, T>
    {
        T operator()(const T& x) { return -x; }
    };

    /* Relational Functors */
    template<typename T>
    struct equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x == y; }
    };

    template<typename T>
    struct not_equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x != y; }
    };

    template<typename T>
    struct greater : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x > y; }
    };

    template<typename T>
    struct greater_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x >= y; }
    };

    template<typename T>
    struct less : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x < y; }
    };

    template<typename T>
    struct less_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x <= y; }
    };

    /* Logical Functors */
    template<typename T>
    struct logical_and : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x && y; }
    };

    template<typename T>
    struct logical_or : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) { return x || y; }
    };

    template<typename T>
    struct logical_not : public unary_function<T, bool>
    {
        bool operator()(const T& x) { return !x; }
    };
}