#include "../include/parse.hpp"
#include "../include/scan.hpp"
#include "../include/types.hpp"

#include <cstddef>
#include <iostream>
#include <string_view>

//using namespace stdx::details;
using namespace stdx::details::literals;

void print_src_info(const std::pair<std::size_t, std::size_t> src, std::string_view note = "") {
    std::cout << "src " << note << ": " << src.first << "-" << src.second << "\n";
}

template <typename FS>
void print_fmt_info(const FS& fmt, std::string_view note = "") {
    std::cout << "fmt " << note << ": " << fmt.get_format_string() << " [N=" << fmt.size() << ", PH:" << fmt.get_placeholder_count() << "]" << std::endl;
    std::cout << "positions: ";
    for (const auto& pair : fmt.get_positions()) {
        std::cout << pair.first << "-" << pair.second << "  ";
    }
    std::cout << std::endl;
}


int main() { 
    //static_assert(stdx::scan<stdx::details::format_string{}, stdx::details::fixed_string{}, int>().i == 42);

    constexpr stdx::details::fixed_string<char, 6> fs1{"Hello"};
    std::cout << "fs1: " << fs1.c_str() << "\n";


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
    print_fmt_info(fmt1, "1");

    constexpr auto fmt2 = stdx::details::format_string<"{}format string test {} test2 {} test3{}">{};
    print_fmt_info(fmt2, "2");

    // должна бросать static_assert
    // constexpr auto fmt3 = stdx::details::format_string<"format string test { test2 {} test3">{};
    // print_fmt_info(fmt3);

    constexpr auto fmt4 = "{} literal test {} test 2{} test3{}"_fs;
    print_fmt_info(fmt4, "4");

    // не должно компилироваться
    // constexpr auto fmt5 = "{test"_fs;
    // print_fmt_info(fm5, "5");

    constexpr auto sr0 = stdx::details::get_current_source_for_parsing<0, "{} literal test {} test 2{} test3{}"_fs, "raz literal test 2 test 2{} test3{{{{">();
    constexpr auto sr1 = stdx::details::get_current_source_for_parsing<1, "{} literal test {} test 2{} test3{}"_fs, "raz literal test 2 test 2{} test3{{{{">();
    constexpr auto sr2 = stdx::details::get_current_source_for_parsing<2, "{} literal test {} test 2{} test3{}"_fs, "raz literal test 2 test 2{} test3{{{{">();
    constexpr auto sr3 = stdx::details::get_current_source_for_parsing<3, "{} literal test {} test 2{} test3{}"_fs, "raz literal test 2 test 2{} test3{{{{">();
    print_src_info(sr0, "0");
    print_src_info(sr1, "1");
    print_src_info(sr2, "2");
    print_src_info(sr3, "3");

    constexpr auto fmt5 = "{} literal test {} test 2{} test3{}"_fs;
    constexpr stdx::details::fixed_string source{"raz literal test 3.3 test 2{} test3{{{{"};
    // constexpr int i = 1;
    // constexpr auto tmp_sr = stdx::details::get_current_source_for_parsing<i, fmt5, source>();
    // auto res1 = stdx::details::parse_value<int>({source.data + tmp_sr.first, source.data + tmp_sr.second});
    // if (res1.has_value()) {
    //     std::cout << "res1: " << res1.value() << "\n";
    // }
    // else {
    //     std::cout << "res1: ERROR " << res1.error().c_str() << "\n";
    // }

    print_src_info(fmt5.get_positions()[1], "fmt5-1");

    auto res2 = stdx::details::parse_input<int, 1, fmt5, source>();
    std::cout << "res2: " << res2 << "\n";
}
