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

namespace math {

//
// clamp
//

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

//
// enumerate
//

template<typename T>
struct enumerate_iterator {
    using container_type = T;
    using value_type = typename container_type::value_type;
    using const_iterator = typename container_type::const_iterator;

    enumerate_iterator& operator++() {
        ++iter;
        return *this;
    }
    enumerate_iterator operator++(int) {
        enumerate_iterator<T> result{*this};
        ++(*this);
        return result;
    }

    enumerate_iterator& operator--() {
        --iter;
        return *this;
    }
    enumerate_iterator operator--(int) {
        enumerate_iterator<T> result{*this};
        --(*this);
        return result;
    }

    std::pair<size_t, value_type> operator*() const {
        return std::make_pair(std::distance(container.begin(), iter), *iter);
    }
    bool operator==(enumerate_iterator const& rhs) { return iter == rhs.iter; }
    bool operator!=(enumerate_iterator const& rhs) { return iter != rhs.iter; }
    enumerate_iterator operator+=(size_t n) {
        iter += n;
        return *this;
    }
    enumerate_iterator operator-=(size_t n) {
        iter -= n;
        return *this;
    }

    const_iterator iter;
    container_type const& container;
};

template<typename T>
inline static enumerate_iterator<T> operator+(enumerate_iterator<T> const& iter, size_t n) {
    auto result = iter;
    result += n;
    return result;
}
template<typename T>
inline static enumerate_iterator<T> operator+(size_t n, enumerate_iterator<T> const& iter) {
    return iter + n;
}
template<typename T>
inline static enumerate_iterator<T> operator-(enumerate_iterator<T> const& iter, size_t n) {
    auto result = iter;
    result -= n;
    return result;
}


template<typename T>
struct enumerate_impl {
    using container_type = T;
    using value_type = std::pair<std::size_t, typename container_type::value_type>;
    using iterator = enumerate_iterator<T>;

    iterator begin() { return iterator{container.begin(), container}; }
    iterator begin() const { return iterator{container.begin(), container}; }

    iterator end() { return iterator{container.end(), container}; }
    iterator end() const { return iterator{container.end(), container}; }

    T const& container;
};

struct enumerate_tag {};
static enumerate_tag enumerate;

template<typename T>
inline static enumerate_impl<T> operator|(T const& vec, enumerate_tag) {
    return enumerate_impl<T>{vec};
}


// reverse
template<typename T>
struct reverse_impl {
    using container_type = T;
    using value_type = typename container_type::value_type;
    using iterator = typename container_type::reverse_iterator;
    using const_iterator = typename container_type::const_reverse_iterator;

    const_iterator begin() const { return container.rbegin(); }
    const_iterator cbegin() const { return container.crbegin(); }

    const_iterator end() const { return container.rend(); }
    const_iterator cend() const { return container.crend(); }

    container_type const& container;
};

struct reverse_tag {};
static reverse_tag reverse;

template<typename T>
inline static reverse_impl<T> operator|(T const& vec, reverse_tag) {
    return reverse_impl<T>{vec};
}

} // end of util

#ifdef AKITSU_CPP_UTIL_ENABLE_EVIL
#include <memory>

template<typename T>
using ptr = std::unique_ptr<T>;

#endif

#if __cplusplus <= 201103
#include <memory>
#include <iostream> // for debug
namespace std {

template<typename T, typename ... Args>
inline static std::unique_ptr<T> make_unique(Args&& ... args) {
    return std::unique_ptr<T>{new T{std::forward<Args>(args) ...}};
}

}
#endif

#endif
