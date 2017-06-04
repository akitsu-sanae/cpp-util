#define AKITSU_CPP_UTIL_ENABLE_EVIL
#include "cpp-util.hpp"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    int a = -130;
    a <= util::math::clamp(0, 640);
    std::cout << a << std::endl;
    std::cout << util::format("{} + {} = {}", 1.0, 2, 3.0) << std::endl;

    std::make_unique<int>(42);

    std::vector<int> vec{13, 42, 114514};
    std::cout << "-=-=-=-= plain =-=-=-=-" << std::endl;
    for (auto const& e : vec)
        std::cout << e << std::endl;

    std::cout << "-=-=-=-= enumerate =-=-=-=-" << std::endl;
    for (auto const& e : vec | util::enumerate)
        std::cout << util::format("{}: {}", e.first, e.second) << std::endl;
    std::cout << "-=-=-=-= mutable enumerate =-=-=-=-" << std::endl;
    for (auto& e : vec | util::mutable_enumerate) {
        std::cout << util::format("{} + {} = {}", e.second, e.first, e.second + e.first) << std::endl;
        e.second += e.first;
    }

    std::cout << "-=-=-=-= reverse =-=-=-=-" << std::endl;
    for (auto const& e : vec | util::reverse)
        std::cout << e << std::endl;

    std::cout << "-=-=-=-= reverse |> enumerate =-=-=-=-" << std::endl;
    for (auto const& e : vec | util::reverse | util::enumerate)
        std::cout << util::format("{}: {}", e.first, e.second) << std::endl;

    std::cout << "-=-=-=-= enumerate |> reverse =-=-=-=-" << std::endl;
    for (auto const& e : vec | util::enumerate | util::reverse)
        std::cout << util::format("{}: {}", e.first, e.second) << std::endl;


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

