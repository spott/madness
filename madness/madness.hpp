#pragma once

//include all the files that we need:

#include "madness/type_traits.hpp"
#include "madness/Vector.hpp"
#include "madness/VectorExpression.hpp"


namespace madness {
static_assert( !is_vector_expression< Vector< double, false > >::value, "is_vector_expression doesn't work on vectors" );
static_assert( is_vector_expression< 
                VectorExpression< 
                    mult<std::complex<double>, double>, 
                    Vector<double, false>, 
                    Vector<std::complex<double>, false>  
                > 
               >::value, "is_vector_type doesn't work on vectorexpressions" );
static_assert( !is_vector_expression< std::complex<double> >::value, "is_vector_type calls complex<doubles> vector_types" );


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


static_assert( std::is_same < typename base_type< Vector< double, false > >::type , double >::value, "base_type doesn't work on Vector<double, false>" ); 
static_assert( std::is_same < typename base_type< double >::type , double >::value, "base_type doesn't work" ); 

}
