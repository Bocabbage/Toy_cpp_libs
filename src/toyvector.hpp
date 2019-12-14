#pragma once
#include"toy_std.hpp"
#include"toymemory.hpp"
#include"toyiterator.hpp"

namespace toy_std
{
	template<typename T,
			 typename Allocator = tallocator<T>>
	class tvector
	{
	protected:


	public:
		/* Member Types */
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = Allocator::pointer;
		using const_pointer = Allocator::const_pointer;
		using iterator = T*;
		using const_iterator = const T*;
		using reference = T&;
		using const_reference = const T&;

		// Wait for implement:
		// using reverse_iterator = ;
		// using const_reverse_iterator = ;


		/* Member Functions */


	};
}