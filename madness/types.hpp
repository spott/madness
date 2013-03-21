#pragma once

//Forward declarations:

namespace madness {

template < typename Operation, typename ... Va>
struct VectorExpression;

template < typename L, typename R >
struct mult;
template < typename L, typename R >
struct add;
template < typename L, typename R >
struct sub;

template <typename T, bool SO >
struct Vector;

template <typename MatType, bool SO >
struct Matrix {
    typedef MatType type;
}


template <typename T, bool SO >
struct SparseMatrix;

}
