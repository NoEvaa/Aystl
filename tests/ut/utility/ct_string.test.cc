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
#include "aystl/utility/ct_string.hpp"

using namespace iin;

TEST_CASE("ct_str: hello world") {
    using test_str_1 = ct_str<"hello world">;
    CHECK(test_str_1::value.size() == 11);
    CHECK(test_str_1::value.capacity() == 12);
    CHECK(!test_str_1::value.empty());
    CHECK(std::string_view(test_str_1::value) == std::string_view{"hello world"});
    CHECK(test_str_1::value == ct_str_v<"hello world">);
    using test_cs_1 = char_seq_t<test_str_1::value>;
    std::cout << test_cs_1::size() << std::endl;
    CHECK(test_cs_1::size() == 11);
    CHECK(std::is_same_v<test_cs_1, char_seq<char,
          'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>>);
}

TEST_CASE("ct_str: empty") {
    using test_str_1 = ct_str<"">;
    CHECK(test_str_1::value.size() == 0);
    CHECK(test_str_1::value.capacity() == 1);
    CHECK(test_str_1::value.empty());
    CHECK(std::string_view(test_str_1::value) == std::string_view{""});
    CHECK(test_str_1::value == test_str_1::value);
    CHECK(char_seq_t<test_str_1::value>::size() == 0);
}

TEST_CASE("ct_str: +") {
    using test_str_1 = ct_str<"abcdef">;
    using test_str_2 = ct_str<"123456">;
    using test_str_12 = ct_str<"abcdef123456">;
    CHECK(test_str_1::value + test_str_2::value == test_str_12::value);
}

