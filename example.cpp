#include "cpp-util.hpp"

#include <iostream>
#include <memory>

int main() {
    int a = -130;
    a <= util::math::clamp(0, 640);
    std::cout << a << std::endl;
    std::cout << util::format("{} + {} = {}", 1.0, 2, 3.0) << std::endl;

    std::make_unique<int>(42);
}

