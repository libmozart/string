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
    using string_iter_t = mpp::string_ref::const_iterator;
    using match_t = std::match_results<string_iter_t>;

    template <typename Out, typename T>
    void write_value(Out &out, T &&t);

    template <typename Out, typename T>
    void write_control(Out &out, const std::string &control);

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
        static void try_write_stream(Out &, ParamT, long) {
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
    struct value_writer : public requires_writable<T> {
    };

    template <>
    struct value_writer<char> : public requires_writable<char, char> {
    };

    template <size_t N>
    struct value_writer<const char (&)[N]> : public requires_writable<std::string> {
    };

    template <typename T, size_t N>
    struct value_writer<T (&)[N]> {
        template <typename Out>
        static void doit(Out &out, const T *arr) {
            write_value(out, "[");
            for (size_t i = 0; i < N; ++i) {
                write_value(out, arr[i]);
                if (i != N - 1) {
                    write_value(out, ", ");
                }
            }
            write_value(out, "]");
        }
    };

    template <typename Out, typename T>
    void write_value(Out &out, T &&t) {
        value_writer<T>::doit(out, std::forward<T>(t));
    }

    template <typename Out, typename T>
    void write_control(Out &out, const std::string &control) {
    }

    template <typename Out, typename T>
    void write_value_and_control(Out &out, T &&t, const match_t &match) {
        write_value(out, std::forward<T>(t));
    }

    template <typename Out, typename T>
    bool try_format_one(Out &out, mpp::string_ref &fmt, const std::regex &re, T &&t) {
        string_iter_t begin = fmt.begin();
        string_iter_t end = fmt.end();

        std::regex_iterator<string_iter_t> re_begin(begin, end, re), re_end;
        if (re_begin != re_end) {
            // text before the match
            write_value(out, fmt.slice(0, re_begin->position(0)).str());
            // the match itself
            write_value_and_control(out, std::forward<T>(t), *re_begin);
            // the rest text to be matched next time
            fmt = fmt.substr(re_begin->position(0) + re_begin->length(0));
            // be greedy
            return true;
        }
        // nothing to match
        return false;
    }

    template <typename Out, typename T>
    bool format_impl(Out &out, mpp::string_ref &fmt, T &&t) {
        std::regex r(R"(\{(\.[0-9]+)?([xX]|[fF])?(\:\-?[0-9]+)?\})");
        return try_format_one(out, fmt, r, std::forward<T>(t));
    }

    template <typename Out, typename T, typename ...Args>
    void format_impl(Out &out, mpp::string_ref &fmt, T &&head, Args &&... args) {
        if (!mpp_impl::format_impl(out, fmt, std::forward<T>(head))) {
            // nothing to format (no {...} was found)
            return;
        }
        mpp_impl::format_impl(out, fmt, std::forward<Args>(args)...);
    }

    template <typename Out>
    void format(Out &out, const std::string &fmt) {
        write_value(out, fmt);
    }

    template <typename Out, typename ...Args>
    void format(Out &out, const std::string &fmt, Args &&... args) {
        mpp::string_ref fmt_ref{fmt};
        mpp_impl::format_impl(out, fmt_ref, std::forward<Args>(args)...);
        if (!fmt_ref.empty()) {
            write_value(out, fmt_ref.str());
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
