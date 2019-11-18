/*
    Project:        Toy_String
    Description:    Build a practice-aimed toy-module in C++
    Update date:    2019/11/18
    Author:         Zhuofan Zhang

    Update Log:     2019/11/13 -- replaced the former version with 'allocator version'.
                    2019/11/14 -- Add 'erase'.
                    2019/11/16 -- Debug; Change the 'end' position.
                    2019/11/18 -- Change the model: '\0' is now placed in end();


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
#include<cstring>
#include<iostream>
#include<memory>
using std::ostream;
using std::uninitialized_copy;



template<typename CharType,
         typename Allocator = std::allocator<CharType> >
class ToyBasicString
{
    /* Member Types */
    typedef Allocator allocator_type;
    typedef std::size_t size_type;
    typedef CharType value_type;
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
    allocator_type _alloc;


    /* Remove */
    void _do_destroy();



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

    ToyBasicString<CharType, Allocator> substr(size_type,size_type);
};

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType,Allocator>::_do_destroy()
{
    // It's different from 'clear()':
    //  It deallocate the _data and set it to nullptr.
    if (_data!=nullptr)
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

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType, Allocator>::resize(size_type new_cap)
{
    if (new_cap == _capability)
        return;

    iterator _new_data = _alloc.allocate(new_cap + 1);
    if (new_cap > _capability)
        uninitialized_copy(begin(), end()+1, _new_data);
    else if (new_cap < _capability)
    {
        uninitialized_copy(begin(), begin() + new_cap, _new_data);
        if(new_cap < _length)
            _length = new_cap;
        _new_data[_length] = '\0';
        
    }
    _alloc.deallocate(_data, _capability + 1);
    _data = _new_data;
    _capability = new_cap;

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType,Allocator>::ToyBasicString(): 
    _alloc(), _capability(_default_capability), _length(0), _data(nullptr)
{ 
    _data = _alloc.allocate(_capability + 1);
    _alloc.construct(_data, '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(size_type len, const char s):
    _alloc(), _capability(len<<1), _length(len), _data(nullptr)
{
    _data = _alloc.allocate(_capability + 1);
    _alloc.construct(_data, _length, s);
    _alloc.construct(_data + _length, '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const_iterator s):
    _alloc(),_capability(strlen(s)<<1),_length(strlen(s)), _data(nullptr)
{
    _data = _alloc.allocate(_capability + 1);
    uninitialized_copy(s, s + strlen(s) + 1, _data);
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const ToyBasicString<CharType,Allocator>& t) :
    _alloc(), _capability(t._capability), _length(t._length), _data(nullptr)
{
    _data = _alloc.allocate(_capability + 1);
    uninitialized_copy(t.cbegin(), t.cend()+1, _data);

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::operator=(const ToyBasicString<CharType, Allocator>& t)
{
    _do_destroy();
    _length = t._length;
    _capability = t._capability;
    _data = _alloc.allocate(_capability + 1);
    uninitialized_copy(t.cbegin(),t.cend()+1,_data);
    return this;
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::~ToyBasicString()
{
    _do_destroy();
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
typename ToyBasicString<CharType, Allocator>::const_iterator
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
    if (pos >= _data && pos < end())
    {
        auto _end = end();
        for (auto p = pos; p < _end; ++p)
            *p = *(p+1);
        _alloc.destroy(_end);
        --_length;

        return pos;
    }
    else
        return end();
    
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::iterator
ToyBasicString<CharType, Allocator>::erase(iterator first, iterator last)
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

template<typename CharType,
         typename Allocator >
void 
ToyBasicString<CharType, Allocator>::clear()
{
    erase(begin(), end());
}

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType, Allocator>::shrink_to_fit()
{
    if (_length < _capability)
    {
        auto _new_data = _alloc.allocate(_length + 1);
        auto _new_cap = _length;
        uninitialized_copy(begin(), end()+1, _new_data);

        _do_destroy();

        _capability = _new_cap;
        _length = _new_cap;
        _data = _new_data;

    }
}

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType, Allocator>::push_back(value_type c)
{
    if (_length == _capability)
        resize(_capability << 1);
    _data[_length++] = c;
    _data[_length] = '\0';
}

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType, Allocator>::pop_back()
{
    erase(end() - 1);
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>
ToyBasicString<CharType, Allocator>::substr(size_type pos, size_type n)
{
    if (pos + n > _length)
        exit(1);
    // Unfinished
}