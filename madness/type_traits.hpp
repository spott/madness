#pragma once

#include<type_traits>

namespace madness{

template <typename T>
using unqualified = std::remove_cv<std::remove_reference<T>>;


template <typename V>
struct isVector {
  enum { value = false };
};

template <typename T1,bool T2>
struct isVector< Vector<T1,T2> > {
  enum { value = true };
};

}
