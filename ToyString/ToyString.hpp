/*
    Project:        Toy_String
    Description:    Build a practice-aimed toy-module in C++
    Update date:    2019/11/13(unfinished)
    Author:         Zhuofan Zhang

    Update Log:     2019/11/13 -- replaced the former version with 'allocator version'.
*/
#pragma once
#include<cstring>
#include<iostream>
#include<memory>

using std::ostream;

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
class ToyBasicString
{
    // Member Types
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using iterator = CharType*;
    using reference = CharType&;

    template<typename X, typename A>
    friend ostream& operator<<(ostream&, const ToyBasicString<X,A>&);

private:
    size_type _capability;
    size_type _length;

    iterator _data;
    Allocator _alloc;

    // Remove 
    void _do_empty();

public:
    // Constructors
    ToyBasicString();
    ToyBasicString(const CharType*);
    ToyBasicString(size_type, const char);
    ToyBasicString(const ToyBasicString<CharType,Allocator>&);
    ToyBasicString<CharType,Allocator>& operator=(const ToyBasicString<CharType,Allocator>&);

    // Destructor
    ~ToyBasicString();

    // Capability
    bool empty() { return _length == 0; }
    size_type length() { return _length; }
    size_type capability() { return _capability; }

    // Element Access
    CharType& operator[](const size_type);
    CharType& at(const size_type);
    CharType& front();
    CharType& back();
    const CharType* c_str() const;

    // Operations
    
};

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType,Allocator>::_do_empty()
{
    if (!this->empty())
    {
        auto tmp = _data + _capability;
        _length = 0;
        _capability = 0;
        while (tmp != _data)
            _alloc.destroy(tmp--);
        _alloc.deallocate(_data, _capability + 1);
    }

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType,Allocator>::ToyBasicString(): 
    _data(nullptr), _capability(0), _length(0),_alloc()
{ }

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(size_type len, const char s):
    _alloc(), _capability(len<<1), _length(len)
{
    _data = _alloc.allocate(_capability + 1);
    _alloc.construct(_data, _length, s);
    _alloc.construct(_data + _length, '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const CharType* s):
    _alloc(),_capability(strlen(s)<<1),_length(strlen(s))
{
    _data = _alloc.allocate(_capability + 1);
    auto p = _data;
    for (size_t i = 0; i <= _length; ++i)
        _alloc.construct(p++, s[i]);
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const ToyBasicString<CharType,Allocator>& t) :
    _alloc(), _capability(t._capability), _length(t._length)
{
    _data = _alloc.allocate(_capability + 1);
    auto p = _data;
    for (size_t i = 0; i <= _length; ++i)
        _alloc.construct(p++, t._data[i]);

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::operator=(const ToyBasicString<CharType, Allocator>& t)
{
    this->_do_empty();
    _length = t._length;
    _capability = t._capability;
    _data = _alloc.allocate(_capability + 1);
    auto p = _data;
    for (size_t i = 0; i <= _length; ++i)
        _alloc.construct(p++, t._data[i]);

    return this;
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::~ToyBasicString()
{
    this->_do_empty();
}

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
ostream&
operator<<(ostream& os, const ToyBasicString<CharType, Allocator> &s)
{
    return os << s._data;
}

template<typename CharType,
         typename Allocator >
CharType&
ToyBasicString<CharType, Allocator>::operator[](const size_type idx)
{
    return _data[idx];
}

template<typename CharType,
         typename Allocator >
CharType&
ToyBasicString<CharType, Allocator>::at(const size_type idx)
{
    if (idx >= _length)
        exit(1);
    return _data[idx];
}

template<typename CharType,
         typename Allocator >
const CharType*
ToyBasicString<CharType, Allocator>::c_str() const
{
    return _data;
}

template<typename CharType,
         typename Allocator >
CharType&
ToyBasicString<CharType, Allocator>::front()
{
    return _data[0];
}

template<typename CharType,
         typename Allocator >
CharType&
ToyBasicString<CharType, Allocator>::back()
{
    return _data[_length - 1];
}