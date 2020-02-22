/**
 * Mozart++ Template Library
 * Licensed under MIT License
 * Copyright (c) 2020 Covariant Institute
 * Website: https://covariant.cn/
 * Github:  https://github.com/covariant-institute/
 */

#include <mozart++/format>
#include <iostream>

struct obj {
    int a = 0;
    int b = 0;
};

struct objx {
    int a = 0;
    int b = 0;
};

std::ostream &operator<<(std::ostream &out, const objx &x) {
    mpp::format(out, "objxOut{a: {}, b: {}}", x.a, x.b);
    return out;
}

template <>
std::string mpp::to_string(const objx &val) {
    return mpp::format("objxToString{a: {}, b: {}}", val.a, val.b);
}

class only_string_writable {
public:

};

only_string_writable &operator<<(only_string_writable &out, const std::string &s) {
    std::cout << s;
    return out;
}

class nothing_writable {
};

int main() {
    auto s = mpp::format("hello {} {} {}", 0.1 + 0.2);
    printf("%s\n", s.c_str());

    mpp::format(std::cout, "this is {} formatter\n", "mpp");

    const char ss[3] = "it";
    mpp::format(std::cout, "love {}\n", ss);

    char sss[3] = {'i', 's', '_'};
    mpp::format(std::cout, "This {} shit\n", sss);

    const char *f = "lover, fucker";
    mpp::format(std::cout, "hop, {}\n", f);

    mpp::format(std::cout, "object is {}\n", obj{});
    mpp::format(std::cout, "objectX is {}\n", objx{.a = 10, .b = 20});

    only_string_writable op;
    mpp::format(op, "test must be string {}", 'a');

    nothing_writable nop;
    mpp::format(nop, "this line will never be formatted", 'f');
    return 0;
}
