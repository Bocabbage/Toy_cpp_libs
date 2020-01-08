/*
	Project:        Toy_Iterator
	Update date:    2019/12/11
	Author:         Zhuofan Zhang

	Iterator Models:

			InputIterator		OutputIterator
				\					/
				 \				   /
				   ForwardIterator
						  |
						  |
				 BidirectionIterator
						  |
						  |
				RandomAccessIterator
*/
#pragma once
#include"toy_std.hpp"

namespace toy_std
{
	/* Iterator categories' tags */
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirection_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirection_iterator_tag {};


	/* Origin Iterator template */
	/*
	template<typename IterCategory,
			 typename T,
			 typename Pointer = T*,
			 typename Reference = T&,
			 typename Distance = ptrdiff_t>
	class iterator
	{
	public:
		using iterator_category = IterCategory;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using difference_type = Distance;
	};
	*/

	/* Traits */
	template<typename Iter>
	class iterator_traits
	{
	public:
		using iterator_category = typename Iter::iterator_category;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using difference_type = typename Iter::difference_type;
	};

	// Partial Specialization version for native pointer
	template<typename T>
	class iterator_traits<T*>
	{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = ptrdiff_t;
	};

	// Partial Specialization version for const native pointer
	template<typename T>
	class iterator_traits<const T*>
	{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using difference_type = ptrdiff_t;
	};


	/* Functions for type-checking */
	template<typename Iter>
	inline typename iterator_traits<Iter>::iterator_category
		__iterator_category(const Iter&)
	{
		typename iterator_traits<Iter>::iterator_category category;
		return category;
	}

	template<typename Iter>
	inline typename iterator_traits<Iter>::value_type*
		__value_type(const Iter&)
	{
		return static_cast<typename iterator_traits<Iter>::value_type*>(0);
	}

	template<typename Iter>
	inline typename iterator_traits<Iter>::difference_type*
		__difference_type(const Iter&)
	{
		return static_cast<typename iterator_traits<Iter>::difference_type*>(0);
	}


	/* distance function */
	template<typename InputIter>
	inline typename iterator_traits<InputIter>::difference_type
		__distance(InputIter first, InputIter last, input_iterator_tag)
	{
		typename iterator_traits<InputIter>::difference_type n = 0;
		while (first != last)
		{
			first++; n++;
		}
		return n;
	}

	template<typename RandomAccessIter>
	inline typename iterator_traits<RandomAccessIter>::difference_type
		__distance(RandomAccessIter first, RandomAccessIter last, random_access_iterator_tag)
	{
		return last - first;
	}

	template<typename Iter>
	inline typename iterator_traits<Iter>::difference_type
		distance(Iter first, Iter last)
	{
		return __distance(first, last, __difference_type(first));
	}


	/* advanced function */
	template<typename InputIter, typename Distance>
	inline void
		__advanced(InputIter it, Distance n, input_iterator_tag)
	{
		while (n > 0)
		{
			it++; n--;
		}
	}

	template<typename BidirIter, typename Distance>
	inline void
		__advanced(BidirIter it, Distance n, bidirection_iterator_tag)
	{
		if (n >= 0)
			while (n > 0)
			{
				it++; n--;
			}
		else
			while (n < 0)
			{
				it--; n++;
			}
	}

	template<typename RandomAccessIter, typename Distance>
	inline void
		__advanced(RandomAccessIter it, Distance n, random_access_iterator_tag)
	{
		it += n;
	}

	template<typename Iter, typename Distance>
	inline void
		advanced(Iter it, Distance n)
	{
		__advanced(it, n, __iterator_category(it));
	}
}