#define AKITSU_CPP_UTIL_ENABLE_EVIL
#define AKITSU_CPP_UTIL_ENABLE_DEBUG_PRINT
#include <iostream>
#include "cpp-util.hpp"

#include <memory>
#include <vector>
#include <map>

int main() {
    int a = -130;
    a = util::math::clamp(0, 640, a);
    std::cout << a << std::endl;
    std::cout << util::format("{} + {} = {}", 1.0, 2, 3.0) << std::endl;

    std::cout << std::vector<int>{1, 1, 4, 5, 1, 4} << std::endl;

    std::cout << std::make_pair(220, 284) << std::endl;
    std::cout << std::map<std::string, int> {
        "hoge"_s >> 12,
        "fuga"_s >> 42,
        "piyo"_s >> 114,
        "nyan"_s >> 514
    } << std::endl;

    std::cout << "-=-=-=-= trim =-=-=-=-" << std::endl;
    std::cout << util::trim_left("  hoge") << std::endl;
    std::cout << util::trim_left("hoge  ") << std::endl;
    std::cout << util::trim_left("  hoge  ") << std::endl;

}

