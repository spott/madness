#pragma once

#include <cassert>
#include <algorithm>
#include "madness/types.hpp"


template <typename T, bool SO>
struct Vector
{
private:
    T* _data;
    size_t _size;

public:

    //typedefs:
    typedef Vector<T,SO> This;
    typedef T            ValueType;
    typedef T*           Iterator;
    typedef const T*     ConstIterator;



    inline Vector() : _data(nullptr), _size(0) {};
    inline Vector( const size_t size ) :  _size(size) {
        _data = new T[size];
    };

    template <typename T2>
    inline Vector( const Vector<T2, SO>& other ) : _size(other._size) {
        _data = new T[_size];
        for(size_t i = 0; i < _size; ++i)
            _data[i] = static_cast<T2>(other[i]);
    };

    template<typename Operation, typename ... Ts>
    inline Vector( const VectorExpression< Operation, Ts...>& rhs ) 
    {
        _size = rhs.size();
        _data = new T[_size];

        for(size_t i = 0; i < _size; i++)
            _data[i] = static_cast<const T>(rhs[i]);
    };

    ~Vector() {
        delete[] _data;
        _size = 0;
    };

    size_t size() { return _size; };
    size_t size() const { return _size; };

    inline ValueType& operator[]( const size_t index ) { 
        assert ( index < _size );
        return _data[index];
    };

    inline const ValueType& operator[]( const size_t index ) const { 
        assert ( index < _size );
        return _data[index];
    };

    //template< typename T2>
    //inline This& operator=( const T2 scalar )
    //{
        //for (size_t i = 0; i < _size; ++i)
            //_data[i](scalar);
        ////for (auto& a : _data)
            ////a(scalar);
        
    //};

    template< typename T2>
    inline This& operator=( const Vector<T2,SO>& rhs )
    {
        delete[] _data;
        this( rhs );
        return *this;
    };

    template< typename Operation, typename... Ts>
    inline This&
    operator=( const VectorExpression< Operation, Ts...>& rhs ) 
    {
        delete[] _data;
        _size = rhs.size();
        _data = new T[_size];

        for(size_t i = 0; i < _size; i++)
            _data[i] = rhs[i];

        return *this;
    };

    inline Iterator begin() {
        return _data;
    };
    inline ConstIterator begin() const {
        return _data;
    };
    inline Iterator end() {
        return _data + _size;
    };
    inline ConstIterator end() const {
        return _data + _size;
    };

};
