/*
    Project:        Toy_String
    Description:    Build a practice-aimed toy-module in C++
    Update date:    2019/11/10
    Author:         Zhuofan Zhang
*/
#pragma once
#include<cstring>
#include<iostream>

using std::ostream;

template<typename CharType>
class ToyBasicString
{

    template<typename X>
    friend ostream& operator<<(ostream&, const ToyBasicString<X>&);

private:
    size_t _capability;
    size_t _length;

    CharType* _data;

    // Remove 
    void _do_empty();

public:
    // Constructors( Don't 'inline' them! )
    ToyBasicString();
    ToyBasicString(const char*);
    ToyBasicString(size_t, const char);
    ToyBasicString(const ToyBasicString<CharType>&);
    ToyBasicString<CharType>& operator=(const ToyBasicString<CharType>&);

    // Destructor
    ~ToyBasicString();

    // Capability
    bool empty() { return _length == 0; }
    size_t length() { return _length; }
    size_t capability() { return _capability; }


};

template<typename CharType>
void
ToyBasicString<CharType>::_do_empty()
{
    if (!this->empty())
    {
        _length = 0;
        _capability = 0;
        delete[] _data;
    }

}

template<typename CharType>
ToyBasicString<CharType>::ToyBasicString(): 
    _data(nullptr), _capability(0), _length(0)
{ }

template<typename CharType>
ToyBasicString<CharType>::ToyBasicString(size_t len, const char s): 
    _data(new CharType[len + 1]), _capability(len), _length(len)
{
    for (size_t i = 0; i < _length; ++i)
        _data[i] = s;
    _data[len] = '\0';
}

template<typename CharType>
ToyBasicString<CharType>::ToyBasicString(const char* s):
    _data(new CharType[strlen(s)+1]), _capability(strlen(s)) , _length(strlen(s))
{
    strcpy(_data, s);
}

template<typename CharType>
ToyBasicString<CharType>::ToyBasicString(const ToyBasicString<CharType>& t) :
    _data(new CharType[t._capability + 1]), _capability(t._capability), _length(t._length)
{
    strcpy(_data, t._data);
}

template<typename CharType>
ToyBasicString<CharType>&
ToyBasicString<CharType>::operator=(const ToyBasicString<CharType>& t)
{
    this->_do_empty();
    _length = t._length;
    _capability = t._capability;
    _data = new CharType[_capability + 1];
    strcpy(_data, t._data);

    return this;
}

template<typename CharType>
ToyBasicString<CharType>::~ToyBasicString()
{
    this->_do_empty();
}

template<typename CharType>
ostream&
operator<<(ostream& os, const ToyBasicString<CharType> &s)
{
    return os << s._data;
}

