#include "madness/madness.hpp"
#include<iostream>
#include<complex>
#include<cassert>

int main()
{
    using namespace madness;

    Vector< double > a({ 1, 1, 2, 2});
    Vector< double > b({ 2, 3, 3, 2});
    Vector< double > c( a + b );
    Vector< double > d( {3,4,5,4} );
    Vector< double, true> j( {1, 1, 2, 2} );
    auto sc = a * j;
    assert ( sc == 10 );
    assert (  c == d );
    c = a * b;
    d = {2, 3, 6, 4};
    assert (  c == d );
    c = a - b;
    d = {-1, -2, -1, 0};
    assert (  c == d );
    c = 2.*a;
    d = {2, 2, 4, 4};
    assert (  c == d );


    //SparseMatrix< double > sa( 10, 10, 10 );
    SparseMatrix< double > sa = SparseMatrix<double>::diagonal( 10, 2. );
    sa.Debug_Print();
    SparseMatrix< double > sb = SparseMatrix<double>::tridiagonal( 10, 1. , 2., 3.);
    sb.Debug_Print();
}
