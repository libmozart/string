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

class my_op {
public:

};

my_op &operator<<(my_op &out, const std::string &s) {
    std::cout << s;
    return out;
}

class no_op {
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

    my_op op;
    mpp::format(op, "test must be string {}", 'a');

    no_op nop;
    mpp::format(nop, "this line will never be formatted", 'f');
    return 0;
}
