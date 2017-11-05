/*============================================================================
  Copyright (C) 2017 akitsu sanae
  https://github.com/akitsu-sanae/cpp-util
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef AKITSU_CPP_UTIL_HPP
#define AKITSU_CPP_UTIL_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>

namespace util {

namespace math {

template<typename T>
inline static T clamp(T const& min, T const& max, T const& val) {
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

} // end of math


//
// format
//

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

std::string& trim_left(std::string& str) {
    auto start = std::begin(str);
    while (std::isspace(*start))
        ++start;
    str.erase(std::begin(str), start);
    return str;
}

std::string& trim_right(std::string& str) {
    auto last = std::end(str);
    while (std::isspace(*(last-1)))
        --last;
    str.erase(last, std::end(str));
    return str;
}

std::string& trim(std::string& str) {
    trim_left(str);
    trim_right(str);
    return str;
}

std::string trim_left(std::string const& str) {
    auto result = str;
    util::trim_left(result);
    return result;
}

std::string trim_right(std::string const& str) {
    auto result = str;
    util::trim_right(result);
    return result;
}

std::string trim(std::string const& str) {
    auto result = str;
    util::trim(result);
    return result;
}

template<typename R, typename T, typename F>
inline static auto fold(R const& head, std::vector<T> const& vec, F const& f) {
    R result = head;
    for (auto const& e : vec)
        result = f(result, e);
    return result;
}

template<typename T, typename F>
inline static auto fold(std::vector<T> const& vec, F const& f) {
    T result = vec[0];
    for (auto it=vec.begin()+1; it!=vec.end(); ++it)
        result = f(result, *it);
    return result;
}

} // end of util

#ifdef AKITSU_CPP_UTIL_ENABLE_EVIL
#include <memory>

template<typename T>
using ptr = std::unique_ptr<T>;

template<typename T, typename U>
std::pair<T, U> operator>>(T const& t, U const& u) {
    return std::make_pair(t, u);
}

std::string operator"" _s(char const* str, std::size_t length) {
    return std::string{str, length};
}

#endif // end of evil

#ifdef AKITSU_CPP_UTIL_ENABLE_DEBUG_PRINT
#include <vector>
#include <ostream>

template<typename T>
inline static std::ostream& operator<<(std::ostream& os, std::vector<T> const& vec) {
    os << "Vec{";
    for (auto it=vec.begin(); it!=vec.end(); ++it) {
        if (it != vec.begin())
            os << ", ";
        os << *it;
    }
    os << "}";
    return os;
}

#include <utility>

template<typename T, typename U>
inline static std::ostream& operator<<(std::ostream& os, std::pair<T, U> const& pair) {
    os << "Pair{" << pair.first << ", " << pair.second << "}";
    return os;
}

#include <map>
template<typename Key, typename Val>
std::ostream& operator<<(std::ostream& os, std::map<Key, Val> const& map) {
    os << "Map{";
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it != map.begin())
            os << ", ";
        os << it->first << " >> " << it->second;
    }
    os << "}";
    return os;
}

#endif // end of debug print

#endif
