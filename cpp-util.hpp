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
struct enumerate_base {
    using value_type = T;
    auto begin() { return m_container.begin(); }
    auto begin() const { return m_container.begin(); }
    auto cbegin() const { return m_container.cbegin(); }

    auto end() { return m_container.end(); }
    auto end() const { return m_container.end(); }
    auto cend() const { return m_container.cend(); }

    bool empty() const { return m_container.empty(); }

    virtual ~enumerate_base() = default;
protected:
    std::vector<value_type> m_container;
};

template<typename T>
struct enumerate_impl : public enumerate_base<std::pair<std::size_t, typename T::value_type const&>> {
    using value_type = std::pair<std::size_t, typename T::value_type const&>;
    using container_type = std::vector<value_type>;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using difference_type = typename container_type::difference_type;
    using size_type = typename T::size_type;

    explicit enumerate_impl(T const& container) {
        this->m_container.reserve(container.size());
        auto it = container.begin();
        size_type index = 0;
        while (it != container.end()) {
            this->m_container.emplace_back(index, *it);
            ++index;
            ++it;
        }
    }
    size_type size() const { return this->m_container.size(); }
};

struct enumerate_tag {};
static enumerate_tag enumerate;

template<
    typename T,
    typename = typename T::value_type> // check whether T is Container
inline static auto operator|(T const& vec, enumerate_tag) {
    return enumerate_impl<T>{vec};
}

template<typename T>
struct mutable_enumerate_impl : public enumerate_base<std::pair<std::size_t, typename T::value_type&>> {
    using value_type = std::pair<std::size_t, typename T::value_type&>;
    using container_type = std::vector<value_type>;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using difference_type = typename container_type::difference_type;
    using size_type = typename T::size_type;

    explicit mutable_enumerate_impl(T& container) {
        this->m_container.reserve(container.size());
        auto it = container.begin();
        size_type index = 0;
        while (it != container.end()) {
            this->m_container.emplace_back(index, *it);
            ++index;
            ++it;
        }
    }
    size_type size() const { return this->m_container.size(); }
};


struct mutable_enumerate_tag {};
static mutable_enumerate_tag mutable_enumerate;
template<
    typename T,
    typename = typename T::value_type> // check whether T is Container
inline static auto operator|(T& vec, mutable_enumerate_tag) {
    return mutable_enumerate_impl<T>{vec};
}

// reverse
template<typename T>
struct reverse_impl {
    using container_type = T;
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename std::reverse_iterator<typename container_type::iterator>;
    using const_iterator = typename std::reverse_iterator<typename container_type::const_iterator>;
    using difference_type = typename container_type::difference_type;
    using size_type = typename container_type::size_type;

    const_iterator begin() const { return const_iterator(container.end()); }
    const_iterator cbegin() const { return const_iterator(container.cend()); }

    const_iterator end() const { return const_iterator(container.begin()); }
    const_iterator cend() const { return const_iterator(container.cbegin()); }

    size_type size() const { return container.size(); }
    bool empty() const { return container.empty(); }

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
