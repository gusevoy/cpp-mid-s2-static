#pragma once

#include <charconv>
#include <concepts>
#include <cstddef>
#include <expected>
#include <optional>
#include <string_view>
#include <system_error>
#include <type_traits>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
// Функция закомментирована, так как еще не реализованы классы, которые она использует

template<int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.get_placeholder_count(), "Invalid placeholder index");

    constexpr auto to_sv = [](const auto& fs) {
        return std::string_view(fs.data, fs.size() - 1);
    };

    constexpr auto fmt_sv = to_sv(fmt.value);
    constexpr auto src_sv = to_sv(source);
    constexpr auto positions = fmt.get_positions();

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&]{
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I-1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I-1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&]{
        // Получаем разделитель после текущего плейсхолдера
        if constexpr(fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep = fmt_sv.substr(fmt_end + 1,
            (I < fmt.get_placeholder_count() - 1)
                ? positions[I+1].first - (fmt_end + 1)
                : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}


// Реализуйте семейство функция parse_value

template <typename T>
consteval std::string_view get_type_format() {
    if constexpr (std::is_convertible_v<T, std::string_view>) {
        //static_assert(false, "Я строка");
        return "%s";
    }
    else if constexpr (std::signed_integral<T>) {
        //static_assert(false, "Я число");
        return "%d";
    }
    else if constexpr (std::unsigned_integral<T>) {
        //static_assert(false, "Я натуральное число");
        return "%u";
    }
    else {
        static_assert(false, "Не удалось определить формат для указанного типа.");
    }
    return {};
}

template <typename T>
requires std::integral<T>
constexpr std::expected<T, parse_error> parse_value(std::string_view input) {
    std::remove_cv_t<T> value;
    if (std::from_chars(input.begin(), input.end(), value).ec == std::errc{} ) {
        return value;
    }
    return std::unexpected(parse_error{"No pattern match or parsed value is not in the range representable by the type"});

}

template <typename T>
requires std::is_convertible_v<T, std::string_view>
constexpr std::expected<T, parse_error> parse_value(std::string_view input) {
    return T{input};
}

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера

template <typename value_type, std::size_t I, format_string fmt, fixed_string source>
constexpr value_type parse_input() {
    static_assert(I < fmt.get_positions().size(), "В строке формата нет такого количество плейсхолдеров");
    constexpr auto fmt_positions = fmt.get_positions()[I];
    constexpr std::string_view type_format_string{fmt.value.data + fmt_positions.first + 1, fmt.value.data + fmt_positions.second};
    if constexpr (!type_format_string.empty()) {
        static_assert(get_type_format<value_type>() == type_format_string, "Формат типа в плейсхолдере и запрашиваемый формат не совпадают.");
    }
    constexpr auto positions = stdx::details::get_current_source_for_parsing<I, fmt, source>();
    constexpr auto result = stdx::details::parse_value<value_type>({source.data + positions.first, source.data + positions.second + 1});
    static_assert(result.has_value(), "Ошибка при парсинге");
    return result.value();
}

} // namespace stdx::details
