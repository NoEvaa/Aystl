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

TEST_CASE("ct_str") {
    using test_str_1 = ct_str_t<"hello world">;
    CHECK(test_str_1::value.size() == 11);
    CHECK(test_str_1::value.capacity() == 12);
    CHECK(!test_str_1::value.empty());
    CHECK(std::string_view(test_str_1::value) == std::string_view{"hello world"});
    CHECK(test_str_1::value == ct_str_t<"hello world">::value);
}

TEST_CASE("ct_str: empty") {
    using test_str_1 = ct_str_t<"">;
    CHECK(test_str_1::value.size() == 11);
    CHECK(test_str_1::value.capacity() == 12);
    CHECK(!test_str_1::value.empty());
    CHECK(std::string_view(test_str_1::value) == std::string_view{"hello world"});
}

