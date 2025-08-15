#pragma once

#include <tuple>


#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

// Главная функция
template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<Ts...> scan() {
    constexpr auto placeholder_pos = fmt.get_positions();

    constexpr auto values = [&]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(details::parse_input<Ts, Is, fmt, source>()...);
    }(std::make_index_sequence<sizeof...(Ts)>{});
    
    return {values};
}

} // namespace stdx