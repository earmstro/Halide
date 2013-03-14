#ifndef HALIDE_UTIL_H
#define HALIDE_UTIL_H

/** \file
 * Various utility functions used internally Halide. */

#include <vector>
#include <string>

// Always use assert, even if llvm-config defines NDEBUG
#ifdef NDEBUG
#undef NDEBUG
#include <assert.h>
#define NDEBUG
#else
#include <assert.h>
#endif

namespace Halide { 
namespace Internal {

/** Build small vectors of up to 6 elements. If we used C++11 and
 * had vector initializers, this would not be necessary, but we
 * don't want to rely on C++11 support. */
//@{
template<typename T>
std::vector<T> vec(T a) {
    std::vector<T> v(1);
    v[0] = a;
    return v;
}

template<typename T>
std::vector<T> vec(T a, T b) {
    std::vector<T> v(2);
    v[0] = a;
    v[1] = b;
    return v;
}

template<typename T>
std::vector<T> vec(T a, T b, T c) {
    std::vector<T> v(3);
    v[0] = a;
    v[1] = b;
    v[2] = c;
    return v;
}

template<typename T>
std::vector<T> vec(T a, T b, T c, T d) {
    std::vector<T> v(4);
    v[0] = a;        
    v[1] = b;
    v[2] = c;
    v[3] = d;
    return v;
}

template<typename T>
std::vector<T> vec(T a, T b, T c, T d, T e) {
    std::vector<T> v(5);
    v[0] = a;        
    v[1] = b;
    v[2] = c;
    v[3] = d;
    v[4] = e;
    return v;
}

template<typename T>
std::vector<T> vec(T a, T b, T c, T d, T e, T f) {
    std::vector<T> v(6);
    v[0] = a;        
    v[1] = b;
    v[2] = c;
    v[3] = d;
    v[4] = e;
    v[5] = f;
    return v;
}
// @}

/** Generate a unique name starting with the given character. It's
 * unique relative to all other calls to unique_name done by this
 * process. Not thread-safe. */
std::string unique_name(char prefix);

/** Test if the first string starts with the second string */
bool starts_with(const std::string &str, const std::string &prefix);

/** Test if the first string ends with the second string */
bool ends_with(const std::string &str, const std::string &suffix);

/** Make sure that a programmer-specified function name is unique.
 * Not thread-safe. */
std::string unique_name(const std::string &name);

}
}

#endif
