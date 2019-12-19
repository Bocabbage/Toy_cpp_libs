/*
	Project:        Toy_String
	Description:    Build a practice-aimed toy-module in C++
	Update date:    2019/12/14
	Author:         Zhuofan Zhang

	Update Log:     2019/11/13 -- replaced the former version with 'allocator version'.
					2019/11/14 -- Add 'erase'.
					2019/11/16 -- Debug; Change the 'end' position.
					2019/11/18 -- Change the model: '\0' is now placed in end();
					2019/11/22 -- Add Exceptions process.
					2019/11/24 -- Add moving constructor/ moving =
					2019/12/14 -- Add overload of >>; replace the <cstring> function with "toycstring".


	Model:

		_data    -------
				|       | <-- begin()
				|-------|
				|  ...  |
				|       |
				|-------|
				|  \0   | <-- _data + _length [end()]
				|-------|
				| unuse |
				|-------|
				|  ...  |
				|-------|
				| unuse | <-- _data + _capability
				 -------

		mem_use: _capability + 1
*/
#pragma once
#include"toy_std.hpp"
#include"toycstring.hpp"
using std::ostream;
using std::istream;
using std::uninitialized_copy;
using std::initializer_list;
using std::range_error;

// temporarily used
#include<unordered_set>

/* Exception Codes */
const int RANGE_ERROR = 2;
const size_t INPUT_INIT_SIZE = 128;

namespace toy_std
{


	template<typename CharType,
		typename Allocator = std::allocator<CharType> >
		class tbasic_string
	{

	public:
		/* Member Types */
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using value_type = CharType;
		using iterator = CharType*;
		using reference = CharType&;
		using const_iterator = const CharType*;
		using const_reference = const CharType&;

		/* Constants */
		static const size_type _default_capability = 15;
		static const size_type _npos = -1;

		/* Non-member functions  */
		template<typename X, typename A>
		friend ostream& operator<<(ostream&, const tbasic_string<X, A>&);

		template<typename X, typename A>
		friend istream& operator>>(istream&, tbasic_string<X, A>&);

		template<typename X, typename A>
		friend tbasic_string<X, A> operator+(const tbasic_string<X, A>&, const tbasic_string<X, A>&);

		// Comparison
		template<typename X, typename A>
		friend bool operator==(const tbasic_string<X, A>&, const tbasic_string<X, A>&);

		template<typename X, typename A>
		friend bool operator!=(const tbasic_string<X, A>& a, const tbasic_string<X, A>& b) { return !(a == b); }

		template<typename X, typename A>
		friend bool operator<(const tbasic_string<X, A>&, const tbasic_string<X, A>&);

		template<typename X, typename A>
		friend bool operator>(const tbasic_string<X, A>&, const tbasic_string<X, A>&);

		template<typename X, typename A>
		friend bool operator<=(const tbasic_string<X, A>& a, const tbasic_string<X, A>& b) { return !(a > b); }

		template<typename X, typename A>
		friend bool operator>=(const tbasic_string<X, A>& a, const tbasic_string<X, A>& b) { return !(a < b); }


	private:
		size_type _capability;
		size_type _length;

		iterator _data;
		allocator_type _alloc;


		/* Remove */
		void _do_destroy();



	public:
		/* Constructors */
		tbasic_string();
		tbasic_string(const_iterator);
		tbasic_string(const_iterator, const_iterator);
		tbasic_string(size_type, const char);
		tbasic_string(const tbasic_string<CharType, Allocator>&);
		tbasic_string(const tbasic_string<CharType, Allocator>&&) noexcept;
		tbasic_string<CharType, Allocator>& operator=(const tbasic_string<CharType, Allocator>&);
		tbasic_string<CharType, Allocator>& operator=(const_iterator);
		tbasic_string<CharType, Allocator>& operator=(const tbasic_string<CharType, Allocator>&&) noexcept;
		tbasic_string(initializer_list<value_type>);


		/* Destructor */
		~tbasic_string() noexcept;


		/* Capability */
		bool empty() { return _length == 0; }
		size_type length() { return _length; }
		size_type capability() { return _capability; }
		void shrink_to_fit();


		/* Iterators */
		iterator begin() { return _data; }
		const_iterator cbegin() const { return _data; }
		iterator end() { return _data + _length; }
		const_iterator cend() const { return _data + _length; }


		/* Element Access */
		const_reference operator[](const size_type)const;
		reference operator[](const size_type);

		const_reference at(const size_type) const;
		reference at(const size_type);

		const_reference front() const;
		reference front();

		const_reference back() const;
		reference back();

		const_iterator c_str() const;


		/* Operations */
		void clear();

		iterator erase(iterator);
		iterator erase(iterator, iterator);

		void resize(size_type);

		void push_back(value_type);
		void pop_back();

		tbasic_string<CharType, Allocator> substr(size_type, size_type);

		tbasic_string<CharType, Allocator>& append(const tbasic_string<CharType, Allocator>&);
		tbasic_string<CharType, Allocator>& append(const_iterator);

		int compare(const tbasic_string<CharType, Allocator>&);

		size_type copy(CharType*, size_type, size_type) const;

		void swap(tbasic_string<CharType, Allocator>&);

		tbasic_string<CharType, Allocator>& insert(size_type, const_iterator);

		size_type find(size_type, const_iterator) const;
		size_type find(size_type pos, const tbasic_string<CharType, Allocator>& str) const
		{
			return find(pos, str._data);
		}

		size_type find_first_of(size_type, const_iterator) const;
		size_type find_first_of(size_type pos, const tbasic_string<CharType, Allocator>& str) const
		{
			return find_first_of(pos, str._data);
		}

		tbasic_string<CharType, Allocator>& operator+=(const tbasic_string<CharType, Allocator>& str)
		{
			return this->append(str);
		}
		tbasic_string<CharType, Allocator>& operator+=(const_iterator s)
		{
			return this->append(s);
		}


	};

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::_do_destroy()
	{
		// It's different from 'clear()':
		// It deallocates the _data and sets it to nullptr.
		if (_data != nullptr)
		{
			auto _end_cap = end() + 1;
			for (auto _tmp = _data; _tmp < _end_cap; ++_tmp)
				_alloc.destroy(_tmp);
			_alloc.deallocate(_data, _capability + 1);

			// Still not sure if this is needed:
			_data = nullptr;

			_length = 0;
			_capability = 0;
		}

	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::resize(size_type new_cap)
	{
		if (new_cap == _capability)
			return;

		iterator _new_data = _alloc.allocate(new_cap + 1);
		if (new_cap > _capability)
			uninitialized_copy(begin(), end() + 1, _new_data);
		else if (new_cap < _capability)
		{
			uninitialized_copy(begin(), begin() + new_cap, _new_data);
			if (new_cap < _length)
				_length = new_cap;
			_new_data[_length] = '\0';

		}
		_alloc.deallocate(_data, _capability + 1);
		_data = _new_data;
		_capability = new_cap;

	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string() :
		_alloc(), _capability(_default_capability), _length(0), _data(_alloc.allocate(_capability + 1))
	{
		_alloc.construct(_data, '\0');
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(size_type len, const char s) :
		_alloc(), _capability(len << 1), _length(len), _data(_alloc.allocate(_capability + 1))
	{
		_alloc.construct(_data, _length, s);
		_alloc.construct(end(), '\0');
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(const_iterator s) :
		_alloc(), _length(Tstrlen<CharType>(s)), _capability(Tstrlen<CharType>(s) << 1), _data(_alloc.allocate(_capability + 1))
	{
		uninitialized_copy(s, s + Tstrlen<CharType>(s) + 1, _data);
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(const_iterator first, const_iterator last) :
		_alloc(), _length(last - first), _capability((last - first) << 1), _data(_alloc.allocate(_capability + 1))
	{
		uninitialized_copy(first, last, _data);
		_alloc.construct(end(), '\0');
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(initializer_list<value_type> ilist) :
		_alloc(), _length(ilist.size()), _capability(ilist.size() << 1), _data(_alloc.allocate(_capability + 1))
	{
		uninitialized_copy(ilist.begin(), ilist.end(), _data);
		_alloc.construct(end(), '\0');
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(const tbasic_string<CharType, Allocator>& t) :
		_alloc(), _capability(t._capability), _length(t._length), _data(_alloc.allocate(_capability + 1))
	{
		uninitialized_copy(t.cbegin(), t.cend() + 1, _data);

	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::tbasic_string(const tbasic_string<CharType, Allocator>&& rt) noexcept :
		_alloc(rt._alloc), _capability(rt._capability), _length(rt._length), _data(rt._data)
	{
		rt._data = nullptr;
		rt._length = rt._capability = 0;
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::operator=(const tbasic_string<CharType, Allocator>& t)
	{
		/*
		_do_destroy();
		_length = t._length;
		_capability = t._capability;
		_data = _alloc.allocate(_capability + 1);
		uninitialized_copy(t.cbegin(),t.cend()+1,_data);
		return *this;
		*/

		/* New Version: self-assignment-safe */
		tbasic_string<CharType, Allocator> _tmp(t);
		swap(_tmp);
		return *this;

	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::operator=(const_iterator s)
	{
		tbasic_string<CharType, Allocator> _tmp(s);
		swap(_tmp);
		return *this;


	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::operator=(const tbasic_string<CharType, Allocator>&& rt) noexcept
	{
		tbasic_string<CharType, Allocator> _tmp(rt);
		swap(_tmp);
		return *this;
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>::~tbasic_string() noexcept
	{
		_do_destroy();
	}



	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::const_reference
		tbasic_string<CharType, Allocator>::operator[](const size_type idx) const
	{
		return _data[idx];
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::reference
		tbasic_string<CharType, Allocator>::operator[](const size_type idx)
	{
		return const_cast<reference>(
			static_cast<const tbasic_string<CharType, Allocator>&>(*this)[idx]
			);
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::const_reference
		tbasic_string<CharType, Allocator>::at(const size_type idx) const
	{
		try
		{
			if (idx >= _length)
				throw range_error("RANGE_ERROR: the index must in range [0,length()).");
			return _data[idx];
		}
		catch (range_error err)
		{
			std::cout << std::endl << err.what() << std::endl;
			exit(RANGE_ERROR);
		}

	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::reference
		tbasic_string<CharType, Allocator>::at(const size_type idx)
	{
		return const_cast<reference>(
			static_cast<const tbasic_string<CharType, Allocator>&>(*this).at(idx)
			);
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::const_iterator
		tbasic_string<CharType, Allocator>::c_str() const
	{
		return _data;
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::const_reference
		tbasic_string<CharType, Allocator>::front() const
	{
		return _data[0];
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::reference
		tbasic_string<CharType, Allocator>::front()
	{
		return const_cast<reference>(
			static_cast<const tbasic_string<CharType, Allocator>&>(*this).front()
			);
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::const_reference
		tbasic_string<CharType, Allocator>::back() const
	{
		return _data[_length - 1];
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::reference
		tbasic_string<CharType, Allocator>::back()
	{
		return const_cast<reference>(
			static_cast<const tbasic_string<CharType, Allocator>&>(*this).back()
			);
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::iterator
		tbasic_string<CharType, Allocator>::erase(iterator pos)
	{
		if (pos >= _data && pos < end())
		{
			auto _end = end();
			for (auto p = pos; p < _end; ++p)
				*p = *(p + 1);
			_alloc.destroy(_end);
			--_length;

			return pos;
		}
		else
			return end();

	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::iterator
		tbasic_string<CharType, Allocator>::erase(iterator first, iterator last)
	{
		if (first < last && first >= _data && last <= end())
			// Natually including the condition that '\0' can't be removed 
		{
			auto p = first, q = last;
			auto _end = end();
			while (q <= _end)
				*(p++) = *(q++);
			p--; q--;
			while (q != p)
			{
				_alloc.destroy(q--);
				--_length;
			}

			return first;
		}
		else
			return end();


	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::clear()
	{
		erase(begin(), end());
	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::shrink_to_fit()
	{
		if (_length < _capability)
		{
			auto _new_data = _alloc.allocate(_length + 1);
			auto _new_cap = _length;
			uninitialized_copy(begin(), end() + 1, _new_data);

			_do_destroy();

			_capability = _new_cap;
			_length = _new_cap;
			_data = _new_data;

		}
	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::push_back(value_type c)
	{
		if (_length == _capability)
			resize(_capability << 1);
		_data[_length++] = c;
		_data[_length] = '\0';
	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::pop_back()
	{
		erase(end() - 1);
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>
		tbasic_string<CharType, Allocator>::substr(size_type pos, size_type n)
	{
		try
		{
			if (pos + n > _length)
				throw range_error("RANGE_ERROR: pos + n must be small than length() or equal it.");

			return tbasic_string<CharType, Allocator>(_data + pos, _data + pos + n);

		}
		catch (range_error err)
		{
			std::cout << std::endl << err.what() << std::endl;
			exit(RANGE_ERROR);
		}
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::append(const tbasic_string<CharType, Allocator>& str)
	{
		return this->append(str._data);
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::append(const_iterator s)
	{
		size_type _new_length = _length + Tstrlen<CharType>(s);
		if (_new_length > _capability)
			resize(_new_length << 1);
		uninitialized_copy(s, s + Tstrlen<CharType>(s) + 1, end());
		_length = _new_length;
		return *this;
	}

	template<typename CharType, typename Allocator >
	int
		tbasic_string<CharType, Allocator>::compare(const tbasic_string<CharType, Allocator>& str)
	{
		return Tstrcmp<CharType>(_data, str._data);
	}

	template<typename CharType, typename Allocator >
	void
		tbasic_string<CharType, Allocator>::swap(tbasic_string<CharType, Allocator>& str)
	{
		// pimpl: Pointer to Implementation
		std::swap(_alloc, str._alloc);
		std::swap(_data, str._data);    // !
		std::swap(_length, str._length);
		std::swap(_capability, str._capability);
	}

	template<typename CharType, typename Allocator >
	tbasic_string<CharType, Allocator>&
		tbasic_string<CharType, Allocator>::insert(size_type idx, const_iterator s)
	{
		auto s_len = Tstrlen<CharType>(s);

		if (_length + s_len > _capability)
			resize((_length + s_len) << 1);

		uninitialized_copy(_data + idx, end() + 1, _data + idx + s_len);
		uninitialized_copy(s, s + s_len, _data + idx);


		_length += s_len;
		return *this;
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::size_type
		tbasic_string<CharType, Allocator>::copy(CharType* dest, size_type count, size_type pos) const
	{
		// By default, we recognize 'dest' points to a constructed mem.
		if (pos + count > _length - 1)
		{
			// uninitialized_copy(_data + pos, _data + _length + 1, dest);
			Tstrcpy<CharType>(dest, _data + pos);
			return _length - pos;
		}
		else
		{
			uninitialized_copy(_data + pos, _data + pos + count, dest);
			dest[count] = '\0';
			return count;
		}
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::size_type
		tbasic_string<CharType, Allocator>::find(size_type pos, const_iterator s) const
	{
		auto _res = Tstrstr<CharType>(_data + pos, s);
		if (_res)
			return _res - _data;
		else
			return _npos;
	}

	template<typename CharType, typename Allocator >
	typename tbasic_string<CharType, Allocator>::size_type
		tbasic_string<CharType, Allocator>::find_first_of(size_type pos, const_iterator s) const
	{
		try
		{
			if (pos >= _length)
				throw range_error("RANGE_ERROR: the index must in range [0,length()).");

			// Wait for implement of container: unordered_set
			std::unordered_set<CharType> _uset;
			while (*s != 0)
				_uset.insert(*s++);

			auto _tmp = _data + pos;
			while (_tmp != cend())
			{
				if (_uset.find(*_tmp) != _uset.end())
					return _tmp - _data;
				_tmp++;
			}
			return _npos;

		}
		catch (range_error err)
		{
			std::cout << std::endl << err.what() << std::endl;
			exit(RANGE_ERROR);
		}

		/*
		auto _res = strpbrk(_data + pos, s);
		if (_res)
			return _res - _data;
		else
			return _npos;
		*/
	}


	/* Friends */

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	ostream&
		operator<<(ostream& os, const tbasic_string<CharType, Allocator>& s)
	{
		return os << s._data;
	}

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	istream&
		operator>>(istream& is, tbasic_string<CharType, Allocator>& s)
	{
		const size_t __Bytes = sizeof(CharType);
		
		bool _stop_sign = false;
		char* _CH = new char[__Bytes];
		CharType* _CHs = new CharType[INPUT_INIT_SIZE + 1];

		size_t _read_nums = 0;
		size_t i = 0;
		for(; i < INPUT_INIT_SIZE; ++i)
		{
			for (size_t j = 0; j < __Bytes; j++)
			{
				is.get(_CH[j]);
				if (_CH[j] == ' ' || _CH[j] == '\n')
					_stop_sign = true;
			}
			if (_stop_sign)
				break;
			else
				uninitialized_copy(_CH, _CH + __Bytes, &(_CHs[i]));

		}
		_CHs[i] = (CharType)0;

		tbasic_string<CharType, Allocator> _tmp(_CHs);
		s.swap(_tmp);

		delete[] _CH;
		delete[] _CHs;
		return is;
	}

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	tbasic_string<CharType, Allocator>
		operator+(const tbasic_string<CharType, Allocator>& a, tbasic_string<CharType, Allocator>& b)
	{
		tbasic_string<CharType, Allocator> _res(a._data);
		_res.append(b._data);
		return _res;
	}

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	bool
		operator==(const tbasic_string<CharType, Allocator>& a, tbasic_string<CharType, Allocator>& b)
	{
		return Tstrcmp<CharType>(a._data, b._data) == 0;
	}

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	bool
		operator<(const tbasic_string<CharType, Allocator>& a, tbasic_string<CharType, Allocator>& b)
	{
		return Tstrcmp<CharType>(a._data, b._data) < 0;
	}

	template<typename CharType, typename Allocator = std::allocator<CharType> >
	bool
		operator>(const tbasic_string<CharType, Allocator>& a, tbasic_string<CharType, Allocator>& b)
	{
		return Tstrcmp<CharType>(a._data, b._data) > 0;
	}


	// Effective cpp: item 25
	template<typename T>
	void swap(tbasic_string<T>& a, tbasic_string<T>& b)
	{
		a.swap(b);
	}

}