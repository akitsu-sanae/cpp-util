/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/cpp-util
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef AKITSU_CPP_UTIL_HPP
#define AKITSU_CPP_UTIL_HPP

#include <string>
#include <stdexcept>

namespace util {

inline static std::string format(std::string const& text) {
    return text;
}

template<typename Head, typename ... Tail>
inline static std::string format(std::string const& text, Head const& head, Tail const& ... tail) {
    auto pos = text.find("{}");
    if (pos == std::string::npos)
        throw std::logic_error{"too few arguments"};
    std::string rest = text.substr(pos+2, text.length());
    return text.substr(0, pos) + std::to_string(head) + format(rest, tail ...);
}


namespace math {

template<typename T>
struct clamp_impl {
    using value_type = T;
    using element_type = T;
    value_type min, max;
};

template<typename T>
inline static clamp_impl<T> clamp(T const& min, T const& max) {
    return clamp_impl<T>{min, max};
}

template<typename T>
inline T& operator<=(T& var, clamp_impl<T> const& c) {
    if (var < c.min)
        var = c.min;
    if (var > c.max)
        var = c.max;
    return var;
}

} // end of math

} // end of util

#ifdef AKITSU_CPP_UTIL_ENABLE_EVIL
#include <memory>

template<typename T>
using ptr = std::unique_ptr<T>;

#endif



#endif
