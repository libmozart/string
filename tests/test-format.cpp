/**
 * Mozart++ Template Library
 * Licensed under MIT License
 * Copyright (c) 2020 Covariant Institute
 * Website: https://covariant.cn/
 * Github:  https://github.com/covariant-institute/
 */

#include <mozart++/format>
#include <iostream>

int main() {
    auto s = mpp::format("hello {} {} {}", 0.1 + 0.2);
    printf("%s\n", s.c_str());

    mpp::format(std::cout, "this is {} formatter", "mpp");

    return 0;
}
