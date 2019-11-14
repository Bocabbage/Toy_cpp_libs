/*
    Project:        Toy_String
    Description:    Build a practice-aimed toy-module in C++
    Update date:    2019/11/14
    Author:         Zhuofan Zhang

    Update Log:     2019/11/13 -- replaced the former version with 'allocator version'.
                    2019/11/14 -- Add 'erase'.
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
    /* Member Types */
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef CharType* iterator;
    typedef CharType& reference;
    typedef const CharType* const_iterator;
    typedef const CharType& const_reference;

    /* Constants */
    const size_type _default_capability = 15;

    template<typename X, typename A>
    friend ostream& operator<<(ostream&, const ToyBasicString<X,A>&);


private:
    size_type _capability;
    size_type _length;

    iterator _data;
    Allocator _alloc;


    /* Remove */
    void _do_empty();


public:
    /* Constructors */
    ToyBasicString();
    ToyBasicString(const_iterator);
    ToyBasicString(size_type, const char);
    ToyBasicString(const ToyBasicString<CharType,Allocator>&);
    ToyBasicString<CharType,Allocator>& operator=(const ToyBasicString<CharType,Allocator>&);


    /* Destructor */
    ~ToyBasicString();


    /* Capability */
    bool empty() { return _length == 0; }
    size_type length() { return _length; }
    size_type capability() { return _capability; }


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

    const_reference c_str() const;


    /* Operations */
    void clear();
    
    iterator erase(iterator);
    iterator erase(iterator, iterator);



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
    _alloc(), _capability(_default_capability), _length(0)
{ 
    _data = _alloc.allocate(_capability + 1);
}

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
ToyBasicString<CharType, Allocator>::ToyBasicString(const_iterator s):
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
typename ToyBasicString<CharType, Allocator>::const_reference 
ToyBasicString<CharType, Allocator>::operator[](const size_type idx) const
{
    return _data[idx];
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::reference
ToyBasicString<CharType, Allocator>::operator[](const size_type idx)
{
    return const_cast<reference>(
                                    static_cast<const ToyBasicString<CharType, Allocator>&>(*this)[idx]
                                );
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::const_reference
ToyBasicString<CharType, Allocator>::at(const size_type idx) const
{
    if (idx >= _length)
        exit(1);
    return _data[idx];
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::reference
ToyBasicString<CharType, Allocator>::at(const size_type idx)
{
    return const_cast<reference>(
                                    static_cast<const ToyBasicString<CharType, Allocator>&>(*this).at(idx)
                                );
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::const_reference
ToyBasicString<CharType, Allocator>::c_str() const
{
    return _data;
}

template<typename CharType,
    typename Allocator >
typename ToyBasicString<CharType, Allocator>::const_reference
ToyBasicString<CharType, Allocator>::front() const
{
    return _data[0];
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::reference
ToyBasicString<CharType, Allocator>::front()
{
    return const_cast<reference>(
                                    static_cast<const ToyBasicString<CharType, Allocator>&>(*this).front()
                                );
}

template<typename CharType,
    typename Allocator >
typename ToyBasicString<CharType, Allocator>::const_reference
ToyBasicString<CharType, Allocator>::back() const
{
    return _data[_length - 1];
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::reference
ToyBasicString<CharType, Allocator>::back()
{
    return const_cast<reference>(
                                    static_cast<const ToyBasicString<CharType, Allocator>&>(*this).back()
                                );
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::iterator
ToyBasicString<CharType, Allocator>::erase(iterator pos)
{
    if (pos >= end() || pos < begin())
        exit(1);
    for (auto p = pos ; p < end(); ++p)
        *p = *(p + 1);
    _alloc.destroy(end());
    --_length;

    return pos;
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::iterator
ToyBasicString<CharType, Allocator>::erase(iterator first, iterator last)
{
    if (first < last && first >= begin() && last <= end())
    {
        auto p = first, q = last;
        while (q <= end())
            *(p++) = *(q++);
        while (q != p)
        {
            _alloc.destroy(q--);
            --_length;
        }

        return first;
    }
    else if (first == last)
        return end();
    else
        exit(1);
    

}

template<typename CharType,
         typename Allocator >
void 
ToyBasicString<CharType, Allocator>::clear()
{
    erase(begin(), end());
}