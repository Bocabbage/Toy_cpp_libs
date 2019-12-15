/*
	Project:        Toy_Type_Traits
	Update date:    2019/12/15
	Author:         Zhuofan Zhang

	Notes:(Refer to 'The Annotated STL sources' book)
		This is NOT an internal header file.
		You can attempt to use and modify it directly.
*/
#pragma once
#define __STL_TEMPLATE_NULL template<>
#include"toy_std.hpp"

namespace toy_std
{
	struct __true_type {};
	struct __false_type {};

	/* Type-traits template */
	template<typename T>
	struct __type_traits
	{
		// See book: The Annotated STL sources,Chapter3.7
		using THIS_DUMMY_MEMBER_MUST_BE_FIRST = __true_type;

		using has_trivival_default_constructor = __false_type;
		using has_trivival_copy_constructor = __false_type;
		using has_trivival_assignment_operator = __false_type;
		using has_trivival_destructor = __false_type;
		using is_POD_type = __false_type;
	};

	/* Specialization version of the Type-traits template */
	__STL_TEMPLATE_NULL struct __type_traits<char>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<signed char>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned char>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<short>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned short>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<int>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned int>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned long>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long long>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<float>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<double>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long double>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};

	// For pointers' partial specialization version
	template<typename T>
	struct __type_traits<T*>
	{
		using has_trivival_default_constructor = __true_type;
		using has_trivival_copy_constructor = __true_type;
		using has_trivival_assignment_operator = __true_type;
		using has_trivival_destructor = __true_type;
		using is_POD_type = __true_type;
	};


}