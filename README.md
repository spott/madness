madness
=======

A domain specific language for linear algebra in C++11... or at least it will be, at the moment it is just a vector library.

Basic Usage
------

Everything is in namespace `madness`.

`Vector` is a templated class with two template parameters:  the type `T` and the storage order `bool SO` (defaults to column, currently doesn't matter).  Create a Vector like so:

    Vector< double > a( 5 ); // create a vector of 10 elements
    a = 20; //set all values to 20
    Vector< double > b( { 1, 2, 3, 4, 5 } ); //create a vector using std::initializer_list;

Then you can add/subtract/ and multiply them:

    Vector< double > c = a + b * (b - a); //vector is assigned the result of the expression, without intermediates.

You can also carry around intermediates:

    auto d = a + b*b;
    auto e = d + b;
    double f = e[0];

but be warned the intermediates can change when you change the original vectors:

    auto d = a + b*b; // == { 21, 24, 29, 36, 45 };
    b = 0;
    Vector<double> c ( d ); // c == { 20, 20, 20, 20, 20 };  d changed because b changed.

also watch out  for memory errors.  The intermediate carries around a reference to it's members, when the member is deleted, you now have a reference to undefined...

Vectors have `begin` and `end` functions, with iterators: you can iterate over a vector:

    for (auto& it: c)
        std::cout << it << " ";
    for ( auto it = c.begin(); it < c.end(); ++it)
        *it = 10; //set all elements to 10;

Internals (aka learning what I did)
------
Start with the `VectorExpression.hpp` header, and use `type_traits.hpp` for all the type templates that aren't clear.

