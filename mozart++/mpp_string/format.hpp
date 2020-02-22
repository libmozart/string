/**
 * Mozart++ Template Library: Format
 * Licensed under MIT License
 * Copyright (c) 2020 Covariant Institute
 * Website: https://covariant.cn/
 * Github:  https://github.com/covariant-institute/
 */

#pragma once

#include <mozart++/core>
#include <mozart++/any>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

namespace mpp_impl {
    template <typename Callback>
    bool try_format_partially(std::string &text, const std::regex &re, Callback f) {
        using IterT = decltype(text.cbegin());

        std::regex_iterator<IterT> begin(text.cbegin(), text.cend(), re), end;
        if (begin != end) {
            std::string s;
            IterT start = text.cbegin() + begin->position(0);
            s.append(text.cbegin(), start);
            s.append(f(*begin));
            s.append(start + begin->length(0), text.cend());
            std::swap(s, text);
            return true;
        }
        return false;
    }

    template <typename T>
    struct format_value_helper {
        static std::string doit(const T &t) {
            return mpp::to_string(t);
        }
    };

    template <>
    struct format_value_helper<char> {
        static std::string doit(char c) {
            return std::string(1, c);
        }
    };

    template <size_t N>
    struct format_value_helper<char[N]> {
        static std::string doit(const char *str) {
            return std::string(str);
        }
    };

    template <typename T>
    std::string format_value(const T &t) {
        return format_value_helper<T>::doit(t);
    }

    template <typename T>
    bool format(std::string &s, const T &t) {
        std::regex r("\\{\\}");
        return try_format_partially(s, r, [&](const std::smatch &sm) {
            return format_value(t);
        });
    }

    template <typename T, typename ...Args>
    void format(std::string &s, const T &head, Args &&... args) {
        if (!mpp_impl::format(s, head)) {
            return;
        }
        mpp_impl::format(s, std::forward<Args>(args)...);
    }
}

namespace mpp {
    std::string format(const std::string &s) {
        return s;
    }

    template <typename ...Args>
    std::string format(std::string s, Args &&... args) {
        mpp_impl::format(s, std::forward<Args>(args)...);
        return s;
    }
}
