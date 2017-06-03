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
}

