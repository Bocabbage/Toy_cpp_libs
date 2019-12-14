#pragma once
#include"toy_std.hpp"
#include<cstring>


namespace toy_std
{
	/* c-string like functions that can be used in different chartype strings. */
	template<typename CharType>
	size_t
		Tstrlen(const CharType* str)
	{
		const CharType* _tmp = str;
		while (*_tmp != 0)
			_tmp++;
		return _tmp - str;
	}

	template<typename CharType>
	int
		Tstrcmp(const CharType* _Str1, const CharType _Str2)
	{
		while (*_Str1 != 0 && !*_Str2 != 0)
		{
			if (*_Str1 < *_Str2)
				return -1;
			else if (*_Str1 > *_Str2)
				return 1;
			else
			{
				_Str1++; _Str2++;
			}
		}

		if (*_Str1 != 0)
			return 1;
		if (*_Str2 != 0)
			return -1;
		return 0;
	}

	template<typename CharType>
	CharType*
		Tstrcpy(CharType* dest, const CharType* src)
	{
		// I change the type of src to const
		// to adapt the const-source string.
		auto _ret = dest;
		while (*dest++ = *src++);
		return _ret;
	}

	template<typename CharType>
	const CharType*
		Tstrstr(const CharType* str, const CharType* target)
	{
		const size_t _target_len = Tstrlen<CharType>(target);
		const size_t _str_len = Tstrlen<CharType>(str);
		if (_target_len > _str_len)
		{
			std::cerr << "pattern can't longer than the string." << std::endl;
			exit(-2);
		}
		const size_t _end = _str_len - _target_len + 1;
		const CharType* _s, * _p;
		for (size_t i = 0; i < _end; i++)
		{
			_s = str + i, _p = target;
			while (*_p != 0)
			{
				if (*_p != *_s)
					break;
				_p++; _s++;
			}
			if (*_p != 0)
				continue;
			return str + i;
		}

		return nullptr;
	}

	template<typename CharType>
	CharType*
		Tstrstr(CharType* str, CharType* target)
	{
		const CharType* _str = str, *_target = target;
		return const_cast<CharType*>(Tstrstr<CharType>(_str, _target));
	}

	

}