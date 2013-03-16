#pragma once

#include <cassert>
#include <algorithm>
#include <initializer_list>
#include "madness/types.hpp"
#include "madness/type_traits.hpp"

namespace madness {

template <typename T, bool SO = false>
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



    //Constructors:
    inline Vector() : _data(nullptr), _size(0) {};
    inline Vector( const size_t size ) :  _size(size) {
        _data = new T[size];
    };

    inline Vector( std::initializer_list<T> list ) : _size(list.size()) {
        _data = new T[_size];
        std::copy( list.begin(), list.end(), _data );
    };


    //Copy Constructor:
    template <typename T2>
    inline Vector( const Vector<T2, SO>& rhs ) : _size(rhs.size() ) {
        _data = new T[_size];
        for(size_t i = 0; i < _size; ++i)
            _data[i] = static_cast<T2>(rhs[i]);
    }

    //Expression Constructor
    template<typename Operation, typename ... Ts>
    inline Vector( const VectorExpression< Operation, Ts...>& rhs ) : _size(rhs.size() )
    {
        _data = new T[_size];

        for(size_t i = 0; i < _size; i++)
            _data[i] = rhs[i];
    }

    //Destructor:
    ~Vector() {
        delete[] _data;
        _size = 0;
    }

    //Size functions:
    inline size_t size() { return _size; };
    inline size_t size() const { return _size; };

    //operator[]
    inline ValueType& operator[]( const size_t index ) { 
        assert ( index < _size );
        return _data[index];
    }

    inline const ValueType& operator[]( const size_t index ) const { 
        assert ( index < _size );
        return _data[index];
    }

    //operator='s
    template< typename T2>
    inline This& operator=( const T2 scalar )
    {
        for (size_t i = 0; i < _size; ++i)
            _data[i] = T(scalar);
        return *this;
    }

    inline This& operator=( std::initializer_list<T> list)
    {
        delete[] _data;
        _size = list.size();
        _data = new T[_size];
        std::copy( list.begin(), list.end(), _data );
        return *this;
    }

    template< typename T2>
    inline This& operator=( const Vector<T2,SO>& rhs ) {
        if ( _size == rhs.size() )
            for(size_t i = 0; i < _size; i++)
                _data[i] = rhs[i];
        else
        {
            delete[] _data;
            _size = rhs.size();
            _data = new T[_size];
            for(size_t i = 0; i < _size; i++)
                _data[i] = rhs[i];
        }
        return *this;
    }

    template< typename Operation, typename... Ts>
    inline This&
    operator=( const VectorExpression< Operation, Ts...>& rhs ) {
        if ( _size == rhs.size() )
            for(size_t i = 0; i < _size; i++)
                _data[i] = rhs[i];
        else
        {
            delete[] _data;
            _size = rhs.size();
            _data = new T[_size];
            for(size_t i = 0; i < _size; i++)
                _data[i] = rhs[i];
        }
        return *this;
    }

    //operator +=
    template< typename T2>
    inline This& operator+=( const T2 scalar )
    {
        for (size_t i = 0; i < _size; ++i)
            _data[i] += T(scalar);
        return *this;
    }

    template< typename T2, enable_if< is_vector_type< unqualified<T2> >::value, void > >
    inline This& operator+=( const T2& rhs ) {
        assert ( this->size() == rhs.size() );
        for (size_t i = 0; i < _size; ++i)
            _data[i] += T( rhs[i] );
        return *this;
    }

    //operator -=
    template< typename T2>
    inline This& operator-=( const T2 scalar )
    {
        for (size_t i = 0; i < _size; ++i)
            _data[i] -= T(scalar);
        return *this;
    }

    template< typename T2, enable_if< is_vector_type< unqualified<T2> >::value, void > >
    inline This& operator-=( const T2& rhs ) {
        assert ( this->size() == rhs.size() );
        for (size_t i = 0; i < _size; ++i)
            _data[i] -= T( rhs[i] );
        return *this;
    }

    //operator *=
    template< typename T2>
    inline This& operator*=( const T2 scalar )
    {
        for (size_t i = 0; i < _size; ++i)
            _data[i] *= T(scalar);
        return *this;
    }

    template< typename T2, enable_if< is_vector_type< unqualified<T2> >::value, void > >
    inline This& operator*=( const T2& rhs ) {
        assert ( this->size() == rhs.size() );
        for (size_t i = 0; i < _size; ++i)
            _data[i] *= T( rhs[i] );
        return *this;
    }

    //template< typename T2, enable_if< is_vector_type< unqualified<T2> >::value, void > >
    template< typename T2 >
    inline bool operator==( const Vector< T2, SO> & rhs ) {
        if ( size() != rhs.size() )
            return false;
        else
            for (size_t i = 0; i < _size; i++)
                if ( _data[i] != T( rhs[i] ) )
                    return false;
        return true;
    }

    //Iterators:
    inline Iterator begin() {
        return _data;
    }
    inline ConstIterator begin() const {
        return _data;
    }
    inline Iterator end() {
        return _data + _size;
    }
    inline ConstIterator end() const {
        return _data + _size;
    }

};

}
