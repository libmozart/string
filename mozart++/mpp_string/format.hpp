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
#include <mozart++/string>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

namespace mpp_impl {
    template <typename T>
    struct format_value_helper {
        static std::string doit(T &&t) {
            return mpp::to_string(std::forward<T>(t));
        }
    };

    template <>
    struct format_value_helper<char> {
        static std::string doit(char c) {
            return std::string(1, c);
        }
    };

    template <size_t N>
    struct format_value_helper<char (&)[N]> {
        static std::string doit(const char *str) {
            return std::string(str);
        }
    };

    template <size_t N>
    struct format_value_helper<const char (&)[N]> {
        static std::string doit(const char *str) {
            return std::string(str);
        }
    };

    template <typename T>
    std::string format_value(T &&t) {
        return format_value_helper<T>::doit(std::forward<T>(t));
    }

    template <typename Out, typename Callback>
    bool try_format_partially(Out &out, mpp::string_ref &fmt, const std::regex &re, Callback f) {
        using IterT = decltype(fmt.begin());

        IterT begin = fmt.begin();
        IterT end = fmt.end();

        std::regex_iterator<IterT> re_begin(begin, end, re), re_end;
        if (re_begin != re_end) {
            out << fmt.slice(0, re_begin->position(0)).str();
            out << f(*re_begin);
            fmt = fmt.substr(re_begin->position(0) + re_begin->length(0));
            return true;
        }
        return false;
    }

    template <typename Out, typename T>
    bool format_impl(Out &out, mpp::string_ref &fmt, T &&t) {
        std::regex r("\\{\\}");
        using IterT = decltype(fmt.begin());
        return try_format_partially(out, fmt, r, [&](const std::match_results<IterT> &sm) {
            return format_value(std::forward<T>(t));
        });
    }

    template <typename Out, typename T, typename ...Args>
    void format_impl(Out &out, mpp::string_ref &fmt, T &&head, Args &&... args) {
        if (!mpp_impl::format_impl(out, fmt, std::forward<T>(head))) {
            return;
        }
        mpp_impl::format_impl(out, fmt, std::forward<Args>(args)...);
    }

    template <typename Out>
    void format(Out &out, const std::string &fmt) {
        out << fmt;
    }

    template <typename Out, typename ...Args>
    void format(Out &out, const std::string &fmt, Args &&... args) {
        mpp::string_ref fmt_ref{fmt};
        mpp_impl::format_impl(out, fmt_ref, std::forward<Args>(args)...);
        if (!fmt_ref.empty()) {
            out << fmt_ref.str();
        }
    }
}

namespace mpp {
    using mpp_impl::format;

    std::string format(const std::string &fmt) {
        return fmt;
    }

    template <typename ...Args>
    std::string format(const std::string &fmt, Args &&... args) {
        std::stringstream out;
        format(out, fmt, std::forward<Args>(args)...);
        return out.str();
    }
}
