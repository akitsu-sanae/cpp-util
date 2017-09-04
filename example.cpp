#define AKITSU_CPP_UTIL_ENABLE_EVIL
#include <iostream>
#include "cpp-util.hpp"

#include <memory>
#include <vector>

int main() {
    int a = -130;
    a <= util::math::clamp(0, 640);
    std::cout << a << std::endl;
    std::cout << util::format("{} + {} = {}", 1.0, 2, 3.0) << std::endl;

    std::make_unique<int>(42);

    auto println = [&](int e) { std::cout << e << std::endl; };
    auto println_with_index = [&](std::pair<int, int const&> e) {
        std::cout << util::format("index {}: {}", e.first, e.second) << std::endl;
    };

    std::vector<int> vec{13, 42, 114514};
    util::container(vec) | println;

    std::cout << "-=-=-=-= + 1 =-=-=-=-" << std::endl;
    util::mut_container(vec) | [](int& e) { e += 1; };

    std::cout << "-=-=-=-= enumerate =-=-=-=-" << std::endl;
    util::container(vec).enumerate() | println_with_index;
    std::cout << "-=-=-=-= mutable enumerate =-=-=-=-" << std::endl;
    util::mut_container(vec).enumerate() | [](std::pair<int, int&> e) {
        e.second += e.first;
    };
    util::container(vec) | println;

    std::cout << "-=-=-=-= reverse =-=-=-=-" << std::endl;
    util::container(vec).reverse() | println;

    std::cout << "-=-=-=-= reverse |> enumerate =-=-=-=-" << std::endl;
    util::container(vec).reverse().enumerate() | println_with_index;

    std::cout << "-=-=-=-= enumerate |> reverse =-=-=-=-" << std::endl;
    util::container(vec).enumerate().reverse() | println_with_index;

    std::cout << "-=-=-=-= fold_left =-=-=-=-" << std::endl;
    std::cout << std::boolalpha;
    std::cout << "sum: " << util::container(vec).fold_left(0, [](int l, int r) { return l + r;}) << std::endl;

    std::cout << "-=-=-=-= syntax sugar, printing std::map, string literal =-=-=-=-" << std::endl;
    std::cout << std::map<std::string, int> {
        "hoge"_s >> 12,
        "fuga"_s >> 42,
        "piyo"_s >> 114,
        "nyan"_s >> 514
    } << std::endl;

    std::cout << "-=-=-=-= range =-=-=-=-" << std::endl;
    util::range(vec)
        .map([](auto const& i) { return i * 2; })
        .foreach([](auto const& i) { std::cout << i << std::endl; });

    std::cout << std::boolalpha;
    std::cout << "Exist x. x == 86 ? :" << util::range(vec).exist([](int i) { return i == 86; }) << std::endl;
    std::cout << "All x. x > 20 ? : " << util::range(vec).all([](int i) { return i > 20; }) << std::endl;
}

