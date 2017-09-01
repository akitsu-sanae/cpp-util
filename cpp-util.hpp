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
template<typename Container>
struct container_impl {
    using inner_container_type = Container;
    using this_type = container_impl<inner_container_type>;
    using value_type = typename inner_container_type::value_type;
    using element_type = value_type;
    using reference = typename inner_container_type::reference;
    using const_reference = typename inner_container_type::const_reference;
    using iterator = typename inner_container_type::iterator;
    using const_iterator = typename inner_container_type::const_iterator;
    using difference_type = typename inner_container_type::difference_type;
    using size_type = typename inner_container_type::size_type;

    explicit container_impl() {}
    explicit container_impl(std::function<size_type (size_type)> const& f) :
        convert_index{f}
    {}
    container_impl(container_impl const& rhs) :
        convert_index{rhs.convert_index}
    {
        container_holder = std::make_unique<container_const_reference>(rhs.container_holder->inner());
    }
    container_impl(container_impl&& rhs) :
        convert_index{std::move(rhs.convert_index)},
        container_holder{std::move(rhs.container_holder)}
    {}

    size_type size() const { return container_holder->const_inner().size(); }

    value_type const& at(size_type i) const { return container_holder->const_inner()[i]; }
    value_type const& operator[](size_type i) const { return container_holder->const_inner()[i]; }

    template<typename F>
    this_type const& each(F&& f) const {
        for (size_type i=0; i < size(); ++i)
            f(container_holder->const_inner()[convert_index(i)]);
        return *this;
    }
    template<typename F>
    this_type& each(F&& f) {
        for (size_type i=0; i < size(); ++i)
            f(container_holder->inner()[convert_index(i)]);
        return *this;
    }
    template<typename F>
    this_type const& operator|(F&& f) const {
        return each(std::forward<F>(f));
    }
    template<typename F>
    this_type& operator|(F&& f) {
        return each(std::forward<F>(f));
    }
    std::function<size_type (size_type)> convert_index = [](size_type i) {
        return i;
    };

    auto enumerate() const {
        using inner_type = std::vector<std::pair<size_type, value_type const&>>;
        inner_type result_inner;
        result_inner.reserve(size());
        for (size_type i=0; i < size(); ++i)
            result_inner.emplace_back(i, container_holder->const_inner()[convert_index(i)]);
        container_impl<inner_type> result;
        result.container_holder = std::make_unique<typename container_impl<inner_type>::container_instance>(result_inner);
        return result;
    }
    auto enumerate() {
        using inner_type = std::vector<std::pair<size_type, value_type&>>;
        inner_type result_inner;
        result_inner.reserve(size());
        for (size_type i=0; i < size(); ++i)
            result_inner.emplace_back(i, container_holder->inner()[convert_index(i)]);
        container_impl<inner_type> result;
        result.container_holder = std::make_unique<typename container_impl<inner_type>::container_instance>(result_inner);
        return result;
    }

    template<typename F>
    auto map(F f) const {
        std::vector<value_type> result;
        result.reserve(size());
        for (size_type i=0; i < size(); ++i)
            result.push_back(container_holder->const_inner()[convert_index(i)]);
        return result;
    }

    template<typename F>
    auto retain(F f) const {
        std::vector<value_type> result;
        result.reserve(size());
        for (size_type i=0; i < size(); ++i) {
            auto e = container_holder->const_inner()[convert_index(i)];
            if (f(e))
                result.push_back(e);
        }
        result.shirink_to_fit();
        return result;
    }

    auto const reverse() const {
        this_type result{[&](size_type i) {
            return size() - convert_index(i) - 1;
        }};
        result.container_holder = std::make_unique<container_const_reference>(container_holder->const_inner());
        return result;
    }
    auto reverse() {
        this_type result{[&](size_type i) {
            return size() - convert_index(i) - 1;
        }};
        result.container_holder = std::make_unique<container_reference>(container_holder->inner());
        return result;
    }

    struct container_base {
        virtual inner_container_type& inner() = 0;
        virtual inner_container_type const& inner() const = 0;
        inner_container_type const& const_inner() const { return inner(); }
    };
    struct container_instance : public container_base {
        explicit container_instance(inner_container_type cont) :
            cont{cont}
        {}
        inner_container_type& inner() override { return cont; }
        inner_container_type const& inner() const override { return cont; }
        inner_container_type cont;
    };
    struct container_reference : public container_base {
        explicit container_reference(inner_container_type& cont) :
            cont{cont}
        {}
        inner_container_type& inner() override { return cont; }
        inner_container_type const& inner() const override { return cont; }
        inner_container_type& cont;
    };
    struct container_const_reference : public container_base {
        explicit container_const_reference(inner_container_type const& cont) :
            cont{cont}
        {}
        inner_container_type& inner() override { throw std::logic_error{"this is cpp-util's bug"}; }
        inner_container_type const& inner() const override { return cont; }
        inner_container_type const&  cont;
    };
    std::unique_ptr<container_base> container_holder;
};

template<typename Container>
static inline auto const container(Container const& con) {
    container_impl<Container> result;
    result.container_holder = std::make_unique<typename container_impl<Container>::container_const_reference>(con);
    return result;
}

template<typename Container>
static inline auto mut_container(Container& con) {
    container_impl<Container> result;
    result.container_holder = std::make_unique<typename container_impl<Container>::container_reference>(con);
    return result;
}

template<typename T>
struct range_impl {
    using container_type = T;
    using value_type = typename container_type::value_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using difference_type = typename container_type::difference_type;
    using size_type = typename container_type::size_type;

    explicit range_impl(container_type& container) :
        start{container.begin()},
        last{container.end()}
    {}
    explicit range_impl(iterator const& start, iterator const& last) :
        start{start},
        last{last}
    {}
    template<typename F>
    range_impl& map(F const& f) {
        for (auto it = start; it != last; ++it)
            *it = f(*it);
        return *this;
    }
    template<typename F>
    range_impl& foreach(F const& f) {
        for (auto it = start; it != last; ++it)
            f(*it);
        return *this;
    }
    template<typename Pred>
    bool exist(Pred const& pred) const {
        for (auto it = start; it != last; ++it) {
            if (pred(*it))
                return true;
        }
        return false;
    }
    template<typename Pred>
    bool all(Pred const& pred) const {
        for (auto it = start; it != last; ++it) {
            if (!pred(*it))
                return false;
        }
        return true;
    }

    iterator start, last;
};

template<typename T>
inline static auto range(T& con) {
    return range_impl<T>{con};
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

#include <map>
#include <ostream>

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
