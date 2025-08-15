#include <gtest/gtest.h>
#include <string>

#include "../include/scan.hpp"
#include "../include/types.hpp"

using namespace stdx::details::literals;

TEST(ScanTest, SimpleTest) {
    auto result = stdx::scan<"test{}"_fs, "test1", int>();
    EXPECT_EQ(std::get<0>(result.values), 1);
}

TEST(ScanTest, AllTypesTest) {
    auto result = stdx::scan<
        "test 0{} 1{} 2{} 3{} 4{} 5{} 6{} 7{} 8{}"_fs,
        "test 0-11 1-22 2-33 3-44 455 566 677 788 8str",
        int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, std::string_view>();

    EXPECT_EQ(std::get<0>(result.values), -11);
    EXPECT_EQ(std::get<1>(result.values), -22);
    EXPECT_EQ(std::get<2>(result.values), -33);
    EXPECT_EQ(std::get<3>(result.values), -44);
    EXPECT_EQ(std::get<4>(result.values), 55);
    EXPECT_EQ(std::get<5>(result.values), 66);
    EXPECT_EQ(std::get<6>(result.values), 77);
    EXPECT_EQ(std::get<7>(result.values), 88);
    EXPECT_EQ(std::get<8>(result.values), "str");
}

TEST(ScanTest, AllTypesConstTest) {
    auto result = stdx::scan<
        "test 0{} 1{} 2{} 3{} 4{} 5{} 6{} 7{} 8{}"_fs,
        "test 0-11 1-22 2-33 3-44 455 566 677 788 8str",
        const int8_t, const int16_t, const int32_t, const int64_t, const uint8_t, const uint16_t, const uint32_t, const uint64_t, const std::string_view>();

    EXPECT_EQ(std::get<0>(result.values), -11);
    EXPECT_EQ(std::get<1>(result.values), -22);
    EXPECT_EQ(std::get<2>(result.values), -33);
    EXPECT_EQ(std::get<3>(result.values), -44);
    EXPECT_EQ(std::get<4>(result.values), 55);
    EXPECT_EQ(std::get<5>(result.values), 66);
    EXPECT_EQ(std::get<6>(result.values), 77);
    EXPECT_EQ(std::get<7>(result.values), 88);
    EXPECT_EQ(std::get<8>(result.values), "str");
}

TEST(ScanTest, AllFormatsTest) {
    auto result = stdx::scan<
        "test 0{%d} 1{%d} 2{%d} 3{%d} 4{%u} 5{%u} 6{%u} 7{%u} 8{%s}"_fs,
        "test 0-11 1-22 2-33 3-44 455 566 677 788 8str",
        int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, std::string_view>();

    EXPECT_EQ(std::get<0>(result.values), -11);
    EXPECT_EQ(std::get<1>(result.values), -22);
    EXPECT_EQ(std::get<2>(result.values), -33);
    EXPECT_EQ(std::get<3>(result.values), -44);
    EXPECT_EQ(std::get<4>(result.values), 55);
    EXPECT_EQ(std::get<5>(result.values), 66);
    EXPECT_EQ(std::get<6>(result.values), 77);
    EXPECT_EQ(std::get<7>(result.values), 88);
    EXPECT_EQ(std::get<8>(result.values), "str");
}

TEST(ScanTest, FormatStringTest) {
    auto result = stdx::scan<"{}_{}"_fs, "{_}", std::string_view, std::string_view>();
    EXPECT_EQ(std::get<0>(result.values), "{");
    EXPECT_EQ(std::get<1>(result.values), "}");
}

TEST(ScanTest, CompileTimeFailTest) {

    // Ничего лучше не придумал. Раскомментирование любой строки в этом тесте ведет
    // к ошибке компиляции с внятным сообщением.

    // неверные строки формата - ошибка времени компиляции
    // constexpr auto format_string_1 = "test {unknown format}"_fs;
    // constexpr auto format_string_2 = "test {"_fs;
    // constexpr auto format_string_3 = "{%s test"_fs;

    // несоответствие типов формату - ошибка времени компиляции
    // auto result_1 = stdx::scan<"test {%d}"_fs, "test 1", std::string>();
    // auto result_2 = stdx::scan<"test {%u}"_fs, "test 1", int>();
    
    // ошибка при парсинге источника - ошибка времени компиляции
    // auto result_3 = stdx::scan<"test {%d}"_fs, "test wrong", int>();
    // auto result_4 = stdx::scan<"test {}"_fs, "test wrong", int>();
    // auto result_5 = stdx::scan<"test {%u}"_fs, "test -1", unsigned int>();

    EXPECT_TRUE(true);
}