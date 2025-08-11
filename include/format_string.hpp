#pragma once

#include <array>
#include <cstddef>
#include <expected>
#include <mutex>
#include <tuple>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
template <fixed_string f_str>
class format_string {
public:
    constexpr format_string() = default;

    static constexpr std::size_t size() {
        return f_str.size();
    }

    constexpr const char* get_format_string() const {
        return f_str.c_str();
    }

    static constexpr std::size_t get_placeholder_count() {
        static_assert(number_placeholders.has_value(), "Ошибка подсчета количества мест вставки");
        return number_placeholders.value();
    }

    static constexpr auto get_positions() {
        return  placeholder_positions;
    }

private:
    static constexpr auto value = f_str;

    //template<fixed_string str>
    static constexpr std::expected<size_t, parse_error> get_number_placeholders() {
        constexpr size_t N = f_str.size();
        if (!N)
            return 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (f_str.data[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (f_str.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = f_str.data[pos];
                constexpr char valid_specs[] = {'d', 'u', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || f_str.data[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }
            ++pos;
        }

        return placeholder_count;
    }

    static constexpr auto number_placeholders = get_number_placeholders();

    static constexpr std::array<std::pair<size_t, size_t>, get_placeholder_count()> get_placeholder_positions() {
        std::array<std::pair<size_t, size_t>, get_placeholder_count()> positions{};
        size_t index = 0;
        
        for (size_t pos = 0; pos < f_str.size() && index < positions.size(); ) {
            if (f_str.data[pos] == '{') {
                size_t start = pos;
                ++pos;
                while (pos < f_str.size() && f_str.data[pos] != '}') {
                    ++pos;
                }
                if (pos < f_str.size()) {
                    positions[index++] = {start, pos};
                }
            }
            pos++;
        }

        // static_assert(index == get_placeholder_count(), "Количество найденых мест вставки не соответствует определенному ранее.");
        return positions;
    }

    static constexpr auto placeholder_positions = get_placeholder_positions();

};

// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/

// Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
// Функция закомментирована, так как еще не реализованы классы, которые она использует
/*
// Сделайте эту свободную функцию методом класса format_string
template<fixed_string str>
consteval std::expected<size_t, parse_error> get_number_placeholders() {
    constexpr size_t N = str.size();
    if (!N)
        return 0;
    size_t placeholder_count = 0;
    size_t pos = 0;
    const size_t size = N - 1; // -1 для игнорирования нуль-терминатора

    while (pos < size) {
        // Пропускаем все символы до '{'
        if (str.data[pos] != '{') {
            ++pos;
            continue;
        }

        // Проверяем незакрытый плейсхолдер
        if (pos + 1 >= size) {
            return std::unexpected(parse_error{"Unclosed last placeholder"});
        }

        // Начало плейсхолдера
        ++placeholder_count;
        ++pos;

        // Проверка спецификатора формата
        if (str.data[pos] == '%') {
            ++pos;
            if (pos >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Проверяем допустимые спецификаторы
            const char spec = str.data[pos];
            constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
            bool valid = false;

            for (const char s : valid_specs) {
                if (spec == s) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                return std::unexpected(parse_error{"Invalid specifier."});
            }
            ++pos;
        }

        // Проверяем закрывающую скобку
        if (pos >= size || str.data[pos] != '}') {
            return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
        }
        ++pos;
    }

    return placeholder_count;
}
*/

// Функция для получения позиций плейсхолдеров

// ваш код здесь
// template <format_string fmt_str>
// constexpr std::array<std::pair<size_t, size_t>, fmt_str.get_placeholder_count()> get_placeholder_positions() {  // сигнатуру тоже нужно изменить
//     size_t num = 0;
//     size_t pos = 0;
//     bool begin_found = false;
//     while (num < fmt_str.get_placeholder_count()) {
//         pos = std::find(fmt_str
//     }

// }

} // namespace stdx::details
