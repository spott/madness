#pragma once

#include <cassert>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include "madness/types.hpp"
#include "madness/type_traits.hpp"

namespace madness {

template <typename T, bool SO = false>
struct SparseMatrix : Matrix<SparseMatrix>
{
private:
    T* _data;
    size_t* _column_indice;
    size_t* _row_start_indice;
    size_t _size;
    size_t _rows;
    size_t _columns;
    //something to reference when there isn't anything in that location:
    constexpr static const T _zero = T(); 

public:

    //typedefs:
    typedef SparseMatrix<T,SO> This;
    typedef T                  ValueType;

    //Constructors:
    inline SparseMatrix() : 
        _data(nullptr), 
        _column_indice(nullptr), 
        _row_start_indice(nullptr), 
        _size(0),
        _rows(0),
        _columns(0) {};

    inline SparseMatrix( const size_t rows, const size_t columns, const size_t non_zeros ) : 
        _size(non_zeros), _rows(rows), _columns(columns) {
        _data = new T[_size];
        _column_indice = new size_t[_size];
        _row_start_indice = new size_t[_rows];
    };

    //Destructors:
    ~SparseMatrix() {
        delete[] _data;
        delete[] _column_indice;
        delete[] _row_start_indice;
    }

    //zero indexed access for an element
    inline T& operator() (const size_t Row, const size_t Col)
    {
        size_t tmp = _row_start_indice[Row];
        for (; tmp < ((Row == _rows - 1) ? _size : _row_start_indice[Row]); ++tmp)
            if (_column_indice[tmp] == Col)
                return _data[tmp];
        return _zero;
    }

    //zero indexed access for an element
    inline T& operator() (const size_t Row, const size_t Col) const
    {
        size_t tmp = _row_start_indice[Row];
        for (; tmp < ((Row == _rows - 1) ? _size : _row_start_indice[Row]); ++tmp)
            if (_column_indice[tmp] == Col)
                return _data[tmp];
        return _zero;
    }

    //multiply every value by a constant:
    template <typename T2>
    inline This& operator*(const T2& rhs)
    {
        for( size_t i = 0; i < _size; ++i)
            _data[i] = static_cast<T>(rhs);
        return *this;
    }

    template< typename T2, bool SO2 = false >
    static inline SparseMatrix<T2, SO2> diagonal ( size_t size, const T2& value )
    {
        SparseMatrix< T2, SO2 > tmp( size, size, size );
        for (int a = size - 1; a >= 0; --a)
        {
            tmp._data[a] = value;
            tmp._row_start_indice[a] = a;
            tmp._column_indice[a] = a;
        }

        return tmp;
    }

    template< typename T2, bool SO2 = false >
    static inline SparseMatrix<T2, SO2> tridiagonal( size_t size, const T2& L, const T2& D, const T2& U )
    {
        SparseMatrix< T2, SO2 > tmp( size, size, 3 * size - 2 );

        for (unsigned int i = 0; i < size; i++)
        {
            if (i == 0) //the first row is weird
            {
                tmp._row_start_indice[i] = i;
                tmp._data[i] = D;
                tmp._data[i+1] = U;
                tmp._column_indice[i] = i;
                tmp._column_indice[i+1] = i+1;
                continue;
            }
            if (i == size-1) // the last row is weird
            {
                tmp._row_start_indice[i] = 3 * i - 1;
                tmp._data[ tmp._row_start_indice[i] ] = L;
                tmp._data[ tmp._row_start_indice[i] + 1 ] = D;
                tmp._column_indice[ tmp._row_start_indice[i] ] = i - 1;
                tmp._column_indice[ tmp._row_start_indice[i] + 1 ] = i;
                continue;
            }
            //the rest:
            tmp._row_start_indice[i] = 3 * i - 1;
            tmp._data[ tmp._row_start_indice[i] ] = L;
            tmp._data[ tmp._row_start_indice[i] + 1 ] = D;
            tmp._data[ tmp._row_start_indice[i] + 2] = U;
            tmp._column_indice[ tmp._row_start_indice[i] ] = i - 1;
            tmp._column_indice[ tmp._row_start_indice[i] + 1 ] = i;
            tmp._column_indice[ tmp._row_start_indice[i] + 2 ] = i + 1;
            continue;
        }


        return tmp;
    }

    void Debug_Print()
    {
        //iterate over the matrix and output it, one row per line:
        for (unsigned int i = 0; i < _rows; i++)
        {
            std::cout << "Row " << i << ":";
            for(unsigned int j = _row_start_indice[i]; j < (i < _rows-1 ? _row_start_indice[i+1] : _size ); j++)
                std::cout << "(" << _column_indice[j] << "," << i << "): " << _data[j] << "\t";
            std::cout << std::endl;
        }
    }

};

}
