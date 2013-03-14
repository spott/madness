#pragma once

#include<type_traits>
#include "madness/types.hpp"
#include "madness/Vector.hpp"
#include "madness/VectorExpression.hpp"

namespace madness{

//some basics:
template <typename If, typename Then, typename Else>
using Conditional = typename std::conditional<If::value, Then, Else>::type;

template <bool B, typename...>
struct dependent_bool_type : std::integral_constant<bool, B> {};
// and an alias, just for kicks :)
template <bool B, typename... T>
using Bool = typename dependent_bool_type<B, T...>::type;

// Meta-logical negation
template <typename T>
using Not = Bool<!T::value>;

// Meta-logical disjunction
template <typename... T>
struct any : Bool<false> {};
template <typename Head, typename... Tail>
struct any<Head, Tail...> : Conditional<Head, Bool<true>, any<Tail...>> {};

// Meta-logical conjunction
template <typename... T>
struct all : Bool<true> {};
template <typename Head, typename... Tail>
struct all<Head, Tail...> : Conditional<Head, all<Tail...>, Bool<false>> {};


// remove constant/volatile and reference markers:
template <typename T>
using unqualified = typename std::remove_cv< typename std::remove_reference< T >::type >::type ;

static_assert( std::is_arithmetic< unqualified<double> >::value, "unqualified double isn't arithmetic" );
static_assert( std::is_arithmetic< unqualified<const double> >::value, "unqualified const double isn't arithmetic" );
static_assert( std::is_arithmetic< unqualified<double&> >::value, "unqualified double& isn't arithmetic" );
static_assert( std::is_arithmetic< unqualified<const double&> >::value, "unqualified const double& isn't arithmetic" );

// enable if typedef to make things cleaner:
template <bool B, typename T>
using enable_if = typename std::enable_if< B, T >::type;
template <bool B, typename T>
using disable_if = typename std::enable_if< ! B, T >::type;

static_assert( enable_if< true, std::true_type >::value , " enable_if doesn't work " );
static_assert( disable_if< false, std::true_type >::value , " disable_if doesn't work " );



// test for straight vector:
template <typename V>
struct is_vector : std::false_type { }; 
template <typename T1,bool T2>
struct is_vector< Vector<T1,T2> > : std::true_type { };

static_assert( is_vector< Vector< double, false > >::value, "is_vector doesn't work" );

//test if it is a vector type (either a vector or a vectorexpression)
template <typename V>
struct is_vector_expression : std::false_type { }; 
template <typename T1, typename T2, typename...Ts>
struct is_vector_expression< VectorExpression<T1,T2,Ts...> > : std::true_type { };

static_assert( !is_vector_expression< Vector< double, false > >::value, "is_vector_expression doesn't work on vectors" );
static_assert( is_vector_expression< 
                VectorExpression< 
                    mult<std::complex<double>, double>, 
                    Vector<double, false>, 
                    Vector<std::complex<double>, false>  
                > 
               >::value, "is_vector_type doesn't work on vectorexpressions" );
static_assert( !is_vector_expression< std::complex<double> >::value, "is_vector_type calls complex<doubles> vector_types" );

template <typename V>
struct is_vector_type : 
    Conditional< any< is_vector<V>, is_vector_expression<V> >, std::true_type, std::false_type> { };

static_assert( is_vector_type< Vector< double, false > >::value, "is_vector_expression doesn't work on vectors" );
static_assert( is_vector_type< 
                VectorExpression< 
                    mult<std::complex<double>, double>, 
                    Vector<double, false>, 
                    Vector<std::complex<double>, false>  
                > 
               >::value, "is_vector_type doesn't work on vectorexpressions" );
static_assert( !is_vector_type< std::complex<double> >::value, "is_vector_type calls complex<doubles> vector_types" );
static_assert( !is_vector_type< double >::value, "is_vector_type calls complex<doubles> vector_types" );

template < typename V, typename = void>
struct base_type { typedef V type; };
template < typename V >
struct base_type< V, 
                 typename base_type<void, typename V::ValueType>::type > { 
    typedef typename V::ValueType type; 
};

static_assert( std::is_same < typename base_type< Vector< double, false > >::type , double >::value, "base_type doesn't work" ); 
static_assert( std::is_same < typename base_type< double >::type , double >::value, "base_type doesn't work" ); 


}
