/*
    Project:        Toy_String
    Description:    Build a practice-aimed toy-module in C++
    Update date:    2019/11/22
    Author:         Zhuofan Zhang

    Update Log:     2019/11/13 -- replaced the former version with 'allocator version'.
                    2019/11/14 -- Add 'erase'.
                    2019/11/16 -- Debug; Change the 'end' position.
                    2019/11/18 -- Change the model: '\0' is now placed in end();
                    2019/11/22 -- Add Exceptions process.


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
#include<stdexcept>
using std::ostream;
using std::uninitialized_copy;
using std::initializer_list;
using std::range_error;

/* Exception Codes */
const int RANGE_ERROR = 2;


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
    static const size_type _default_capability = 15;
    static const size_type _npos = -1;

    /* Non-member functions  */
    template<typename X, typename A>
    friend ostream& operator<<(ostream&, const ToyBasicString<X,A>&);

    template<typename X, typename A>
    friend ToyBasicString<X, A> operator+(const ToyBasicString<X, A>&, const ToyBasicString<X, A>&);

    // Comparison
    template<typename X, typename A>
    friend bool operator==(const ToyBasicString<X, A>&, const ToyBasicString<X, A>&);

    template<typename X, typename A>
    friend bool operator!=(const ToyBasicString<X, A>& a, const ToyBasicString<X, A>& b) { return !(a == b); }

    template<typename X, typename A>
    friend bool operator<(const ToyBasicString<X, A>&, const ToyBasicString<X, A>&);

    template<typename X, typename A>
    friend bool operator>(const ToyBasicString<X, A>&, const ToyBasicString<X, A>&);

    template<typename X, typename A>
    friend bool operator<=(const ToyBasicString<X, A>& a, const ToyBasicString<X, A>& b) { return !(a > b); }

    template<typename X, typename A>
    friend bool operator>=(const ToyBasicString<X, A>& a, const ToyBasicString<X, A>& b) { return !(a < b); }


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
    ToyBasicString(const_iterator, const_iterator);
    ToyBasicString(size_type, const char);
    ToyBasicString(const ToyBasicString<CharType,Allocator>&);
    ToyBasicString<CharType,Allocator>& operator=(const ToyBasicString<CharType,Allocator>&);
    ToyBasicString(initializer_list<value_type>);


    /* Destructor */
    ~ToyBasicString() noexcept;


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

    ToyBasicString<CharType, Allocator>& append(const ToyBasicString<CharType, Allocator>&);
    ToyBasicString<CharType, Allocator>& append(const_iterator);

    int compare(const ToyBasicString<CharType, Allocator>&);

    size_type copy(CharType*, size_type, size_type) const;

    void swap(ToyBasicString<CharType, Allocator>&);

    ToyBasicString<CharType, Allocator>& insert(size_type, const_iterator);

    size_type find(size_type, const_iterator) const;
    size_type find(size_type pos, const ToyBasicString<CharType, Allocator>& str) const
    { return find(pos, str._data);}

    size_type find_first_of(size_type, const_iterator) const;
    size_type find_first_of(size_type pos, const ToyBasicString<CharType, Allocator>& str) const
    { return find_first_of(pos, str._data); }

    ToyBasicString<CharType, Allocator>& operator+=(const ToyBasicString<CharType, Allocator>& str)
    { return this->append(str); }
    ToyBasicString<CharType, Allocator>& operator+=(const_iterator s)
    { return this->append(s); }


};

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType,Allocator>::_do_destroy()
{
    // It's different from 'clear()':
    // It deallocates the _data and sets it to nullptr.
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
    _alloc(), _capability(_default_capability), _length(0), _data(_alloc.allocate(_capability + 1))
{ 
    _alloc.construct(_data, '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(size_type len, const char s):
    _alloc(), _capability(len<<1), _length(len), _data(_alloc.allocate(_capability + 1))
{
    _alloc.construct(_data, _length, s);
    _alloc.construct(end(), '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const_iterator s):
    _alloc(), _length(strlen(s)), _capability(strlen(s) << 1), _data(_alloc.allocate(_capability + 1))
{
    uninitialized_copy(s, s + strlen(s) + 1, _data);
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const_iterator first, const_iterator last):
    _alloc(), _length(last - first) ,_capability((last - first) << 1), _data(_alloc.allocate(_capability + 1))
{
    uninitialized_copy(first, last, _data);
    _alloc.construct(end(), '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(initializer_list<value_type> ilist):
    _alloc(), _length(ilist.size()), _capability(ilist.size() << 1), _data(_alloc.allocate(_capability + 1))
{
    uninitialized_copy(ilist.begin(), ilist.end(), _data);
    _alloc.construct(end(), '\0');
}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::ToyBasicString(const ToyBasicString<CharType,Allocator>& t) :
    _alloc(), _capability(t._capability), _length(t._length), _data(_alloc.allocate(_capability + 1))
{
    uninitialized_copy(t.cbegin(), t.cend()+1, _data);

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::operator=(const ToyBasicString<CharType, Allocator>& t)
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
    ToyBasicString<CharType, Allocator> _tmp(t);
    swap(_tmp);
    return *this;

}

template<typename CharType,
         typename Allocator >
ToyBasicString<CharType, Allocator>::~ToyBasicString() noexcept
{
    _do_destroy();
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
    try
    {
        if (pos + n > _length)
            throw range_error("RANGE_ERROR: pos + n must be small than length() or equal it.");
        
        return ToyBasicString<CharType, Allocator>(_data + pos, _data + pos + n);

    }   
    catch(range_error err)
    {
        std::cout << std::endl << err.what() << std::endl;
        exit(RANGE_ERROR);
    }
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::append(const ToyBasicString<CharType, Allocator>& str)
{
    return this->append(str._data);
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::append(const_iterator s)
{
    size_type _new_length = _length + strlen(s);
    if (_new_length > _capability)
        resize(_new_length << 1);
    uninitialized_copy(s, s+ strlen(s) + 1, end());
    _length = _new_length;
    return *this;
}

template<typename CharType,
         typename Allocator >
int
ToyBasicString<CharType, Allocator>::compare(const ToyBasicString<CharType, Allocator>& str)
{
    return strcmp(_data, str._data);
}

template<typename CharType,
         typename Allocator >
void
ToyBasicString<CharType, Allocator>::swap(ToyBasicString<CharType, Allocator>& str)
{
    // pimpl: Pointer to Implementation
    std::swap(_alloc, str._alloc);
    std::swap(_data, str._data);    // !
    std::swap(_length, str._length);
    std::swap(_capability, str._capability);
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>&
ToyBasicString<CharType, Allocator>::insert(size_type idx, const_iterator s)
{
    auto s_len = strlen(s);
    
    if (_length + s_len > _capability)
        resize((_length + s_len) << 1);

    uninitialized_copy(_data + idx, end()+1, _data + idx + s_len);
    uninitialized_copy(s, s + s_len, _data + idx);
    

    _length += s_len;
    return *this;
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::size_type
ToyBasicString<CharType, Allocator>::copy(CharType* dest, size_type count, size_type pos) const
{
    // By default, we recognize 'dest' points to a constructed mem.
    if (pos + count > _length - 1)
    {
        // uninitialized_copy(_data + pos, _data + _length + 1, dest);
        strcpy(dest, _data + pos);
        return _length - pos;
    }
    else
    {
        uninitialized_copy(_data + pos, _data + pos + count, dest);
        dest[count] = '\0';
        return count;
    }
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::size_type
ToyBasicString<CharType, Allocator>::find(size_type pos, const_iterator s) const
{
    auto _res = strstr(_data + pos, s);
    if (_res)
        return _res - _data;
    else
        return _npos;
}

template<typename CharType,
         typename Allocator >
typename ToyBasicString<CharType, Allocator>::size_type
ToyBasicString<CharType, Allocator>::find_first_of(size_type pos, const_iterator s) const
{
    auto _res = strpbrk(_data + pos, s);
    if (_res)
        return _res - _data;
    else
        return _npos;
}


/* Friends */

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
ostream&
operator<<(ostream& os, const ToyBasicString<CharType, Allocator>& s)
{
    return os << s._data;
}

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
ToyBasicString<CharType, Allocator>
operator+(const ToyBasicString<CharType, Allocator>& a, ToyBasicString<CharType, Allocator>& b)
{
    ToyBasicString<CharType, Allocator> _res(a._data);
    _res.append(b._data);
    return _res;
}

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
bool
operator==(const ToyBasicString<CharType, Allocator>& a, ToyBasicString<CharType, Allocator>& b)
{
    return strcmp(a._data, b._data) == 0;
}

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
bool
operator<(const ToyBasicString<CharType, Allocator>& a, ToyBasicString<CharType, Allocator>& b)
{
    return strcmp(a._data, b._data) < 0;
}

template<typename CharType,
         typename Allocator = std::allocator<CharType> >
bool
operator>(const ToyBasicString<CharType, Allocator>& a, ToyBasicString<CharType, Allocator>& b)
{
    return strcmp(a._data, b._data) > 0;
}


// Effective cpp: item 25
template<typename T>
void swap(ToyBasicString<T>& a, ToyBasicString<T>& b)
{
    a.swap(b);
}