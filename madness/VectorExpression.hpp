#pragma once

#include<complex>
#include <type_traits>
#include "madness/types.hpp"
#include "madness/type_traits.hpp"

namespace madness {

// operators
template < typename R, typename L >
struct add {
inline auto operator()( const R& a, const L& b) const -> decltype( R() + L() ) {
    return a + b;
}};

template < typename R, typename L >
struct sub {
inline auto operator()( const R& a, const L& b) const -> decltype( R() - L() ) {
    return a - b;
}};

template < typename R, typename L >
struct mult {
inline auto operator()( const R& a, const L& b) const -> decltype( R() * L() ) {
    return a * b;
}};


template < typename Operation, typename Va>
struct VectorExpression < Operation, Va >
{
public:
    friend Va;
    //constexpr static const Operation op = O;

    const Va& vector;
    typedef VectorExpression< Operation, Va>  This;
    typedef typename base_type< Va >::type ValueType;


private:
    //SFINAE function for operator[]
    template <typename T = Va, typename R = unqualified<decltype( std::declval<T>()[0] )> >
    inline const R subscript_helper(size_t i, int) const { return vector[i]; }

    inline const Va subscript_helper(size_t i, ...) const { return vector; }

public:
    inline VectorExpression ( const Va& first) : 
        vector(first) {}

    template <typename T = Va, typename R = decltype(std::declval<T>().size() )>
    inline const R size() const {
        return vector.size();
    }

    inline auto operator[]( size_t i ) const -> decltype( this->subscript_helper(0,0) ) {
        return subscript_helper(i,0);
    }
};

template < typename Operation, typename Va, typename ... Vs>
struct VectorExpression <Operation, Va, Vs...> : protected VectorExpression< Operation, Vs...>
{
public:
    friend Va;
    constexpr static const Operation op = Operation();
    const Va& vector;
    typedef VectorExpression< Operation, Va, Vs...>     This;
    typedef VectorExpression< Operation, Vs... >        Base;
    typedef typename base_type< Va >::type              VaType;

    typedef decltype( std::declval<Operation>()( VaType() , typename Base::ValueType() ) ) ValueType;

private:
    //SFINAE function for operator[] (we want to call different versions if T is a scalar or vector type
    template <typename T = Va, typename R = unqualified<decltype( std::declval<T>()[0] )> >
    inline const R & subscript_helper(size_t i, int) const { return vector[i]; }

    inline const Va& subscript_helper(size_t i, ...) const { return vector; }

    //SFINAE function for size() (we don't want to try to call size on a scalar...
    template < typename T = Va, typename R = decltype(std::declval<T>().size() ) >
    inline R size_helper(int) const { return vector.size(); }

    inline auto size_helper(...) const -> decltype(this->Base::size()) { return Base::size(); }

public:
    inline VectorExpression ( const Va& first, const Vs&... vs ) : 
        VectorExpression< Operation, Vs...>(vs...), 
        vector(first) {}

    inline auto size() const ->decltype( this->size_helper(0) ) {
        return size_helper(0);
    }

    inline const ValueType operator[]( size_t i ) const {
        return this->op( subscript_helper(i,0), Base::operator[](i) );
    }
};

//This is required to prevent the debug build from failing at linking... not sure why...
template <typename Operation, typename Va, typename ... Vs>
constexpr const Operation VectorExpression<Operation, Va, Vs...>::op;

//overloads:  return a vector expression, and make sure the operator is templated on the base types 
//(underlying type) not the Vector or VectorExpression types.
template < typename LHS, typename RHS, 
           typename LHS_TYPE = unqualified< typename LHS::ValueType >,
           typename RHS_TYPE = unqualified< typename RHS::ValueType > >
inline VectorExpression< add < LHS_TYPE , RHS_TYPE>, LHS, RHS>
operator+( const LHS& lhs, const RHS& rhs ) {
    return VectorExpression<  add< LHS_TYPE, RHS_TYPE >, LHS, RHS >
        ( lhs, rhs );
}

template < typename LHS, typename RHS, 
           typename LHS_TYPE = unqualified< typename LHS::ValueType >,
           typename RHS_TYPE = unqualified< typename RHS::ValueType > >
inline VectorExpression< sub < LHS_TYPE , RHS_TYPE>, LHS, RHS>
operator-( const LHS& lhs, const RHS& rhs ) {
    return VectorExpression<  sub< LHS_TYPE, RHS_TYPE >, LHS, RHS >
        ( lhs, rhs );
}

//Overloads for mult.  We can mult with scalars, so we need to SFINAE this for scalar on the left or right.
template < typename RHS, typename LHS, 
           typename LHS_TYPE = unqualified< typename LHS::ValueType >,
           typename RHS_TYPE = unqualified< typename RHS::ValueType > >
inline auto operator*( const RHS& rhs, const LHS& lhs ) 
    -> VectorExpression<mult< RHS_TYPE, LHS_TYPE >, RHS, LHS>
{
    return 
        VectorExpression< mult< RHS_TYPE, LHS_TYPE >, RHS, LHS >
                ( rhs, lhs );
}

template < typename RHS, typename LHS, 
           typename RHS_TYPE = unqualified< typename RHS::ValueType >,
           typename LHS_TYPE = unqualified< LHS > ,
           disable_if< is_vector_type< unqualified<LHS> >::value, void>... >
inline auto operator*( const RHS& rhs, const LHS& lhs ) 
    -> VectorExpression<mult< RHS_TYPE, LHS_TYPE >, RHS, LHS>
{
    return 
        VectorExpression< mult< RHS_TYPE, LHS_TYPE >, RHS, LHS >
                (rhs, lhs );
}

template < typename RHS, typename LHS, 
           typename RHS_TYPE = unqualified<RHS>,
           typename LHS_TYPE = unqualified< typename LHS::ValueType  >,
           disable_if< is_vector_type< unqualified<RHS> >::value, void>... >
inline auto operator*( const RHS& rhs, const LHS& lhs )
    -> VectorExpression< mult< RHS_TYPE, LHS_TYPE >, RHS, LHS>
{
    return 
        VectorExpression< mult< RHS_TYPE, LHS_TYPE >, RHS, LHS >
                (rhs, lhs );
}

}
