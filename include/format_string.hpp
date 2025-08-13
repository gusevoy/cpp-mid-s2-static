#pragma once

#include <array>
#include <cstddef>
#include <expected>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
template <fixed_string f_str>
class format_string {
public:

    consteval format_string() = default;

    static constexpr auto value = f_str;

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

        return positions;
    }

    static constexpr auto placeholder_positions = get_placeholder_positions();

};

namespace literals {

// Пользовательский литерал
template <fixed_string FS>
consteval auto operator""_fs() {
    return format_string<FS>{};
}

} // namespace stdx::details::literals

} // namespace stdx::details
