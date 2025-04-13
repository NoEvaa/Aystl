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
#include "aystl/core/type_traits/common.hpp"

using namespace iin;

TEST_CASE("is any of") {
    CHECK(is_any_of_v<std::true_type, std::true_type>);
    CHECK(is_any_of_v<std::true_type, std::false_type>);
    CHECK(is_any_of_v<std::false_type, std::true_type>);
    CHECK(!is_any_of_v<std::false_type, std::false_type>);
}

TEST_CASE("is all of") {
    CHECK(is_all_of_v<std::true_type, std::true_type>);
    CHECK(!is_all_of_v<std::true_type, std::false_type>);
    CHECK(!is_all_of_v<std::false_type, std::true_type>);
    CHECK(!is_all_of_v<std::false_type, std::false_type>);
}

TEST_CASE("is any same of") {
    CHECK(is_any_same_of_v<int, double, int>);
    CHECK(!is_any_same_of_v<char, double, int>);
}

