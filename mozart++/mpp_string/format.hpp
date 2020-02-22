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
    template <typename Out, typename T>
    void format_value(Out &out, T &&t);

    template <typename T, typename ParamT = T &&>
    struct requires_writable {
        template <typename Out>
        static auto try_write_stream(Out &out, ParamT t, bool) -> decltype(out << std::forward<T>(t)) {
            out << std::forward<T>(t);
        }

        template <typename Out>
        static auto try_write_stream(Out &out, ParamT t, int) -> decltype(out << mpp::to_string(std::forward<T>(t))) {
            out << mpp::to_string(std::forward<T>(t));
        }

        template <typename Out>
        static void try_write_stream(Out &out, ParamT t, long) {
            // the stream cannot write anything, even a string.
        }

        template <typename Out>
        static void doit(Out &out, ParamT t) {
            // Try to use stream operator first,
            // bacause mpp::to_string() is the final fallback.
            try_write_stream(out, std::forward<T>(t), true);
        }
    };

    template <typename T>
    struct format_value_helper : public requires_writable<T> {
    };

    template <>
    struct format_value_helper<char> : public requires_writable<char, char> {
    };

    template <size_t N>
    struct format_value_helper<const char (&)[N]> : public requires_writable<std::string> {
    };

    template <typename T, size_t N>
    struct format_value_helper<T (&)[N]> {
        template <typename Out>
        static void doit(Out &out, const T *arr) {
            format_value(out, "[");
            for (size_t i = 0; i < N; ++i) {
                format_value(out, arr[i]);
                if (i != N - 1) {
                    format_value(out, ", ");
                }
            }
            format_value(out, "]");
        }
    };

    template <typename Out, typename T>
    void format_value(Out &out, T &&t) {
        format_value_helper<T>::doit(out, std::forward<T>(t));
    }

    template <typename Out, typename Callback>
    bool try_format_partially(Out &out, mpp::string_ref &fmt, const std::regex &re, Callback f) {
        using IterT = decltype(fmt.begin());

        IterT begin = fmt.begin();
        IterT end = fmt.end();

        std::regex_iterator<IterT> re_begin(begin, end, re), re_end;
        if (re_begin != re_end) {
            format_value(out, fmt.slice(0, re_begin->position(0)).str());
            f(out, *re_begin);
            fmt = fmt.substr(re_begin->position(0) + re_begin->length(0));
            return true;
        }
        return false;
    }

    template <typename Out, typename T>
    bool format_impl(Out &out, mpp::string_ref &fmt, T &&t) {
        std::regex r("\\{\\}");
        using IterT = decltype(fmt.begin());
        return try_format_partially(out, fmt, r, [&](Out &out, const std::match_results<IterT> &sm) {
            format_value(out, std::forward<T>(t));
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
            format_value(out, fmt_ref.str());
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
