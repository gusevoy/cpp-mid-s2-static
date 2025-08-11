#include "../include/scan.hpp"
#include "../include/types.hpp"

#include <iostream>

using namespace std::string_literals;

int main() { 
    //static_assert(stdx::scan<stdx::details::format_string{}, stdx::details::fixed_string{}, int>().i == 42);

    constexpr stdx::details::fixed_string<char, 6> fs1{"Hello"};
    std::cout << "fs1: "s << fs1.c_str() << "\n";


    constexpr stdx::details::fixed_string<char, 10> fs2{"World"};
    std::cout << "fs2: " << fs2.c_str() << "\n";

    static constexpr char arr[] = {'T', 'e', 's', 't'};
    constexpr const char* arr_begin = &arr[0];
    constexpr const char* arr_end = &arr[4];
    constexpr stdx::details::fixed_string<char, 5> fs3(arr_begin, arr_end);
    std::cout << "fs3: " << fs3.c_str() << "\n";

    stdx::details::parse_error pe{"Error message"};
    std::cout << "pe: " << pe.c_str() << "\n";

    constexpr auto fmt1 = stdx::details::format_string<"format string test {} test2">{};
    std::cout << "fmt1: " << fmt1.get_format_string() << " [N=" << fmt1.size() << ", PH:" << fmt1.get_placeholder_count() << "]" << std::endl;

    constexpr auto fmt2 = stdx::details::format_string<"{}format string test {} test2 {} test3{}">{};
    std::cout << "fmt2: " << fmt2.get_format_string() << " [N=" << fmt2.size() << ", PH:" << fmt2.get_placeholder_count() << "]" << std::endl;

    // должна бросать static_assert
    // constexpr auto fmt3 = stdx::details::format_string<"format string test { test2 {} test3">{};
    // std::cout << "fmt3: " << fmt3.get_format_string() << " [N=" << fmt3.size() << ", PH:" << fmt3.get_placeholder_count() << "]" << std::endl;

    std::cout << "fmt2 positions: ";
    for (const auto& pair : fmt2.get_positions()) {
        std::cout << pair.first << "-" << pair.second << "  ";
    }
    std::cout << std::endl;

}
