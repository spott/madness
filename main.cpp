#include "madness/madness.hpp"
#include<iostream>
#include<complex>


int main( int argc, char ** argv)
{
    //using namespace madness;

    //std::cout << (is_vector<Vector< std::complex<double>, false > >::value ? "true" : "false") << std::endl;

    //add<std::complex<double>, float >( std::complex<double>(0,0), 0.0f );
    //add<std::complex<float>, int >();
    madness::Vector<std::complex<double>, false> a(10);
    madness::Vector<double, false> b(10);
    madness::Vector<std::complex<double>, false> c(10);

    for (size_t i = 0; i < 10; i++)
    {
        a[i] = i;
        b[i] = 2*i;
        c[i] = i+1;
    }

    std::cout << "add: " << std::endl;
    //auto d = std::complex<double>(1.) * b;
    //std::cout << d[0];
    //madness::Vector<std::complex<double>, false> d = std::complex<double>(0,1.) * b;
    madness::Vector<std::complex<double>,false> d = c + std::complex<double>(0,10.) * a;

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



