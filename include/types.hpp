#pragma once

#include <algorithm>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

template <typename CharT, std::size_t N>
struct fixed_string {
    constexpr fixed_string(const CharT (&str)[N]) {
        std::copy_n(str, N, data);
    }

    template <std::size_t M>
    constexpr fixed_string(const CharT (&str)[M]) {
        static_assert(M <= N, "Слишком длинная входная строка.");
        std::copy_n(str, M, data);
    }

    constexpr fixed_string(const CharT* begin, const CharT* end) {
        const std::size_t length = end - begin;
        std::copy_n(begin, length, data);
    }

    CharT data[N]{};

    constexpr const CharT* c_str() const {
        return data;
    }

    constexpr std::size_t size() const {
        return N;
    }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга
struct parse_error : fixed_string<char, 200> {};

// Шаблонный класс для хранения результатов парсинга
template <typename... Ts>
struct scan_result {
  std::tuple<Ts...> values;
};

} // namespace stdx::details
