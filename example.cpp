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
    for (auto const& e : vec | util::enumerate) {
        std::cout << e.first << ": " << e.second << std::endl;
    }
}

