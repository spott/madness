#include "madness/madness.hpp"

#include<iostream>
#include<complex>

//double operator* ( float a, double b ) { return double(a) * b; };

const std::complex<double> operator+(float a, const std::complex<double> b ) { return std::complex<double>(a) + b; };


int main( int argc, char ** argv)
{
    using namespace madness;

    //std::cout << (is_vector<Vector< std::complex<double>, false > >::value ? "true" : "false") << std::endl;

    
    //add<std::complex<double>, float >( std::complex<double>(0,0), 0.0f );
    //add<std::complex<float>, int >();
    Vector<std::complex<double>, false> a(10);
    Vector<std::complex<double>, false> b(10);
    Vector<std::complex<double>, false> c(10);

    for (size_t i = 0; i < 10; i++)
    {
        a[i] = i;
        b[i] = 2*i;
        c[i] = i+1;
    }

    std::cout << "add: " << std::endl;
    Vector<std::complex<double>, false> d = 1. * b;
    //Vector<std::complex<double>,false> d = b + std::complex<double>(10.) * a - c * b;

    for (const auto& it : d)
        std::cout << " " << it << std::endl;
    //std::cout << "sub: " << std::endl;
    //d = b - a;
    //for (const auto& it : d)
        //std::cout << " " << it << std::endl;
    //std::cout << "mult: " << std::endl;
    //d = b * a;
    //for (const auto& it : d)
        //std::cout << " " << it << std::endl;
}

