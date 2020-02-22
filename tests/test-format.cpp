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

    mpp::format(std::cout, "this is {} formatter\n", "mpp");

    const char ss[3] = "it";
    mpp::format(std::cout, "love {}\n", ss);

    char sss[3] = {'i', 's', '\0'};
    mpp::format(std::cout, "This {} shit\n", sss);
    return 0;
}
