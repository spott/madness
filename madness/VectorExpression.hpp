#pragma once

#include<complex>
#include <type_traits>
#include "madness/type_traits.hpp"
#include "madness/types.hpp"

namespace madness {

template < typename Operation, typename Va>
struct VectorExpression < Operation, Va >
{
    friend Va;
    typedef VectorExpression< Operation, Va>  This;

    typedef Conditional< is_vector_type< unqualified<Va> >, typename Va::ValueType, Va>     ValueType;
    const Operation& op;
    const Va& vector;

    inline VectorExpression ( Operation oper, const Va& first) : 
        op(oper), 
        vector(first) 
    {};

    inline auto size() const ->decltype( vector.size() ) {
        return vector.size();
    };

    template <typename T = Va, enable_if< is_vector_type< unqualified<T> >::value, void >... >
    inline auto operator[]( size_t i ) const -> decltype( vector[i] ) {
        return vector[i];
        };

    template <typename T = Va, disable_if< is_vector_type< unqualified<T> >::value, void >... >
    inline auto operator[]( size_t i ) const -> decltype( vector ) {
        return vector;
        };
};

template < typename Operation, typename Va, typename ... Vs>
struct VectorExpression <Operation, Va, Vs...> : protected VectorExpression< Operation, Vs...>
{
    friend Va;
    const Operation op;
    const Va& vector;
    typedef VectorExpression< Operation, Va, Vs...>     This;
    typedef VectorExpression< Operation, Vs... >        Base;
    typedef Conditional< is_vector_type< unqualified<Va> >, typename Va::ValueType, Va>     VaType;
    typedef decltype( op( VaType(), typename Base::ValueType() ) )    ValueType;


    inline VectorExpression ( Operation oper, const Va& first, const Vs&... vs ) : 
        VectorExpression< Operation, Vs...>(oper, vs...), 
        op(oper), 
        vector(first) 
    {};

    inline auto size() const ->decltype( vector.size() ) {
        return vector.size();
    };

    template <typename T = Va, enable_if< is_vector_type< unqualified<T> >::value, void >... >
    inline auto operator[]( size_t i ) const
        ->decltype( op( 
                    vector[i] , 
                    this->Base::operator[](i) 
                    ) 
                ) {
        return op( vector[i], Base::operator[](i) );
        };

    template <typename T = Va, disable_if< is_vector_type< unqualified<T> >::value, void >... >
    inline auto operator[]( size_t i ) const
        ->decltype( op( 
                    vector, 
                    this->Base::operator[](i)
                    ) ) {
            static_assert( !is_vector_type< Va >::value, "vector isn't a vector type... " );
            return op( vector, Base::operator[](i) );
        };
};


template < typename R, typename L >
struct add {
inline auto operator()( const R& a, const L& b) const -> decltype( R() + L() )
{
    return a + b;
}};

template < typename R, typename L >
struct sub {
inline auto operator()( const R& a, const L& b) const -> decltype( R() - L() )
{
    return a - b;
}};

template < typename R, typename L >
struct mult {
inline auto operator()( const R& a, const L& b) const -> decltype( R() * L() )
{
    return a * b;
}};

//Add using VectorExpression:
template < typename T, typename T2 >
VectorExpression< add < typename T::ValueType, typename T2::ValueType >, T, T2>
inline operator+( const T& rhs, const T2& lhs ) 
{
    return VectorExpression<  add< typename T::ValueType, typename T2::ValueType >, T, T2 >
        ( add< typename T::ValueType, typename T2::ValueType >() , rhs, lhs );
};

template < typename T, typename T2 >
VectorExpression< sub< typename T::ValueType, typename T2::ValueType >, T, T2>
inline operator-( const T& rhs, const T2& lhs ) 
{
    return VectorExpression<  sub< typename T::ValueType, typename T2::ValueType >, T, T2 >
        ( sub< typename T::ValueType, typename T2::ValueType >() , rhs, lhs );
};

template < typename T, typename T2 >
inline auto operator*( const T& rhs, const T2& lhs ) 
    -> typename VectorExpression< 
            mult< 
                Conditional< is_vector_type< unqualified<T>  >, typename T::ValueType , T >, 
                Conditional< is_vector_type< unqualified<T2> >, typename T2::ValueType, T2>
            >, T, T2>
{
    return 
        VectorExpression< 
            mult< 
                Conditional< is_vector_type< unqualified<T>  >, typename T::ValueType , T >, 
                Conditional< is_vector_type< unqualified<T2> >, typename T2::ValueType, T2>
            >, T, T2 >
                ( mult< 
                    Conditional< is_vector_type< unqualified<T>  >, typename T::ValueType , T >, 
                    Conditional< is_vector_type< unqualified<T2> >, typename T2::ValueType, T2>
                  >(), rhs, lhs );
};

}
