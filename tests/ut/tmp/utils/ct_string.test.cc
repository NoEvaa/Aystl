/**
 * Copyright 2024 NoEvaa
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "testlib.h"
#include "aystl/tmp/utils/ct_string.hpp"

using namespace iin;

using test_str_1 = ct_str_t<"hello world">;
using test_str_2 = ct_str_t<"">;
using test_str_3 = ct_str_t<"abcdef">;
using test_str_4 = ct_str_t<"123456">;
using test_str_34 = ct_str_t<"abcdef123456">;

TEST_CASE("ct_str: basic") {
    CHECK(test_str_1::value.size() == 11);
    CHECK(test_str_1::value.capacity() == 12);
    CHECK(!test_str_1::value.empty());
    CHECK(std::string_view(test_str_1::value) == std::string_view{"hello world"});
    CHECK(test_str_1::value == ct_str_v<"hello world">);

    CHECK(test_str_2::value.size() == 0);
    CHECK(test_str_2::value.capacity() == 1);
    CHECK(test_str_2::value.empty());
    CHECK(std::string_view(test_str_2::value) == std::string_view{""});
    CHECK(test_str_2::value == test_str_2::value);

    CHECK(std::is_same_v<decltype(ct_str_v<u8"a">)::value_type, char8_t>);
}

TEST_CASE("ct_str: +") {
    CHECK(test_str_3::value + test_str_4::value == test_str_34::value);
}

namespace {
template <ct_str> struct TestOp { int operator()() { return 0; } };
template <> struct TestOp<"1"> { int operator()() { return 1; } };
template <> struct TestOp<"3"> { int operator()() { return 3; } };

template <typename> struct TestOp2 { int operator()() { return 0; } };
template <> struct TestOp2<ct_str_t<"1">> { int operator()() { return 1; } };
template <> struct TestOp2<ct_str_t<"3">> { int operator()() { return 3; } };
}
TEST_CASE("ct_str: template specialization") {
    CHECK(TestOp<"aaa">()() == 0);
    CHECK(TestOp<"1">()() == 1);
    CHECK(TestOp<"3">()() == 3);

    CHECK(TestOp2<int>()() == 0);
    CHECK(TestOp2<ct_str_t<"1">>()() == 1);
    CHECK(TestOp2<ct_str_t<"3">>()() == 3);
}

TEST_CASE("char_seq: basic") {
    using test_cs_1 = char_seq_t<test_str_1::value>;
    CHECK(CharSeqType<test_cs_1>);
    CHECK(test_cs_1::size() == 11);
    CHECK(std::is_same_v<test_cs_1, char_seq<char,
          'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>>);

    CHECK(std::is_same_v<test_cs_1::wrapped<va_list_tmpl_t>,
          value_list<'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>>);

    CHECK(char_seq_t<test_str_2::value>::size() == 0);
}

TEST_CASE("ct_str_substr_v") {
    CHECK(ct_str_substr_v<test_str_1::value, 0, 0> == ct_str_v<"">);
    CHECK(ct_str_substr_v<test_str_1::value, 2, 0> == ct_str_v<"">);
    CHECK(ct_str_substr_v<test_str_1::value, 100, 1> == ct_str_v<"">);
    CHECK(ct_str_substr_v<test_str_1::value, 0, 1> == ct_str_v<"h">);
    CHECK(ct_str_substr_v<test_str_1::value, 1, 2> == ct_str_v<"el">);
    CHECK(ct_str_substr_v<test_str_1::value, 9, 100> == ct_str_v<"ld">);
    CHECK(ct_str_substr_v<test_str_1::value, 10, 0> == ct_str_v<"">);
    CHECK(ct_str_substr_v<test_str_1::value, 10, 1> == ct_str_v<"d">);
    CHECK(ct_str_substr_v<"abcde", 1, 3> == ct_str_v<"bcd">);
}

