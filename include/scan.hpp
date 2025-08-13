#pragma once

#include <tuple>


#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

// Главная функция
template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<Ts...> scan() { // передайте пакет параметров в scan_result
    return details::scan_result<Ts...>{42};
    // 1. Получаем позиции плейсхолдеров из format_string
    // constexpr auto placeholder_pos = fmt.get_positions();
    
    // // 2. Инициализируем builder
    // details::scan_builder<Ts...> builder;
    
    // // 3. Обрабатываем каждый плейсхолдер
    // [&]<size_t... I>(std::index_sequence<I...>) {
    //     ((process_placeholder<I>(builder, placeholder_pos[I], source)), ...);
    // }(std::make_index_sequence<sizeof...(Ts)>{});
    
    // // 4. Проверяем успешность и возвращаем результат
    // if constexpr (builder.failed) {
    //     []<bool flag = false>(){ static_assert(flag, "Scanning failed"); }();
    // }
    
    // return {builder.current_values, builder.current_pos};

    // details::scan_result<Ts...> result{};
    // constexpr size_t num = fmt.get_number_placeholders();

}

} // namespace stdx